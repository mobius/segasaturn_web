/* Ymir Web frontend — drives the WASM emulator module.
 * Flow: pick BIOS + disc image -> Start -> rAF loop(runFrame -> canvas -> audio pump).
 * Persistence: BIOS and backup RAM live in IndexedDB so reloads keep state.
 */
"use strict";

(() => {
    // ------------------------------------------------------------------
    // Minimal IndexedDB key-value helper
    // ------------------------------------------------------------------
    const idb = {
        _db: null,
        open() {
            return new Promise((resolve, reject) => {
                const req = indexedDB.open("ymir-web", 1);
                req.onupgradeneeded = () => req.result.createObjectStore("kv");
                req.onsuccess = () => resolve((this._db = req.result));
                req.onerror = () => reject(req.error);
            });
        },
        set(key, value) {
            return new Promise((resolve, reject) => {
                const tx = this._db.transaction("kv", "readwrite");
                tx.objectStore("kv").put(value, key);
                tx.oncomplete = resolve;
                tx.onerror = () => reject(tx.error);
            });
        },
        get(key) {
            return new Promise((resolve, reject) => {
                const tx = this._db.transaction("kv", "readonly");
                const req = tx.objectStore("kv").get(key);
                req.onsuccess = () => resolve(req.result);
                req.onerror = () => reject(req.error);
            });
        },
    };

    // ------------------------------------------------------------------
    // UI elements
    // ------------------------------------------------------------------
    const $ = (id) => document.getElementById(id);
    const statusEl = $("status");
    const canvas = $("screen");
    const ctx = canvas.getContext("2d");
    const startBtn = $("startBtn");
    const resetBtn = $("resetBtn");
    const fpsEl = $("fps");
    const setStatus = (t) => (statusEl.textContent = t);

    // ------------------------------------------------------------------
    // Emulator state
    // ------------------------------------------------------------------
    let Module = null;
    let running = false;
    let biosReady = false;
    let discReady = false;
    let audio = { ctx: null, node: null };

    const setButtons = () => {
        startBtn.disabled = !(biosReady && discReady);
        resetBtn.disabled = !running;
    };

    async function setupAudio() {
        if (audio.ctx) return;
        const actx = new AudioContext({ sampleRate: 44100 });
        await actx.audioWorklet.addModule("ymir-web-audio-worklet.js");
        const node = new AudioWorkletNode(actx, "ymir-audio", {
            outputChannelCount: [2],
        });
        node.connect(actx.destination);
        audio = { ctx: actx, node };
    }

    function pumpAudio() {
        if (!audio.node) return;
        const avail = Module.audioAvailable();
        if (avail > 0) {
            const s = Module.readAudio(avail);
            // Copy out of wasm memory before posting
            audio.node.port.postMessage(s.slice());
        }
    }

    // ------------------------------------------------------------------
    // Frame loop (rAF-paced, throttled by audio backlog)
    // ------------------------------------------------------------------
    let frames = 0, fpsTime = 0;
    function loop(t) {
        if (!running) return;
        // If the audio backlog exceeds ~250 ms, skip emulation this frame to let it drain.
        if (Module.audioAvailable() < 11025) {
            const ok = Module.runFrame();
            if (ok === false) {
                running = false;
                setButtons();
                const detail = (Module.getLastRunError && Module.getLastRunError()) || "unknown";
                setStatus(`模拟中断：${detail}`);
                return;
            }
        }
        // Video
        const w = Module.getFrameWidth(), h = Module.getFrameHeight();
        if (w > 0 && h > 0) {
            if (canvas.width !== w || canvas.height !== h) {
                canvas.width = w;
                canvas.height = h;
            }
            const fb = Module.getFramebuffer();
            ctx.putImageData(new ImageData(new Uint8ClampedArray(fb), w, h), 0, 0);
        }
        pumpAudio();
        // FPS
        frames++;
        if (t - fpsTime >= 1000) {
            fpsEl.textContent = `${frames} fps`;
            frames = 0;
            fpsTime = t;
        }
        requestAnimationFrame(loop);
    }

    // ------------------------------------------------------------------
    // Input (Saturn control pad: bit=1 released, 0 pressed)
    // ------------------------------------------------------------------
    const BIT = { Right: 15, Left: 14, Down: 13, Up: 12, Start: 11, A: 10, C: 9, B: 8, R: 7, X: 6, Y: 5, Z: 4, L: 3 };
    const KEYMAP = {
        ArrowRight: BIT.Right, ArrowLeft: BIT.Left, ArrowDown: BIT.Down, ArrowUp: BIT.Up,
        Enter: BIT.Start,
        KeyZ: BIT.A, KeyX: BIT.B, KeyC: BIT.C,
        KeyA: BIT.X, KeyS: BIT.Y, KeyD: BIT.Z,
        KeyQ: BIT.L, KeyW: BIT.R,
    };
    let padMask = 0xFFFF;
    function updatePad(e, pressed) {
        const bit = KEYMAP[e.code];
        if (bit === undefined) return;
        e.preventDefault();
        padMask = pressed ? padMask & ~(1 << bit) : padMask | (1 << bit);
        Module.setPadState(0, padMask);
    }
    addEventListener("keydown", (e) => Module && updatePad(e, true));
    addEventListener("keyup", (e) => Module && updatePad(e, false));

    // ------------------------------------------------------------------
    // File loading
    // ------------------------------------------------------------------
    async function loadBiosFile(file) {
        const buf = new Uint8Array(await file.arrayBuffer());
        if (!Module.loadIPL(buf)) {
            setStatus(`BIOS 加载失败：大小 ${buf.length} 字节（应为 524288）`);
            return;
        }
        await idb.set("bios", buf);
        biosReady = true;
        setStatus("BIOS 已加载" + (discReady ? "" : "，请选择光盘镜像"));
        setButtons();
    }

    // Prefer descriptor formats over raw .iso when several are present.
    const DISC_MAIN_PRIORITY = [".chd", ".cue", ".mds", ".ccd", ".iso"];
    function pickMainDiscFile(names) {
        let best = null, bestRank = 999;
        for (const name of names) {
            const ext = name.slice(name.lastIndexOf(".")).toLowerCase();
            const rank = DISC_MAIN_PRIORITY.indexOf(ext);
            if (rank >= 0 && rank < bestRank) {
                best = name;
                bestRank = rank;
            }
        }
        return best;
    }
    async function writeRomsAndLoad(entries) {
        Module.FS.mkdirTree("/roms");
        // Flat MEMFS: browser File.name and tiny-unzip both supply basenames only.
        // Cue FILE lines with subdirectories (e.g. "Track/game.bin") will not resolve.
        const written = [];
        for (const [name, data] of entries) {
            const base = String(name).split(/[/\\]/).pop();
            Module.FS.writeFile(`/roms/${base}`, data);
            written.push(base);
        }
        const mainFile = pickMainDiscFile(written);
        if (!mainFile) {
            setStatus("未找到主镜像文件（.chd/.cue/.iso 等）");
            return;
        }
        setStatus(`正在解析 ${mainFile} …`);
        await new Promise((r) => setTimeout(r, 30));
        if (!Module.loadDisc(`/roms/${mainFile}`)) {
            const detail = (Module.getLastMediaError && Module.getLastMediaError()) || "";
            setStatus(
                `光盘解析失败：${mainFile}` +
                    (detail ? ` — ${detail}` : "") +
                    "（多文件镜像请把 .cue 与所有 .bin 一起选中且文件名与 cue 的 FILE 行一致，或改用 .chd / 扁平 zip）"
            );
            return;
        }
        discReady = true;
        const warn = (Module.getLastMediaError && Module.getLastMediaError()) || "";
        const bootable = !(Module.isDiscBootable) || Module.isDiscBootable();
        const title = (Module.getDiscTitle && Module.getDiscTitle().trim()) || "";
        if (!bootable || warn) {
            setStatus(
                `光盘已插入但可能无法启动游戏` +
                    (title ? `（${title}）` : "") +
                    (warn ? `：${warn}` : "。") +
                    " BIOS 语言/CD 菜单可用，但进游戏请用 Z=A 确认；镜像异常时请换 CHD 或核对 cue 扇区 MODE。"
            );
        } else {
            setStatus(
                `光盘已加载${title ? `：${title}` : ""}，可以启动。` +
                    ` BIOS 里确认请按 Z（A 键），Enter 只是 Start。`
            );
        }
        setButtons();
    }
    async function loadDiscZip(file) {
        setStatus(`正在解压 ${file.name} …`);
        await new Promise((r) => setTimeout(r, 30));
        if (typeof ymirUnzip !== "function") {
            setStatus("解压模块未加载");
            return;
        }
        let files;
        try {
            files = await ymirUnzip(new Uint8Array(await file.arrayBuffer()));
        } catch (err) {
            setStatus(`ZIP 解压失败：${err.message ?? err}`);
            return;
        }
        const names = Object.keys(files);
        setStatus(`已解压 ${names.length} 个文件，正在写入 …`);
        await writeRomsAndLoad(names.map((n) => [n, files[n]]));
    }
    async function loadDiscFiles(files) {
        if (files.length === 1 && /\.zip$/i.test(files[0].name)) {
            await loadDiscZip(files[0]);
            return;
        }
        // Mixed selection: if any .zip is present among multiple files, reject clearly.
        if (files.some((f) => /\.zip$/i.test(f.name))) {
            setStatus("请单独上传一个 .zip，或只选择未压缩的镜像文件（不要混选）");
            return;
        }
        const entries = [];
        for (const f of files) {
            entries.push([f.name, new Uint8Array(await f.arrayBuffer())]);
        }
        await writeRomsAndLoad(entries);
    }

    $("biosFile").addEventListener("change", (e) => e.target.files[0] && loadBiosFile(e.target.files[0]));
    $("discFiles").addEventListener("change", (e) => e.target.files.length && loadDiscFiles([...e.target.files]));

    // ------------------------------------------------------------------
    // Start / reset
    // ------------------------------------------------------------------
    startBtn.addEventListener("click", async () => {
        await setupAudio();
        await audio.ctx.resume();
        Module.reset(true);
        running = true;
        setButtons();
        setStatus("运行中");
        fpsTime = performance.now();
        frames = 0;
        requestAnimationFrame(loop);
    });
    resetBtn.addEventListener("click", () => Module.reset(true));

    // ------------------------------------------------------------------
    // Backup RAM persistence
    // ------------------------------------------------------------------
    async function restoreBackupRAM() {
        const saved = await idb.get("backupRAM");
        if (saved && Module.setBackupRAM(new Uint8Array(saved))) {
            console.log("backup RAM restored");
        }
    }
    async function saveBackupRAM() {
        if (!Module || !running) return;
        await idb.set("backupRAM", Module.getBackupRAM().slice());
    }
    setInterval(saveBackupRAM, 10000);
    addEventListener("pagehide", saveBackupRAM);
    document.addEventListener("visibilitychange", () => {
        if (document.visibilityState === "hidden") saveBackupRAM();
    });

    // ------------------------------------------------------------------
    // Boot
    // ------------------------------------------------------------------
    (async () => {
        try {
            await idb.open();
            Module = await createYmirModule();
            if (!Module.init()) throw new Error("emulator init failed");
            // Debug/test hook (also handy in devtools)
            window.ymir = { Module, isRunning: () => running };
            const savedBios = await idb.get("bios");
            if (savedBios && Module.loadIPL(new Uint8Array(savedBios))) {
                biosReady = true;
            }
            await restoreBackupRAM();
            setStatus(`Ymir ${Module.getVersion()} 就绪。${biosReady ? "BIOS 已恢复，" : ""}请选择 BIOS 与光盘镜像`);
            setButtons();
        } catch (err) {
            console.error(err);
            setStatus(`初始化失败：${err.message ?? err}`);
        }
    })();
})();
