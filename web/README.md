# Ymir Web

将 [Ymir](https://github.com/StrikerX3/Ymir)（Sega Saturn 模拟器，C++20）编译为 WebAssembly 并在浏览器中运行。

## 架构

```
浏览器页面 (shell/)
├── index.html                 UI：BIOS/镜像选择、画面、状态栏
├── main.js                    主循环 rAF → runFrame → Canvas；键盘输入；IndexedDB 持久化
├── ymir-web-audio-worklet.js  AudioWorklet：S16 立体声环形缓冲 → Float32 输出
└── ymir-web.js / .wasm        Emscripten 产物
      └── bridge/ymir_web_bridge.cpp   Embind 桥接层（本目录唯一新 C++）
            └── ymir-core              上游模拟器核心（ymir/ 子目录，含少量 wasm 移植补丁）
```

模拟核心在浏览器主线程内单线程运行（未使用 pthread/SharedArrayBuffer，页面无需 COOP/COEP 头）。

## 构建

前置：本仓库 `tools/emsdk/` 已安装 Emscripten（`./emsdk install latest`），Ymir 的 vendor 子模块已拉取。

```bash
source tools/emsdk/emsdk_env.sh
emcmake cmake -S web -B build-web -G Ninja -DARCHITECTURES=wasm32 -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --parallel
```

产物：`build-web/ymir-web.js` + `build-web/ymir-web.wasm`。

## 运行

```bash
web/serve.sh 8080        # 组装 build-web/dist 并启动本地服务器
# 打开 http://localhost:8080
```

1. 选择 Saturn BIOS（IPL ROM，512 KiB，需自备，受版权保护不可分发）
2. 选择光盘镜像（推荐 `.chd`；`.cue` 需与所有 `.bin` 一起多选；也支持 `.iso/.mds/.ccd`）
3. 点击"启动"

按键：方向键 = 十字键；`Z X C` = A B C；`A S D` = X Y Z；`Q/W` = L/R；`Enter` = Start。

BIOS 与备份 RAM（游戏存档）自动保存于浏览器 IndexedDB，刷新后自动恢复。

## 测试

```bash
node web/test/smoke.cjs   # 无浏览器冒烟测试：init/跑帧/画面/音频/输入/存档 15 项检查
```

## 已知限制（首版）

- 单线程：VDP 渲染线程关闭；性能取决于浏览器单核速度（本机 node 实测约 4.4 ms/帧，余量充足）
- 镜像经 MEMFS 全量载入内存：cue/bin 会在内存中保留两份拷贝（MEMFS + preload），推荐 CHD
- 帧节奏为 rAF 近似（NTSC 59.97Hz 由 60Hz rAF + 音频积压节流逼近）；无即时存档（savestate）UI
- 多文件镜像（cue+bin）必须同目录一起选中上传
