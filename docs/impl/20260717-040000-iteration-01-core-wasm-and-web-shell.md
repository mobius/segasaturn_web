# 迭代 01：核心 WASM 化 + 最小 Web 前端（从克隆到浏览器跑通）

- 时间：2026-07-17 03:00–04:05 UTC
- 目标里程碑：M1（环境）→ M2（核心编译）→ M3（桥接冒烟）→ M4（浏览器验证）
- 结果：**全部达成**。wasm 模块编译成功，node 冒烟 15 项全过，无头 Chromium 端到端 10 项全过。

## 1. 时间线

| 时刻 | 动作 | 结果 |
|---|---|---|
| 03:04 | 环境检测（CPU 36 线程 / 125G 内存 / 4×V100 / 无 emcc） | 判定可支持任务；GPU 非关键路径 |
| 03:05 | 浅克隆 Ymir（244d5c84）+ 建 docs 结构 | 23MB |
| 03:07–03:18 | 双路并行代码侦察（核心 API / 可移植性） | 两份 research 报告 |
| 03:25 | plan v1 文档；后台拉 submodule + 装 emsdk 6.0.3 到 `tools/emsdk/` | 均成功（emsdk 约 320MB，全部在工作目录内） |
| 03:30 | 写 `web/CMakeLists.txt` + `web/bridge/ymir_web_bridge.cpp`；`emcmake` 配置 | 配置一次通过（关键：`-DARCHITECTURES=wasm32` 绕过架构检测，**零补丁**） |
| 03:34 | 首次编译 | 失败：6 处 `std::min` + saturn.cpp 19 个级联错误 |
| 03:38 | 补丁 A（binary_reader × 6） | min 错误消除；暴露出 sh2.hpp 真实错误 |
| 03:41 | 补丁 B（sh2.hpp ChunkedMemory/常量 64 位化） | 核心编译通过；saturn.cpp 级联错误自动消失 |
| 03:43 | 桥接层修正（`ymir::sint16` → 全局 `sint16`） | 编译过，链接失败：缺 `sem_timedwait` |
| 03:44 | 加 `web/bridge/sem_stub.c` | **链接成功**：`ymir-web.js` 100KB + `ymir-web.wasm` 3.6MB |
| 03:46 | node 冒烟 | `init()` 抛异常："thread constructor failed" |
| 03:49 | 补丁 C（configuration.hpp：Emscripten 下渲染线程默认关） | init 通过；15 项中 14 项过 |
| 03:52 | 修备份 RAM 总线寻址（`WriteByte(i*2)`） | **冒烟 15/15 全过**，4.4 ms/帧 |
| 03:58 | 写前端三件套 + puppeteer-core e2e | favicon 404 → 内联 favicon 后 **e2e 10/10 全过** |

## 2. 上游补丁清单（ymir/ 目录内，共 3 处，均为 32 位/wasm 移植性修复）

| # | 文件 | 改动 | 原因 |
|---|---|---|---|
| A | `libs/ymir-core/include/ymir/media/binary_reader/binary_reader_{composite,file,mem,mmap,subview,zero}.hpp` | `std::min(size, output.size())` → `std::min(size, static_cast<uintmax_t>(output.size()))`（每文件 1 行） | wasm32 下 `uintmax_t`(u64) 与 `size_t`(u32) 类型冲突，`std::min` 模板推导失败。该 bug 在任何 32 位平台上都会触发，可向 upstream 提 PR |
| B | `libs/ymir-core/include/ymir/hw/sh2/sh2.hpp` | `kAddressSpaceSize/kBreakpointMapSize/kWatchpointMapSize` 类型 size_t→uint64；`ChunkedMemory::kSize` 改 `1ull << sizeBits`，`kNumChunks` 显式 cast | 2^32 常量放不进 32 位 size_t（constexpr 收窄错误）。这三个常量无外部使用，改动安全 |
| C | `libs/ymir-core/include/ymir/core/configuration.hpp` | `threadedVDP1/2/Deinterlacer` 默认值在 `__EMSCRIPTEN__` 下为 false（新增 `kDefaultThreadedRendering` 常量） | 渲染线程在 Saturn **构造期**按默认值创建，单线程 Emscripten 无法 spawn thread，构造即抛 `std::system_error` |

经验教训：首次编译时 saturn.cpp 的 19 个"灵异"错误（`default initialization of const type`）全是 sh2.hpp 常量错误的级联——**修编译错误先修日志里的第一个**。

## 3. 新增文件（全部为 web/ 下自有代码）

```
web/
├── CMakeLists.txt              # 包装工程：add_subdirectory(ymir) + 桥接 target
├── bridge/
│   ├── ymir_web_bridge.cpp     # Embind API（init/loadIPL/loadDisc/runFrame/帧缓冲/音频/输入/备份RAM）
│   └── sem_stub.c              # sem_timedwait stub（见"问题 4"）
├── shell/
│   ├── index.html              # UI（BIOS/镜像选择、Canvas、按键说明）
│   ├── main.js                 # rAF 主循环、音频泵送、键盘映射、IndexedDB 持久化
│   └── ymir-web-audio-worklet.js  # AudioWorklet 环形缓冲
├── test/
│   ├── smoke.cjs               # node 冒烟（15 项）
│   └── e2e.cjs                 # 无头 Chromium 端到端（10 项，含截图）
├── serve.sh                    # 组装 build-web/dist 并起本地服务器
└── README.md
```

## 4. 问题与解决（详录）

1. **`std::min` 类型冲突（补丁 A）**：LP64 Linux 上 `size_t==uintmax_t` 编译不出错，掩盖了 bug；wasm32 立即暴露。教训：32 位交叉编译是免费的类型检查器。
2. **constexpr 收窄（补丁 B）**：`1ull<<32` 赋给 32 位 `size_t` 不是常量表达式。CHD/断点映射等"大地址空间"常量在 wasm 都要警惕。
3. **架构检测**：`DetectArchitecture.cmake` 只认 x86_64/arm64，但 `-DARCHITECTURES=wasm32` 经缓存变量短路其 `if (NOT DEFINED)` 守卫，**避免了对上游构建系统的任何改动**。
4. **`sem_timedwait` 未定义**：moodycamel `BlockingConcurrentQueue`（SCSP/VDP 事件队列）的信号量路径引用它；单线程 Emscripten libc 不提供。因这些队列的阻塞等待只在可选 worker 线程里用（本构建全部禁用），stub 返回 `ETIMEDOUT` 保活性安全。
5. **构造期线程（补丁 C）**：`threadedVDP*` 默认 true 且线程在 `Saturn()` 构造时创建——前端没有机会先改配置。教训：库的"运行时配置"若影响构造期行为，必须用条件默认值。
6. **备份 RAM 寻址**：`BackupMemory::WriteByte` 收**总线地址**（内部 `>>1`），`ReadAll` 返回线性数据。桥接层导入镜像必须 `WriteByte(i*2, v)`。冒烟测试抓住了这个 off-by-2。
7. **favicon 404**：浏览器自动请求 `/favicon.ico` 会计入 console error，内联 `<link rel="icon" href="data:,">` 解决。

## 5. 验证结果

- **node 冒烟**（`web/test/smoke.cjs`）：15/15。含 init、版本、IPL 尺寸校验、垃圾镜像优雅失败、30 帧执行（4.4ms/帧，约 3.8 倍速余量）、帧回调 320×224、音频 22049 立体声帧、输入注入、备份 RAM 往返。
- **浏览器 e2e**（`web/test/e2e.cjs`，无头 Chromium）：10/10。页面加载、wasm 启动、UI 上传 BIOS、浏览器内跑 20 帧、帧缓冲尺寸、音频产出、Canvas 写像素、全程零 console 错误。截图：`build-web/e2e-screenshot.png`。

## 5.1 可复现性终验（2026-07-17 04:15 UTC）

删除 `build-web/` 后按 README 文档步骤全量重建（`emcmake` 配置 + 编译，36 并行），随后复跑两套测试：

- `node web/test/smoke.cjs` → **ALL CHECKS PASSED**（15/15）
- `node web/test/e2e.cjs` → **E2E ALL CHECKS PASSED**（10/10）

文档记录的构建/测试路径与实际行为一致，交付可复现。

## 6. 下一步（迭代 02 候选）

- 真实游戏验证（需用户自备 BIOS/镜像，人工或补充自动化）
- 音频驱动的精确帧 pacing（当前 rAF 近似 + 积压节流）
- savestate（需 cereal + 移植 app 层 serdes 头）
- 内存优化：cue/bin 双份拷贝（MEMFS + preloadToRAM）→ 自定义 IBinaryReader
- Gamepad API、性能 profiling、`-msimd128` 评估
