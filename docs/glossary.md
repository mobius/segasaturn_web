# 技术词汇表（glossary）

> 本项目交互过程中出现的新技术词汇，按主题分组解释。随迭代持续补充。
> 最近更新：2026-07-17（迭代 01 完成后）

## 32 位移植（迭代 01 实战踩坑）

- **LP64 / ILP32 数据模型**：64 位 Linux 是 LP64（`long` 和指针 64 位），`size_t` 为 64 位；wasm32 是 32 位模型，`size_t` 仅 32 位。同一份 C++ 代码在两种模型下类型关系不同，是移植 bug 的温床。
- **`std::min` 模板推导冲突**：`std::min(a, b)` 要求 a、b 类型完全一致（模板参数 T 只能推导出一个）。LP64 下 `size_t` 恰好等于 `uintmax_t` 不报错；wasm32 下一个是 32 位一个是 64 位，直接编译失败。修法：`static_cast` 显式统一类型。
- **constexpr 收窄（narrowing）**：常量表达式中不允许"值会改变"的隐式转换。`1ull << 32`（值 2^32）赋给 32 位 `size_t` 放不下，constexpr 直接报错。普通运行时代码则会被静默截断——所以 32 位目标能额外暴露这类隐患。
- **级联编译错误**：一个头文件里的错误会让包含它的所有 .cpp 报出大量莫名其妙的错误（本次 sh2.hpp 的 constexpr 错误导致 saturn.cpp 报 19 个"灵异"错误）。原则：**永远先修日志里的第一个错误**。

## 系统编程

- **POSIX 信号量（semaphore）**：`sem_init/sem_wait/sem_post/sem_timedwait` 一族计数同步原语，常用于有界队列的阻塞等待。Emscripten 单线程 libc 不提供 `sem_timedwait`（它在 pthread 构建里才有），本项目用 stub（存根函数）补符号：直接返回 `ETIMEDOUT`，调用方按"暂无事件"处理。
- **stub（存根）**：为满足链接器而提供的空实现/假实现函数，保证"用不到的代码路径"也能链接通过。
- **总线地址 vs 线性偏移**：硬件芯片挂在总线上时，软件看到的地址不一定等于存储内部的第几个字节。Saturn 备份 RAM 每个数据字节占一个偶数总线地址，所以"镜像第 i 字节"要写到总线地址 `i*2`——本项目的备份 RAM 导入 bug 就出在这里。

## Web 平台补充

- **typed_memory_view**：Embind 的零拷贝视图机制，让 JS 直接"看到"WASM 线性内存里的一段数据（如帧缓冲），不需要复制。视图只在底层内存不被移动时有效，所以每帧重新获取。
- **Puppeteer / puppeteer-core**：用代码驱动 Chromium 的 Node 库（点击、上传文件、截图、在页面里执行 JS）。`puppeteer-core` 不自带浏览器下载，可复用系统已装的 Chromium，适合做端到端测试。
- **无头浏览器（headless browser）**：没有窗口界面的浏览器模式，可在服务器/CI 里跑真实网页并做断言。本项目用它验证"页面加载→WASM 启动→上传 BIOS→跑帧→画面/音频产出"全链路。
- **MODULARIZE / EXPORT_NAME**：Emscripten 输出选项。MODULARIZE 把胶水 JS 包成工厂函数（`createYmirModule()`）而非全局脚本，便于多实例和模块化加载。
- **ALLOW_MEMORY_GROWTH**：允许 WASM 线性内存按需增长（配合 `MAXIMUM_MEMORY` 上限）。光盘镜像体积不可预知，必须开启。
- **favicon.ico 404**：浏览器会自动请求 `/favicon.ico`，缺失会产生 404 并计入 console 错误。用 `<link rel="icon" href="data:,">` 声明空图标即可消除。

## 前次词汇（研究阶段，保留）

（Web 化/工具链、Saturn 硬件、模拟器架构等词汇见下文各节，未改动。）

## Web 化 / 编译工具链

- **WebAssembly (WASM)**：一种浏览器可执行的二进制指令格式，接近原生性能。把 C/C++ 编译成 `.wasm` 后，网页里的 JavaScript 可以加载并调用它。本项目"Web 版"的本质就是把模拟器核心编译成 WASM 跑在浏览器里。
- **Emscripten**：把 C/C++ 编译为 WebAssembly 的完整工具链，核心命令是 `emcc`（替代 gcc/clang 的编译器前端）。它还提供 POSIX API 的 JS 实现（文件系统、线程等），让原生代码尽量不改就能跑在网页里。
- **emsdk**：Emscripten 的官方安装/版本管理器（一个 git 仓库 + Python 脚本）。本项目将它克隆在 `tools/emsdk/` 目录内使用，所有文件落在工作目录中，不污染系统全局环境。
- **Embind**：Emscripten 自带的 C++ ↔ JavaScript 绑定机制。在 C++ 里用宏声明哪些类/函数要暴露，编译后 JS 就能直接 `Module.xxx()` 调用，自动处理类型转换。
- **EM_ASM**：Emscripten 提供的宏，允许在 C++ 代码里内嵌一段 JavaScript 代码，用于从 WASM 侧直接回调 JS（例如把画面帧交给 Canvas）。
- **MEMFS**：Emscripten 在内存中模拟的文件系统。C++ 代码里的 `fopen`/`ifstream` 读写的是浏览器内存里的虚拟文件，不触碰真实磁盘。光盘镜像等大文件写入 MEMFS 后，原有的文件解析代码可原样工作。
- **SharedArrayBuffer (SAB)**：JS 中可被多个线程（主线程 + Worker）共享的内存，是 WASM 多线程（pthreads）的基础。浏览器出于安全（防 Spectre 攻击）要求页面必须带特定 HTTP 响应头才能用。
- **COOP / COEP**：两个 HTTP 响应头（`Cross-Origin-Opener-Policy: same-origin` 与 `Cross-Origin-Embedder-Policy: require-corp`），同时设置后页面进入"跨源隔离"状态，才可使用 SharedArrayBuffer。本项目首版走单线程方案，暂不需要。
- **AudioWorklet**：浏览器 WebAudio API 的现代音频处理接口，在独立音频线程中运行 JS 代码，适合低延迟地持续供给模拟器产生的 PCM 采样数据（替代已废弃的 ScriptProcessorNode）。
- **requestAnimationFrame (rAF)**：浏览器提供的每帧回调（通常 60Hz），与屏幕刷新同步。Web 版用它驱动"跑一帧模拟 → 绘制画面"的主循环。
- **IndexedDB**：浏览器内置的 NoSQL 数据库，可持久存储大体积二进制数据。Web 版用它保存游戏存档（备份 RAM）与即时存档（savestate）。

## 交叉编译 / 构建系统

- **vcpkg**：微软的 C/C++ 包管理器，Ymir 官方用它来装 SDL3 等桌面端依赖。Web 版只编译核心库，绕开 vcpkg。
- **git submodule**：git 的子仓库机制，Ymir 把 fmt、libchdr 等依赖以 submodule 形式放在 `vendor/`。浅克隆不会自动拉取，需 `git submodule update --init --recursive`。
- **CMake toolchain file**：告诉 CMake"目标平台是什么、用什么编译器"的配置文件。Emscripten 自带 `Emscripten.cmake`，用 `emcmake cmake ...` 命令即可注入。
- **IPO / LTO**：跨过程优化 / 链接期优化，把多个编译单元在链接时整体优化以提升性能。Emscripten 支持，但若出问题可 `-DYmir_ENABLE_IPO=OFF` 关闭。
- **`-fexceptions`**：开启 C++ 异常支持的编译选项。Emscripten 默认关闭异常（减小体积），但 Ymir 的 CHD 加载器里有 try/catch，必须显式打开。

## Sega Saturn 硬件 / 模拟器术语

- **IPL (Initial Program Loader)**：Saturn 的 BIOS ROM（512 KiB），开机后首先运行的固件程序。运行游戏必须提供。BIOS 有版权，**不能**随项目分发，需用户自备。
- **SH-2 (Master/Slave)**：日立 SuperH 系列 CPU，Saturn 搭载两颗 SH-2（主/从）作主处理器。模拟器中 `StepMasterSH2/StepSlaveSH2` 即单步执行它们。
- **SH-1**：第三颗 SuperH 芯片，专管 CD 光驱（CD Block）。
- **VDP1 / VDP2 (Video Display Processor)**：Saturn 的两颗图形芯片。VDP1 负责画精灵/多边形，VDP2 负责背景层与合成输出。模拟器里的"软件渲染器"就是用 CPU 代码逐像素复刻这两颗芯片的行为。
- **SCSP (Saturn Custom Sound Processor)**：土星的声音芯片（含一颗 MC68EC000 CPU），产生游戏的音乐和音效。模拟器每产生一个音频采样就回调前端一次。
- **SMPC (System Manager & Peripheral Control)**：系统管理与外设控制芯片，管手柄端口、RTC 实时时钟、复位按钮等。Web 版的按键输入经 SMPC 的手柄端口注入。
- **M68K**：摩托罗拉 68000 系列 CPU，此处指 SCSP 里的 MC68EC000（音频驱动用）。
- **HLE / LLE**：高层模拟（用代码模拟芯片的"行为结果"）与低层模拟（逐周期复刻芯片内部电路）。LLE 更精确但更慢。
- **CHD (Compressed Hunks of Data)**：MAME 项目发明的光盘镜像压缩格式，把 CD 数据分块用 lzma/zstd/flac 等压缩。Ymir 用 libchdr 库读取，该库是纯 C，可直接编译进 WASM。
- **cue/bin、iso、mdf/mds、img/ccd/sub**：常见 CD 光盘镜像格式。cue 是描述轨道布局的文本文件，bin 是原始数据。Saturn 游戏几乎都是 CD 镜像。
- **备份 RAM (Backup RAM / BUP)**：Saturn 机身内置 32 KiB 存档内存（也可插扩展卡），游戏进度存在这里。Web 版用纯内存镜像 + IndexedDB 持久化。
- **Savestate（即时存档）**：把模拟器整机的当前状态（所有芯片寄存器、内存内容）序列化保存，可随时"时间回溯"。Ymir 用 cereal 库做序列化。
- **cereal**：C++ 序列化库，把结构体转成二进制/JSON 流。Ymir 的即时存档靠它，可输出到内存流再交给 IndexedDB。

## 模拟器架构

- **前端 / 核心分离**：模拟器项目常见架构——`ymir-core` 是纯模拟逻辑（不依赖任何窗口/图形系统），`ymir-sdl3` 是桌面前端。Web 版就是再写一个新前端（WASM 桥接层 + JS），核心原样复用。
- **拉模型输入 (pull-model input)**：核心在需要读手柄时主动回调前端索取按键状态（而非前端主动推送给核心）。好处是输入采样时机与真实硬件完全一致。
- **音频同步限速**：模拟器跑多快由"音频缓冲消耗速度"自然限制——缓冲满了就阻塞模拟线程，从而实现与真实硬件同速。Web 版改由 AudioWorklet/rAF 节奏实现。
- **软件渲染器 (software renderer)**：用 CPU 逐像素计算画面（不用 GPU）。Ymir 目前只有软件渲染器，恰好与 WASM 环境匹配（WASM 里难以直接用 GPU）。
