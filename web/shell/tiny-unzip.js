/* Tiny ZIP inflater for Ymir Web (STORE + DEFLATE). */
"use strict";
(function (global) {
  function u16(v, o) { return v[o] | (v[o + 1] << 8); }
  function u32(v, o) {
    return (v[o] | (v[o + 1] << 8) | (v[o + 2] << 16) | (v[o + 3] << 24)) >>> 0;
  }
  function findEOCD(buf) {
    const min = Math.max(0, buf.length - 65557);
    for (let i = buf.length - 22; i >= min; --i) {
      if (u32(buf, i) === 0x06054b50) return i;
    }
    throw new Error("无效的 ZIP（找不到中央目录）");
  }
  async function inflateRaw(data) {
    if (typeof DecompressionStream === "undefined") {
      throw new Error("浏览器不支持解压（需要 DecompressionStream）");
    }
    const ds = new DecompressionStream("deflate-raw");
    const stream = new Blob([data]).stream().pipeThrough(ds);
    const ab = await new Response(stream).arrayBuffer();
    return new Uint8Array(ab);
  }
  function decodeName(bytes) {
    try {
      return new TextDecoder("utf-8", { fatal: true }).decode(bytes);
    } catch (_) {
      let s = "";
      for (let i = 0; i < bytes.length; i++) s += String.fromCharCode(bytes[i]);
      return s;
    }
  }
  async function ymirUnzip(zipBytes) {
    const buf = zipBytes instanceof Uint8Array ? zipBytes : new Uint8Array(zipBytes);
    const eocd = findEOCD(buf);
    const entryCount = u16(buf, eocd + 10);
    let cdOff = u32(buf, eocd + 16);
    if (entryCount === 0xffff || cdOff === 0xffffffff) {
      throw new Error("暂不支持 Zip64 格式");
    }
    const out = Object.create(null);
    let fileCount = 0;
    let pos = cdOff;
    for (let i = 0; i < entryCount; i++) {
      if (u32(buf, pos) !== 0x02014b50) throw new Error("ZIP 中央目录损坏");
      const method = u16(buf, pos + 10);
      const compSize = u32(buf, pos + 20);
      const nameLen = u16(buf, pos + 28);
      const extraLen = u16(buf, pos + 30);
      const commentLen = u16(buf, pos + 32);
      const localOff = u32(buf, pos + 42);
      const nameBytes = buf.subarray(pos + 46, pos + 46 + nameLen);
      const fullName = decodeName(nameBytes);
      pos += 46 + nameLen + extraLen + commentLen;
      if (fullName.endsWith("/")) continue;
      const base = fullName.split(/[/\\\\]/).pop();
      if (!base) continue;
      if (Object.prototype.hasOwnProperty.call(out, base)) {
        throw new Error("ZIP 内存在重复文件名：" + base);
      }
      if (u32(buf, localOff) !== 0x04034b50) throw new Error("ZIP 本地头损坏：" + base);
      const lNameLen = u16(buf, localOff + 26);
      const lExtraLen = u16(buf, localOff + 28);
      const dataStart = localOff + 30 + lNameLen + lExtraLen;
      const compressed = buf.subarray(dataStart, dataStart + compSize);
      let data;
      if (method === 0) {
        data = compressed.slice();
      } else if (method === 8) {
        data = await inflateRaw(compressed);
      } else {
        throw new Error("不支持的 ZIP 压缩方式 " + method + "（文件 " + base + "）");
      }
      out[base] = data;
      fileCount++;
    }
    if (fileCount === 0) throw new Error("ZIP 中没有可解压的文件");
    return out;
  }
  global.ymirUnzip = ymirUnzip;
})(typeof self !== "undefined" ? self : globalThis);
