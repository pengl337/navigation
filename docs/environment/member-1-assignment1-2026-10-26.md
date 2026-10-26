# 1 号第一次作业验证环境

日期：2026-10-26（Asia/Shanghai）。本记录区分历史基础环境与本轮框架验证环境，应用说明见 [框架文档](../assignment-1-framework.md)。

## 原虚拟机复检

原虚拟机 `P1-Ubuntu-24.04` 本轮启动后，SSH 执行 `/usr/bin/uptime`、`/usr/bin/free` 返回 `Input/output error`，SCP 文件传输关闭。通过 Bash 内建读取 `/proc/mounts`，根目录显示：

```text
/dev/sda2 / ext4 rw,relatime,emergency_ro 0 0
```

此状态不能作为可用构建环境。根因尚未确定，没有自动运行文件系统修复、格式化、删除磁盘或回滚原虚拟机；也不据此否定此前已经保存的基础验证历史。

## 用户选择与独立克隆

用户本轮明确选择：“保留现有虚拟机，使用基础快照的独立克隆进行验证（推荐）”。据此从 `p1-dev-baseline`（快照 UUID `a622eda7-b9e8-43fc-bf02-b7f32a9c44db`）创建链接克隆：

- 名称：`P1-Assignment1-Verify-20261026`。
- 本机位置：`I:\LinuxVM\VMs\P1-Assignment1-Verify-20261026`。
- 配置：1 vCPU / 8192 MiB RAM / VMSVGA / 3D 关闭；VirtualBox 7.2.22。
- 网络：NAT，本机回环 `127.0.0.1:2223` → 来宾 SSH 22；不向局域网开放。
- 启动后根文件系统为 `/dev/sda2 / ext4 rw,relatime`，未出现原机的 emergency_ro 标志。
- `tools/verify-environment.sh` 实际通过：Ubuntu 24.04 noble、x86_64、Qt 6.4.2、GCC/G++ 13.3.0、CMake 3.28.3。
- 本轮源码目录：来宾 `~/navigation-assignment1-20261026-1626/`；独立于原环境检查工程。

克隆与原机共享基础快照磁盘链，保留克隆期间不得删除原快照或共享基盘。克隆含本机账号配置，不能作为已脱敏的团队镜像分发；凭据和连接密钥未写入仓库。

## 应用验证状态

构建使用 Debug / Ninja / GCC 13.3.0 / `-j1`。Qt CMake 配置成功，但输出可选 XKB 库/头文件未找到的提示；没有因此擅自安装依赖或改变团队版本。原基础版本 Qt Test 为 7 通过；桌面补测修正初始窗口适配并新增回归用例后，最终 offscreen 与真实 Wayland 两个平台均为 8 通过、0 失败，安装后启动也通过。原始报告、4 张 offscreen 图与实际桌面截图见 [验证记录](../verification/assignment1/README.md)。

用户本轮已登录并在中途锁屏后自行解锁；桌面会话为 Wayland，根目录仍可写。使用同一安装程序检查了初始窗口、主题切换、组件页面/对话框及 Qt 文件选择框；Noto Sans CJK SC Regular 匹配成功。日志存在 Mesa/EGL 初始化警告，3D 保持关闭，不能声称硬件加速通过。GNOME portal 原生外观、完整人工 Tab 顺序、高 DPI/200% 字号仍待验证；没有关闭锁屏或其他系统安全设置。

原虚拟机的文件系统修复仍是独立待办。克隆仅用于本轮验证，不说明原环境已恢复正常。关联日志：`20261026-162617-member1-framework-05`。

桌面补测日志：`20261026-174106-member1-native-ui-07`。用户登录后不再以“等待登录”作为阻塞；当前剩余项为完整缩放/键盘验收、原机修复及其他成员复现。
