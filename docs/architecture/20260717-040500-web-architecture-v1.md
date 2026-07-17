# Ymir Web 架构文档 v1

- 时间：2026-07-17 04:05 UTC
- 对应迭代：`docs/impl/20260717-040000-iteration-01-core-wasm-and-web-shell.md`

## 1. 分层视图

```
┌─────────────────────────── 浏览器（单线程，无 SharedArrayBuffer）───────────────────────────┐
│                                                                                          │
│  shell/index.html ── shell/main.js（ES2020，零框架零构建）                                │
│    │                     │                                                               │
│    │   Canvas 2D ◄───────┤  rAF 主循环：runFrame → 帧缓冲上屏 → 音频泵送                  │
│    │   WebAudio ◄────────┤  AudioWorklet（ymir-web-audio-worklet.js）                    │
│    │   键盘事件 ─────────►│  按键位图 → setPadState                                       │
│    │   IndexedDB ◄───────┤  BIOS / 备份 RAM 持久化                                       │
│    │                     │                                                               │
│    ▼                     ▼                                                               │
│  ymir-web.js + ymir-web.wasm（Emscripten 6.0.3，-O3，-fexceptions，--bind）              │
│    │                                                                                     │
│    ├─ bridge/ymir_web_bridge.cpp（Embind 桥，~300 行，本工程唯一新 C++）                  │
│    │    回调桥接：视频帧拷贝 / 音频 S16 环形缓冲 / 输入拉模型 / 备份 RAM 读写              │
│    │                                                                                     │
│    └─ ymir-core（上游静态库，66 cpp + vendor: fmt/mio/concurrentqueue/xxHash/libchdr）    │
│         SH-2×2 / SH-1 / M68K / VDP1+2 软件渲染 / SCSP / SMPC / CD Block                  │
└──────────────────────────────────────────────────────────────────────────────────────────┘
```

关键取舍：**不移植官方 SDL3+ImGui 前端**（179 个 cpp，桌面 UI 价值低、依赖重）。Ymir 官方支持 `Ymir_LIBRARY_ONLY` 纯库模式，Web 版 = 核心 + 薄桥 + 新 JS 前端，侵入最小。

## 2. 数据流

### 视频（推模型）
核心 VDP2 帧结束 → `SetSoftwareRenderCallback` 回调（`uint32* fb, w, h`，像素内存序 R,G,B,X）→ 桥内 `std::vector` 拷贝 → JS `getFramebuffer()` 取 `Uint8Array` 视图 → `new ImageData(Uint8ClampedArray)` → `ctx.putImageData`。格式与 Canvas RGBA 天然对齐，零转换。典型分辨率 320×224，最高 704×480。

### 音频（环形缓冲 + 拉取）
核心 SCSP 每采样回调一次（44.1kHz S16 立体声）→ 桥内 65536 帧环形缓冲（满则丢最旧，延迟有界）→ JS 每 rAF `readAudio()` 排空 → `postMessage` 给 AudioWorklet → worklet 内 1 秒 Float32 环形缓冲 → `process()` 供给声卡。两级缓冲吸收 rAF（60Hz）与声卡时钟的抖动。

### 输入（拉模型）
JS 键盘事件维护位图（1=松开 0=按下）→ `setPadState` 存入桥 → 核心 SMPC 采样手柄时回调 `OnPadReport<port>` 读取。采样时机与真实硬件一致，无需前端关心时序。

### 加载
BIOS：JS `Uint8Array` → `LoadIPL(span)`（纯内存）。光盘：JS 把文件写入 MEMFS `/roms/` → `media::LoadDisc(path, preloadToRAM=true)` → `Saturn::LoadDisc` → `AutodetectRegion`。支持 chd/cue+bin/iso/mds/ccd。

### 持久化
备份 RAM：`CreateInMemory(32KiB)` 运行时使用；每 10 秒 + 页面隐藏时 `ReadAll()` → IndexedDB；启动时恢复（注意 `WriteByte` 收总线地址，偏移×2）。BIOS 同样入 IndexedDB 免重复选择。

## 3. 节奏控制（v1 简化模型）

核心不限速。v1 用 rAF 驱动：每 rAF 跑 1 帧（NTSC 59.97Hz ≈ 60Hz 刷新）；当音频积压 > 11025 帧（250ms）时跳过该帧模拟让音频排空，防止延迟累积。PAL 50Hz 会偏快——迭代 02 考虑以音频消费速率为主时钟的精确 pacing。

## 4. 线程模型

**全单线程**。上游渲染线程（VDP1/VDP2/去隔行）在 Emscripten 下默认关闭（补丁 C），SCSP 本就单线程。优点：不需要 SharedArrayBuffer → 不需要 COOP/COEP 响应头 → 任意静态托管（GitHub Pages 等）可部署。代价：多核利用率低；若性能不足，迭代可加 `-pthread` + Worker 方案（届时需要跨源隔离头）。

## 5. 内存模型

wasm32 线性内存：初始 64MB，`ALLOW_MEMORY_GROWTH` 上限 4GB。占用大头：模拟器状态（~20MB）+ MEMFS 镜像 + 预载镜像。cue/bin 双份拷贝（MEMFS 文件 + preload 向量）是已知浪费，推荐 CHD（流式读取、自带压缩）。迭代 02 可用自定义 `IBinaryReader` 消除。

## 6. 构建拓扑

```
segasaturn/
├── ymir/            上游（浅克隆 + 3 处 wasm 移植补丁，见 impl 文档 §2）
├── web/             本工程（CMakeLists 包装 + bridge + shell + test）
├── build-web/       emcmake 产物（ymir-web.js/.wasm + dist/）
└── tools/emsdk/     本地 Emscripten 6.0.3（不污染全局环境）
```

`web/CMakeLists.txt` 以 `add_subdirectory(../ymir)` 引入上游——非顶层自动 `Ymir_LIBRARY_ONLY=ON`，跳过 vcpkg/SDL3/apps。`-DARCHITECTURES=wasm32` 短路上游架构探测，无需补丁。

## 7. 安全与合规

- 不分发 BIOS/游戏 ROM（版权）；用户本地文件经 File API 进入浏览器内存，不出本机
- 无网络请求（构建后纯静态）；无第三方 JS 依赖；无遥测
- 提交前审计：无主机绝对路径写入产物、无密钥、无个人信息（见每次迭代 impl 文档末尾审计节）
