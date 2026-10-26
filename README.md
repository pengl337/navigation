# P1 Linux 导航项目

**组名：Linux 端导航应用开发组（Linux PAL 适配）。**

5 人使用 C++、Qt Widgets / OpenGL ES 和 CMake 开发 Linux 导航应用，集成 A*/HMM，完成渲染、语音、存储、定位四项 PAL 适配，并与 G6 联调。当前完成的是规划与规则，尚无可运行工程。

## 统一开发环境

五人统一在 **Linux 桌面虚拟机**中开发和验证：Ubuntu Desktop **24.04 LTS（noble，amd64）**、Qt **6.4.2**、GCC/G++ **13.3.0**、CMake **3.28.3**（C++17）。安装、版本核对与复现步骤见 [统一开发环境](docs/development-environment.md)。版本已约定，实际安装与运行尚未验证。

## 团队分工

| 组员 | 主要职责 |
| --- | --- |
| 1 | Qt 框架/公共 UI、CMake 与发布、用户页面、LinuxVoiceAdapter（TTS/ASR/FFI） |
| 2 | 导入、StorageAdapter（SQLite/文件）、离线包、用户本地数据 |
| 3 | C++ A*/HMM 集成、异步任务、算法调用页与性能监控 |
| 4 | LocationAdapter（GeoClue2/权限）、导航/偏航/模拟回放、G6 联调报告 |
| 5 | LinuxRenderer（Qt Widget/OpenGL ES）、地图交互与仪表盘 |

## 提示词框架

新对话可以使用 [prompts/README.md](prompts/README.md)：按“通用开场 + 本人职责 + 本次流程”组合，覆盖五人模块开发、算法方案对比、跨组联调、排错与验收。

例如：`我是 P1 Linux 组员 2，请使用 prompts/start.md、members.md 的 2 号职责和 workflows.md 的模块开发流程，处理本次数据导入任务。`

## 开始工作

1. 阅读 [当前任务书摘录](docs/linux-assignment-reference.md) 与 [project.md](project.md)，确认环境、职责、验收和跨组接口。
2. 阅读 [AGENTS.md](AGENTS.md)，向 AI 说明组员编号和具体任务；查看 [AI 历史日志](ai_log.md) 与 [方案抉择记录](decision_log.md)。
3. 各模块先使用公共契约和固定样例独立开发，完成后联调；算法方案试验与决策按记录模板保留证据。

推荐开场：`我是 P1 Linux 组员 3。请阅读项目规划、协作规则和方案抉择记录，再处理 C++ 算法集成任务。`

支持 AGENTS.md 的工具应读取根目录规则；其他工具请手动附加相关文件。环境与构建步骤待工程创建后实测补充。最终产物为 Linux 可执行程序及完整 CMake 工程，附运行截图、3–5 分钟 Linux 视频与 G6 联调报告。

旧平台任务摘录仅供历史参考，当前 P1 范围以新 PDF 为准。
