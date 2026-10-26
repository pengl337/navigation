# 第一次作业应用框架验证

日期：2026-10-26；负责人：1。关联日志：`20261026-162617-member1-framework-05`、`20261026-174106-member1-native-ui-07`。

## 实际环境与结果

使用用户授权的独立基础快照克隆 `P1-Assignment1-Verify-20261026`：Ubuntu 24.04 noble x86_64、Qt 6.4.2、GCC/G++ 13.3.0、CMake 3.28.3；1 vCPU、8 GiB RAM、3D 加速关闭。详细限制见 [环境记录](../../environment/member-1-assignment1-2026-10-26.md)。

| 检查 | 实际结果 |
| --- | --- |
| `bash tools/verify-environment.sh` | 版本/架构全部通过 |
| CMake Debug / Ninja / `cmake --build build -j1` | 配置、编译、链接通过，生成应用与测试程序 |
| `ctest --test-dir build --output-on-failure` | offscreen 平台 1/1 测试目标通过；内部 6 项用例加初始化/收尾共 8 通过、0 失败 |
| `QT_QPA_PLATFORM=wayland ./build/p1-ui-tests`（带已登录会话环境） | 真实 Wayland 平台同一套用例 8 通过、0 失败 |
| 构建目录 offscreen `--smoke` | `P1 shell startup: PASS` |
| `--capture-dir` | 4 张真实 Qt 控件截图生成，人工视觉检查通过 |
| `cmake --install build --prefix "$PWD/out/app"` | 安装通过；安装目录内程序 offscreen 启动通过 |
| `file` / `ldd` | Linux x86-64 动态链接 ELF，Debug 信息保留；当前系统运行依赖无缺失 |

最后一次 CTest 总耗时 8.02 秒仅用于执行定位，不是导航性能基准；原始输出见 [ui-tests.log](ui-tests.log)、[Wayland 测试日志](wayland-ui-tests.log)。Qt Test 的 `propagateSizeHints()` 警告来自 offscreen 插件，不影响用例通过。Wayland 启动时有 Mesa/EGL 设备初始化警告，3D 加速保持关闭；已观察到 QWidget 界面正常显示，不能据此声称硬件加速通过。Qt 发行版库中的编译器元信息与应用编译器是两个来源；应用实际 CMake 编译器为 GCC 13.3.0。

截图发现演示进度数字输入的默认箭头边框不整齐，改为无按钮的数值输入后，在同一环境重新构建、复验与生成全部截图。以下图像和可执行包均来自修正后的版本。

原生桌面补测发现固定 1440×900 初始窗口会在 1280×800 来宾屏幕上越界：现已按可用屏幕限制初始客户区，并在较小桌面启动时请求 compositor 最大化，使系统面板和原生标题栏参与布局。新增 `initialClientFitsAvailableScreen()` 回归用例；offscreen 参考图仍显式使用 1440×900 / 1024×720。演示对话框改为显式排列“取消、确认”，与 design.md 对齐。上述修改完成后重新构建、双平台复验、安装及更新程序包。

## 图像证据

这些是 **Ubuntu Qt offscreen 渲染**，不包含 Linux 原生窗口标题栏，不能当作 Wayland 桌面截图、原生文件对话框或硬件渲染验收。

- [浅色导入页，1440×900](import-light.png)
- [深色导入页，1440×900](import-dark.png)
- [紧凑导入页，1024×720](import-compact.png)
- [公共组件页，1440×900](components-light.png)

以下是 computer-use 直接捕获的 **实际 VirtualBox / Ubuntu Wayland 桌面**，保留宿主虚拟机窗口、Ubuntu 系统栏与原生标题栏，不与 offscreen 图混用：

- [修正版浅色桌面](native-import-light.png)
- [深色桌面及保留的文件选择](native-import-dark.png)
- [真实文件选择框](native-file-dialog.png)
- [文件选择返回后的状态](native-file-selected.png)
- [真实组件页](native-components-dark.png)
- [最终演示对话框：取消左、确认右](native-demo-dialog.png)

用户已登录并在中途锁屏后手动解锁。桌面核对包含：应用启动、修正后完整窗口可见、主题切换、组件页与演示对话框打开、文件选择与元信息返回，以及主题切换后保留选择。文件样例为工程内 `CMakeLists.txt`，只检查选择与元信息，不解释为业务格式或导入成功。当前弹出的是 Qt 标准文件选择框，未证明通过 GNOME portal/GTK 原生选择器。

## 交付与复现定位

- 来宾工程：`~/navigation-assignment1-20261026-1626/`。
- 来宾安装入口：`~/navigation-assignment1-20261026-1626/out/app/bin/p1-navigation`。
- 宿主 Linux 包：`out/p1-navigation-linux-amd64.tar.gz`（未纳入 Git），包含 `bin/p1-navigation`；在同基线 Ubuntu 解压后运行，需要系统提供 Qt 6.4.2 Widgets 运行库及平台插件。
- 本轮最终源码归档 SHA256：`F60751EA1C6ABC18D77BB1FC1F75238B92C47197D1C13E569EDA1F0D66509245`。
- 最终可执行包 SHA256：`F55E5FA4FAB9CEA16C38A5457CDAFB3E2283D3554E4617FF8A2E5331F815EE9B`。

## 待验收

常规桌面基础能力已完成补测。跨虚拟机键盘注入未能稳定验证，完整人工 Tab 顺序仍待复核；对话框 Esc 行为已在真实 Wayland Qt Test 中通过。高 DPI/200% 字号、GNOME portal 外观、其他成员复现与全部业务导入仍未验收，故基础子项可完成，应用框架总体仍保留“进行中”。原虚拟机文件系统故障仍未修复。
