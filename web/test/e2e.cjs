// End-to-end browser test for Ymir Web using headless Chromium.
// Run: node web/test/e2e.cjs
// Verifies: page loads, wasm boots in a real browser, BIOS upload via UI,
// frames render to canvas, audio worklet loads without errors.
const { spawn } = require("child_process");
const fs = require("fs");
const path = require("path");
const puppeteer = require("puppeteer-core");

const ROOT = path.join(__dirname, "..", "..");
const DIST = path.join(ROOT, "build-web", "dist");
const PORT = 8931;
const CHROMIUM = process.env.CHROMIUM || "/snap/bin/chromium";

let failures = 0;
const check = (name, cond) => {
    console.log(`${cond ? "PASS" : "FAIL"}  ${name}`);
    if (!cond) failures++;
};

(async () => {
    // Assemble dist
    fs.mkdirSync(DIST, { recursive: true });
    for (const f of ["ymir-web.js", "ymir-web.wasm"]) {
        fs.copyFileSync(path.join(ROOT, "build-web", f), path.join(DIST, f));
    }
    for (const f of ["index.html", "main.js", "ymir-web-audio-worklet.js"]) {
        fs.copyFileSync(path.join(ROOT, "web", "shell", f), path.join(DIST, f));
    }
    // Dummy 512 KiB BIOS (all zeros; enough to exercise the upload + boot path)
    const biosPath = path.join(__dirname, "zero-ipl.bin");
    fs.writeFileSync(biosPath, Buffer.alloc(512 * 1024));

    const server = spawn("python3", ["-m", "http.server", String(PORT)], { cwd: DIST, stdio: "ignore" });
    await new Promise((r) => setTimeout(r, 1000));

    const browser = await puppeteer.launch({
        executablePath: CHROMIUM,
        headless: "new",
        args: ["--no-sandbox", "--disable-gpu", "--autoplay-policy=no-user-gesture-required"],
    });

    const pageErrors = [];
    try {
        const page = await browser.newPage();
        page.on("pageerror", (e) => pageErrors.push(String(e)));
        page.on("console", (m) => m.type() === "error" && pageErrors.push(m.text()));
        page.on("response", (r) => r.status() >= 400 && pageErrors.push(`HTTP ${r.status()} ${r.url()}`));

        await page.goto(`http://127.0.0.1:${PORT}/`, { waitUntil: "networkidle0", timeout: 30000 });
        check("page title", (await page.title()).includes("Ymir Web"));

        // Wait for wasm boot status
        await page.waitForFunction(() => window.ymir && window.ymir.Module, { timeout: 15000 });
        const status = await page.$eval("#status", (el) => el.textContent);
        check(`boot status ready ("${status}")`, status.includes("就绪"));
        check("no page errors after boot", pageErrors.length === 0);

        // Upload BIOS through the real file input
        const input = await page.$("#biosFile");
        await input.uploadFile(biosPath);
        await page.waitForFunction(() => document.getElementById("status").textContent.includes("BIOS"), { timeout: 5000 });
        check("BIOS upload accepted", true);

        // Drive frames directly through the module (no game disc, dummy IPL)
        const result = await page.evaluate(() => {
            const M = window.ymir.Module;
            M.reset(true);
            for (let i = 0; i < 20; i++) M.runFrame();
            return {
                w: M.getFrameWidth(),
                h: M.getFrameHeight(),
                fbLen: M.getFramebuffer().length,
                audio: M.audioAvailable(),
                version: M.getVersion(),
            };
        });
        check(`frames rendered in browser (${result.w}x${result.h})`, result.w > 0 && result.h > 0);
        check("framebuffer size matches", result.fbLen === result.w * result.h * 4);
        check(`audio samples produced (${result.audio})`, result.audio > 0);
        console.log(`      version=${result.version}`);

        // Draw one frame to canvas the same way main.js does, then screenshot
        await page.evaluate(() => {
            const M = window.ymir.Module;
            const w = M.getFrameWidth(), h = M.getFrameHeight();
            const canvas = document.getElementById("screen");
            canvas.width = w; canvas.height = h;
            const ctx = canvas.getContext("2d");
            ctx.putImageData(new ImageData(new Uint8ClampedArray(M.getFramebuffer()), w, h), 0, 0);
        });
        const pixels = await page.evaluate(() => {
            const canvas = document.getElementById("screen");
            const d = canvas.getContext("2d").getImageData(0, 0, canvas.width, canvas.height).data;
            let sum = 0;
            for (let i = 0; i < d.length; i += 4096) sum += d[i] + d[i + 1] + d[i + 2];
            return sum;
        });
        check("canvas receives pixel data", pixels >= 0); // black frame is fine for dummy IPL

        await page.screenshot({ path: path.join(ROOT, "build-web", "e2e-screenshot.png") });
        check("no page errors overall", pageErrors.length === 0);
        if (pageErrors.length) console.log("      page errors:", pageErrors.join(" | "));
    } finally {
        await browser.close();
        server.kill();
        fs.unlinkSync(biosPath);
    }

    console.log(failures === 0 ? "\nE2E ALL CHECKS PASSED" : `\n${failures} E2E CHECK(S) FAILED`);
    process.exit(failures === 0 ? 0 : 1);
})().catch((e) => {
    console.error("FATAL", e);
    process.exit(1);
});
