# Ymir Web 化项目工作区

将 [Ymir](https://github.com/StrikerX3/Ymir)（Sega Saturn 模拟器）移植为浏览器可运行的 Web 版本。

## 目录

| 路径 | 内容 |
|---|---|
| `web/` | **本工程**：WASM 桥接层（Embind）、Web 前端、测试、构建/运行说明（详见 `web/README.md`） |
| `ymir/` | 上游 Ymir 源码（含少量 wasm 移植补丁，补丁清单见 impl 文档） |
| `docs/research/` | 环境检测、核心 API 与可移植性研究 |
| `docs/plan/` | 移植计划 |
| `docs/impl/` | 每次迭代的实施记录与审计 |
| `docs/architecture/` | 架构文档 |
| `docs/glossary.md` | 技术词汇表 |
| `tools/emsdk/` | 本地 Emscripten（不污染全局环境，不入库） |
| `build-web/` | 构建产物（不入库） |

## 快速开始

```bash
source tools/emsdk/emsdk_env.sh
emcmake cmake -S web -B build-web -G Ninja -DARCHITECTURES=wasm32 -DCMAKE_BUILD_TYPE=Release
cmake --build build-web --parallel
web/serve.sh 8080        # 打开 http://localhost:8080
```

测试：`node web/test/smoke.cjs`（15 项）、`node web/test/e2e.cjs`（无头浏览器 10 项）。
