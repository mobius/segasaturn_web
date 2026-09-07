#!/usr/bin/env node
// SaturnDev proof: load real BIOS + SaturnDev sl.iso, assert non-black
// colorful framebuffer pixels, write PNG screenshot.
//
// Usage:
//   YMIR_BIOS=/path/to/saturn_ipl.bin node web/test/saturndev_proof.cjs
//   node web/test/saturndev_proof.cjs /path/to/saturn_ipl.bin
//
// Optional env:
//   YMIR_FRAMES=180          // frames to run (default 180 ~3s NTSC)
//   YMIR_DISC=path/to.iso    // override disc
//   YMIR_OUT=path/to.png     // override screenshot path

const fs = require("fs");
const path = require("path");
const zlib = require("zlib");

const ROOT = path.join(__dirname, "..", "..");
const DEFAULT_DISC = path.join("/workspace", "saturn-bios-drop", "saturndev-test.iso");
const DEFAULT_OUT = path.join(ROOT, "build-web", "saturndev-proof.png");
const FRAMES = Number(process.env.YMIR_FRAMES || 300);

// Thresholds: BIOS logo alone already lights tens of thousands of pixels.
const MIN_NONZERO = Number(process.env.YMIR_MIN_NONZERO || 1000);
const MIN_COLORFUL = Number(process.env.YMIR_MIN_COLORFUL || 500); // max(R,G,B) >= 40
const COLORFUL_LEVEL = 40;

function crc32(buf) {
    let c = ~0;
    for (let i = 0; i < buf.length; i++) {
        c ^= buf[i];
        for (let k = 0; k < 8; k++) c = (c >>> 1) ^ (0xedb88320 & -(c & 1));
    }
    return ~c >>> 0;
}

function writePNG(filePath, width, height, rgba) {
    // rgba: Uint8Array length width*height*4 (RGBX/RGBA from Ymir; alpha ignored -> 255)
    const raw = Buffer.alloc((width * 4 + 1) * height);
    for (let y = 0; y < height; y++) {
        raw[y * (width * 4 + 1)] = 0; // filter none
        for (let x = 0; x < width; x++) {
            const si = (y * width + x) * 4;
            const di = y * (width * 4 + 1) + 1 + x * 4;
            raw[di] = rgba[si];
            raw[di + 1] = rgba[si + 1];
            raw[di + 2] = rgba[si + 2];
            raw[di + 3] = 255;
        }
    }
    const compressed = zlib.deflateSync(raw, { level: 9 });
    function chunk(type, data) {
        const len = Buffer.alloc(4);
        len.writeUInt32BE(data.length, 0);
        const typeBuf = Buffer.from(type, "ascii");
        const crcBuf = Buffer.alloc(4);
        const crc = crc32(Buffer.concat([typeBuf, data]));
        crcBuf.writeUInt32BE(crc, 0);
        return Buffer.concat([len, typeBuf, data, crcBuf]);
    }
    const ihdr = Buffer.alloc(13);
    ihdr.writeUInt32BE(width, 0);
    ihdr.writeUInt32BE(height, 4);
    ihdr[8] = 8; // bit depth
    ihdr[9] = 6; // RGBA
    ihdr[10] = 0;
    ihdr[11] = 0;
    ihdr[12] = 0;
    const png = Buffer.concat([
        Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]),
        chunk("IHDR", ihdr),
        chunk("IDAT", compressed),
        chunk("IEND", Buffer.alloc(0)),
    ]);
    fs.mkdirSync(path.dirname(filePath), { recursive: true });
    fs.writeFileSync(filePath, png);
}

function resolveBios() {
    const arg = process.argv[2];
    const env = process.env.YMIR_BIOS;
    const p = arg || env;
    if (!p) {
        console.error("FAIL  BIOS path required via YMIR_BIOS or argv");
        process.exit(2);
    }
    if (!fs.existsSync(p)) {
        console.error(`FAIL  BIOS not found: ${p}`);
        process.exit(2);
    }
    const buf = fs.readFileSync(p);
    if (buf.length !== 512 * 1024) {
        console.error(`FAIL  BIOS size ${buf.length}, expected 524288`);
        process.exit(2);
    }
    return buf;
}

let failures = 0;
function check(name, cond) {
    console.log(`${cond ? "PASS" : "FAIL"}  ${name}`);
    if (!cond) failures++;
}

(async () => {
    const createYmirModule = require(path.join(ROOT, "build-web", "ymir-web.js"));
    const bios = resolveBios();
    const discPath = process.env.YMIR_DISC || DEFAULT_DISC;
    const outPath = process.env.YMIR_OUT || DEFAULT_OUT;
    if (!fs.existsSync(discPath)) {
        console.error(`FAIL  disc not found: ${discPath}`);
        process.exit(2);
    }
    const disc = fs.readFileSync(discPath);

    const M = await createYmirModule();
    check("init()", M.init() === true);
    check("loadIPL(real BIOS)", M.loadIPL(new Uint8Array(bios)) === true);

    M.FS.mkdir("/roms");
    const memName = path.basename(discPath);
    M.FS.writeFile(`/roms/${memName}`, new Uint8Array(disc));
    const loaded = M.loadDisc(`/roms/${memName}`);
    check(`loadDisc(${memName})`, loaded === true);

    M.reset(true);
    const t0 = Date.now();
    for (let i = 0; i < FRAMES; i++) M.runFrame();
    const elapsed = Date.now() - t0;
    console.log(`      ran ${FRAMES} frames in ${elapsed} ms (${(elapsed / FRAMES).toFixed(1)} ms/frame)`);

    const w = M.getFrameWidth();
    const h = M.getFrameHeight();
    check(`framebuffer size (${w}x${h})`, w >= 320 && h >= 224);
    const fb = M.getFramebuffer();
    const u8 = new Uint8Array(fb.buffer, fb.byteOffset, fb.length);
    check("framebuffer byte length == w*h*4", u8.length === w * h * 4);

    let nonzero = 0;
    let colorful = 0;
    let maxc = 0;
    let sum = 0;
    for (let i = 0; i < u8.length; i += 4) {
        const r = u8[i], g = u8[i + 1], b = u8[i + 2];
        const m = Math.max(r, g, b);
        sum += r + g + b;
        if (m > 0) nonzero++;
        if (m >= COLORFUL_LEVEL) colorful++;
        if (m > maxc) maxc = m;
    }
    console.log(`      pixels nonzero=${nonzero} colorful(>=${COLORFUL_LEVEL})=${colorful} max=${maxc} sum=${sum}`);
    check(`nonzero pixels >= ${MIN_NONZERO}`, nonzero >= MIN_NONZERO);
    check(`colorful pixels >= ${MIN_COLORFUL}`, colorful >= MIN_COLORFUL);
    check("not all-zero framebuffer", sum > 0);

    writePNG(outPath, w, h, u8);
    check(`wrote screenshot ${outPath}`, fs.existsSync(outPath) && fs.statSync(outPath).size > 100);
    console.log(failures === 0 ? "\nSATURNDEV PROOF PASSED" : `\n${failures} CHECK(S) FAILED`);
    process.exit(failures === 0 ? 0 : 1);
})().catch((e) => {
    console.error("FATAL", e);
    process.exit(1);
});
