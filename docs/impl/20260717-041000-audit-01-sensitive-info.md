# 敏感信息审计记录 01

- 时间：2026-07-17 04:10 UTC
- 范围：工作区自有内容（`docs/`、`web/`、根 `.gitignore`）。`ymir/`（上游开源代码）与 `tools/`、`build-web/`（本地工具链与产物，不入库）不在审计范围。
- 结论：**通过，无泄露**。

## 检查项与结果

| # | 检查项 | 方法 | 结果 |
|---|---|---|---|
| 1 | 密码/密钥/令牌/私钥 | `grep -iE "password|secret|token|api_?key|private_key|ssh-rsa|BEGIN.*PRIVATE KEY"` | 无命中 |
| 2 | 个人路径/主机名/用户名 | `grep -E "/home/<用户>|/Users/|HOSTNAME"` | 无命中 |
| 3 | 内网/公网 IP | IPv4 正则，排除 `127.0.0.1/0.0.0.0/localhost`（测试代码本机回环，合理） | 无命中 |
| 4 | 邮箱地址 | 邮箱正则 | 无命中 |
| 5 | 版权 ROM/BIOS/镜像文件 | `find` 查 `*.bin/*.cue/*.iso/*.chd/*.rom` 等 | 无（e2e 生成的全零 dummy BIOS 已自动删除） |
| 6 | 大体积产物/依赖误入库 | 审查目录树 | 已在 `.gitignore` 排除 `build-web/`、`tools/`、`node_modules/` |

## 已识别的可接受项（留档说明）

- 文档中出现工作区路径 `/mnt/storage/workspace/k3_test/segasaturn`：为项目自身路径，不含用户名/主机名/内网地址，属复现所需的正常记录，保留。
- `web/test/package-lock.json` 含 npm registry 公开 URL：公共信息，无风险。
- `web/shell/main.js`、`web/test/*.cjs` 使用 `127.0.0.1`/localhost：本地测试语义所需。

## 预防措施（已落地）

- `.gitignore` 预排除：构建产物、emsdk、node_modules、e2e 临时 dummy BIOS、以及一切 ROM/存档类扩展名（防止用户日后误把自己的游戏文件提交）。
- 项目不分发任何 BIOS/ROM；UI 与 README 均注明"需用户自备合法文件"。
- 前端零第三方 JS 依赖、零网络请求、零遥测。

## 后续每次迭代的要求

每次提交前重跑本审计第 1–5 项的 grep 命令（见上文），结果附在当次 impl 文档末尾。
