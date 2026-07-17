# Ymir 核心 API 与 Emscripten 可移植性研究报告

- 时间：2026-07-17 03:18 UTC
- 调查对象：`ymir/`（Ymir 主分支，commit `244d5c84`，2026-07-15）
- 调查方式：两个并行代码侦察（核心嵌入 API / 平台依赖），结论已与仓库源码交叉核对

## 1. 仓库结构速览

```
ymir/
├── libs/ymir-core/      # 模拟器核心静态库（66 个 cpp），唯一需要移植的部分
├── libs/ymir-dbg-commons/
├── apps/ymir-sdl3/      # 官方 SDL3+ImGui 前端（179 个 cpp）——Web 版不移植它
├── apps/ymir-headless/  # 目前只是配置解析 stub，不能作为嵌入参考
├── apps/{ymdasm,ymir-dbg,ymir-sandbox}/
├── vendor/              # fmt / mio / concurrentqueue / xxHash / libchdr / imgui / lz4（git submodule）
├── vcpkg.json           # 仅 apps 层需要（sdl3、curl 等）；core 不依赖 vcpkg
└── libs/ymir-core/docs/mainpage.hpp  # 官方嵌入指南（权威参考）
```

关键事实：`Ymir_LIBRARY_ONLY=ON` 时只构建 `ymir-core`，不走 vcpkg，只用 vendor 内的 5 个依赖（fmt、mio、concurrentqueue、xxHash、libchdr），**全部可 wasm 编译**。

## 2. 核心嵌入 API（`ymir::Saturn` 门面）

源码：`libs/ymir-core/include/ymir/sys/saturn.hpp`

| 能力 | API | Web 友好度 |
|---|---|---|
| 构造/复位 | `Saturn()` / `Reset(bool hard)` / `FactoryReset()` | ✅ |
| 加载 BIOS(IPL) | `LoadIPL(std::span<uint8, 512_KiB>)` — **纯内存** | ✅ 无需文件系统 |
| 加载光盘 | `LoadDisc(media::Disc&&)`；`Disc/Track` 为公开 POD，`Track::binaryReader` 是 `unique_ptr<IBinaryReader>`，可用 `MemoryBinaryReader(std::span)` 手工构建，绕过一切文件路径 | ✅ |
| 跑一帧 | `RunFrame()`（核心**不做限速**，节奏由前端掌握） | ✅ 与 rAF 模型匹配 |
| 视频回调 | `VDP.SetSoftwareRenderCallback(CBSoftwareFrameComplete)`，签名 `void(uint32* fb, uint32 w, uint32 h)`；像素为 `Color888`，内存字节序 R,G,B,X（= Canvas `ImageData` 的 RGBA 可直接用，alpha 恒 0xFF） | ✅ 回调内拷贝到 JS 侧 |
| 音频回调 | `SCSP.SetSampleCallback(CBOutputSample)`，签名 `void(int16 l, int16 r)`，44.1kHz 立体声，**每个采样点回调一次**（每帧约 735~788 次） | ✅ 攒入环形缓冲 → WebAudio |
| 输入 | **拉模型**：`port.SetPeripheralReportCallback(cb)`，核心采样输入时回调前端填 `PeripheralReport`；`Button` 位掩码 1=松开/0=按下；`port.ConnectControlPad()` 插入手柄 | ✅ JS 维护按键位图即可 |
| Savestate | `SaveState(savestate::SaveState&)` / `LoadState(...)`；序列化在前端层用 cereal（vendor 已有）写任意 `std::ostream` → 可导出内存 buffer | ✅ 存 IndexedDB |
| 备份 RAM | `bup::BackupMemory::CreateInMemory(_256Kbit)` + `mem.SetInternalBackupRAM(...)`；导出 `ReadAll() → vector<uint8>`；导入逐字节 `WriteByte` | ✅ 绕开 mmap |
| SMPC 设置持久化 | `SMPC.SetPersistDataCallback(...)` | ✅ 可选 |

回调机制为 C 风格「函数指针 + void* context」（`util::OptionalCallback`），**不能**直接传 JS 闭包，需用静态函数 + 全局实例指针桥接，再经 Embind/EM_ASM 通知 JS。

## 3. 可移植性结论（Emscripten 风险清单）

| # | 问题 | 证据 | 对策 |
|---|---|---|---|
| 1 | `cmake/DetectArchitecture.cmake` 只认 x86_64/arm64，wasm 配置期 `FATAL_ERROR` | 根 `CMakeLists.txt:65-67` | 打补丁：识别 `CMAKE_SYSTEM_NAME=Emscripten` 走 generic 分支 |
| 2 | `loader_chd.cpp:198` 有 try/catch，Emscripten 默认 `-fno-exceptions` 会编译失败 | 该文件 | 编译/链接加 `-fexceptions` |
| 3 | VDP 渲染线程默认开启（`configuration.video.threadedVDP1/2` 默认 true） | `configuration.hpp:100-106` | 方案 A：运行前置 false（单线程、免 SharedArrayBuffer）；方案 B：`-pthread` + COOP/COEP 头。**首版选 A** |
| 4 | mmap（mio）用于备份 RAM 文件与光盘 mmap reader | `backup_ram.hpp`、`binary_reader_mmap.hpp` | 全部绕开：`CreateInMemory` + `MemoryBinaryReader`；头文件会被编译但运行时不触碰 |
| 5 | CHD 支持硬链接 `chdr-static`（含 vendored lzma/zstd/miniz + 内置 flac） | `vendor/libchdr/CMakeLists.txt` | 保留（纯 C，wasm 可编译），CHD 文件写入 MEMFS 后正常加载 |
| 6 | vendor submodule 未随浅克隆 checkout | `vendor/fmt` 等为空目录 | `git submodule update --init --recursive` |
| 7 | OS 特定代码（thread_name/event/process/host_cd） | `util/*.cpp`、`media/host_cd_*` | 均有 wasm 安全 fallback 或为死代码（feature flag 默认关）；无需修改 |
| 8 | SIMD：`vdp_renderer_sw.cpp` 的 SSE/AVX 分支有标量回退 | 该文件 2941+ 行 | wasm32 自动走标量回退；后续可评估 `-msimd128` |
| 9 | 异常外的其他风险（setjmp/dlopen/socket/clock_gettime） | 全仓 grep | **零使用**，无风险 |

## 4. 参考：SDL3 前端如何驱动核心（`apps/ymir-sdl3/src/app/app.cpp`）

- 模拟线程主循环 `App::EmulatorThread()`（app.cpp:3247-3469），核心是事件队列 + `RunFrame()`（:3442）
- 帧限速挂在 `VDP2DrawFinished` 回调（:938-971）；整速靠音频环形缓冲阻塞同步（`audio_system.cpp:76-89`）
- 视频：`SetSoftwareRenderCallback`（:973-997）回调内 `std::copy_n` 到 staging buffer，GUI 线程以 `SDL_PIXELFORMAT_ABGR8888` 上屏
- 输入：`input_service.cpp:341-343` 写按键位；`ReadPeripheral`（:954-958）一行赋值给 report
- IPL 加载：`util::LoadIPLROM`（app.cpp:1733）；光盘：`media::LoadDisc` + `saturn.LoadDisc(std::move)`（app.cpp:3343）

Web 前端将复刻这一结构，但线程模型改为：**浏览器主线程（或单个 Worker）内同步 `RunFrame()`**，节奏由 AudioWorklet/rAF 驱动。

## 5. 总体判断

ymir-core 的可移植性**相当好**：OS 依赖全部隔离在 `util/` 与 `media/host_cd_*`，且都有 wasm 安全 fallback。核心 API 原生支持纯内存数据流（IPL/Disc/备份 RAM/Savestate），与 Web 端「ArrayBuffer 进、Canvas/WebAudio 出」的模型高度契合。**无需 fork 式大改，预计补丁量 < 50 行**，主要工作在新写的 WASM 桥接层与 JS 前端。
