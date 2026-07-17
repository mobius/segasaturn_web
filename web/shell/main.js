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
            Module.runFrame();
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

    const DISC_MAIN_EXTS = [".chd", ".cue", ".mds", ".ccd", ".iso"];
    async function loadDiscFiles(files) {
        Module.FS.mkdirTree("/roms");
        let mainFile = null;
        for (const f of files) {
            const data = new Uint8Array(await f.arrayBuffer());
            Module.FS.writeFile(`/roms/${f.name}`, data);
            const ext = f.name.slice(f.name.lastIndexOf(".")).toLowerCase();
            if (DISC_MAIN_EXTS.includes(ext) && !mainFile) mainFile = f.name;
        }
        if (!mainFile) {
            setStatus("未找到主镜像文件（.chd/.cue/.iso 等）");
            return;
        }
        setStatus(`正在解析 ${mainFile} …`);
        // Let the status paint before the (blocking) parse
        await new Promise((r) => setTimeout(r, 30));
        if (!Module.loadDisc(`/roms/${mainFile}`)) {
            setStatus(`光盘解析失败：${mainFile}（多文件镜像请把 .cue 与所有 .bin 一起选中）`);
            return;
        }
        discReady = true;
        setStatus("光盘已加载，可以启动");
        setButtons();
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
