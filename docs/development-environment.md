# 统一开发环境：Linux 桌面虚拟机

约定日期：2026-10-26。适用于 P1 Linux 组五位成员。本文是环境基线的维护入口；1 号本机已完成 I 盘 Ubuntu 虚拟机安装、版本核对、独立 Qt Widgets 桌面运行和 Qt Creator Kit 构建验证，见 [实测记录](environment/member-1-2026-10-26.md)。其余成员待复现，导航业务工程尚未创建。

## 1. 统一版本

2026-10-26 第一次作业复检补充：原虚拟机出现根文件系统 emergency_ro 和系统命令 I/O 错误；用户选择保留原机并从基础快照创建独立克隆验证。版本约定不变，本轮实测状态及克隆依赖见 [第一次作业验证环境](environment/member-1-assignment1-2026-10-26.md)，不能用历史环境检查成功代替本轮应用验收。

| 项目 | 统一要求 | 说明 |
| --- | --- | --- |
| 开发方式 | Linux 桌面虚拟机 | Windows 可作为宿主机；编译、运行与主要验收在虚拟机内进行 |
| Linux 发行版 | Ubuntu Desktop 24.04 LTS，代号 noble | 使用 Desktop 桌面镜像；不混用 Server、其他发行版或其他 Ubuntu 系列作为团队验收基线 |
| CPU 架构 | x86_64 / amd64 | 五人一致，避免 amd64/arm64 二进制混用 |
| Qt | **6.4.2** | 使用 Ubuntu noble 的 Qt 6 开发包；不混用 Qt 5、其他 Qt 6 版本或宿主机 Qt Kit |
| 编译器 | **GCC / G++ 13.3.0** | 显式使用 gcc-13、g++-13，验证完整版本，不能只看到“13”就视为一致 |
| CMake | **3.28.3** | 使用虚拟机内该版本；IDE 内置、pip、Snap 或其他安装目录的 CMake 需防止优先级覆盖 |
| C++ 标准 | C++17 | 后续 CMake 目标显式设置标准；不得个别成员自行升级 |
| IDE | Qt Creator（Ubuntu 软件源版本） | IDE 版本不是上述四项基线之一，但首次配置时记录；Kit 必须指向本表工具链 |

这里锁定的是 Ubuntu 系列、架构及工具的上游完整版本。Ubuntu 24.04.x 点版本、内核、发行版安全修订号和其他依赖也需记录；它们不等于新的 Qt/GCC/CMake 上游版本。全员以 1 号首次验证后的共同镜像/软件包清单复现，不凭“都是 24.04”就认定环境完全相同。

选择依据（2026-10-26 核对官方包条目，并已在 1 号虚拟机实测）：Ubuntu noble 提供 Qt 6.4.2、GCC/G++ 13.3.0 和 CMake 3.28.3；这是便于团队复现的项目约定，不声称是最新版本，也不声称所有 PAL 后端已完成兼容验证。

- [Ubuntu 桌面镜像](https://releases.ubuntu.com/24.04/)
- [Qt 6 开发包](https://packages.ubuntu.com/noble/qt6-base-dev)（包修订参考：6.4.2+dfsg-21.1build5）
- [G++ 13](https://packages.ubuntu.com/noble/g%2B%2B-13)（amd64 包修订参考：13.3.0-6ubuntu2~24.04.1）
- [CMake](https://packages.ubuntu.com/noble/cmake)（包修订参考：3.28.3-1build7）

软件源可能更新。上述核心包修订号与 1 号实测一致，完整安装清单见实测记录；实际可安装版本不符合上表时先报告差异，由 1 号核对并统一处理，不悄悄换版或强行混用依赖。

## 2. 虚拟机约定

1. 1 号先选定宿主软件并建立 Ubuntu Desktop 24.04 LTS amd64 基础虚拟机，记录软件/版本、ISO 文件名及官方 SHA256、系统点版本、CPU/内存/磁盘配置、显示协议和图形驱动。1 号本机已使用 VirtualBox 7.2.22，Ubuntu 24.04.4；本机资源与兼容配置见实测记录，其他成员仍需记录各自实际配置。
2. 完成工具链安装与最小 Qt Widgets 工程验证后，保存基础快照及软件包清单。其他成员优先使用同一不含个人凭据的基础镜像/克隆，或按清单重建。虚拟机镜像放团队另行约定的位置，不提交大体积镜像到源码仓库。
3. 源码优先克隆到虚拟机内的用户目录。跨系统共享文件夹用于交换资料；避免把宿主机生成物当成 Linux 构建结果。切换工具链或宿主/来宾环境时使用独立构建目录。
4. 构建、运行、截图和录屏在 Ubuntu 桌面中完成，明确标记“虚拟机”。记录宿主与虚拟机资源和是否使用软件渲染；只在可比条件下比较性能。
5. 验证虚拟机的网络、音频输入输出和图形能力。GeoClue2 返回的真实定位、ASR 麦克风访问、OpenGL ES 支持与性能须逐项实测；模拟位置或软件渲染单列，不自动视为真实定位/硬件加速已验收。当前约定并未替 5 号选定渲染后端或替 1 号选定语音后端。

## 3. 安装与版本核对（在 Ubuntu 虚拟机内执行）

以下步骤已在 1 号虚拟机完成，供其他成员复现。先确认使用 Ubuntu noble 的正式软件源并启用所需的 universe 组件；如果 apt 找不到包，应检查软件源和网络，而不是直接改用其他 Ubuntu 版本的包。

```bash
sudo apt update
sudo apt install gcc-13 g++-13 cmake qtcreator qt6-base-dev qt6-base-dev-tools qt6-qpa-plugins libqt6opengl6-dev libqt6sql6-sqlite ninja-build pkg-config git
```

安装后逐项核对；安装命令不保证软件源未来仍返回相同版本，版本不符时停止使用该环境作为团队基线：

```bash
cat /etc/os-release
uname -m
gcc-13 -dumpfullversion
g++-13 -dumpfullversion
cmake --version
qmake6 -query QT_VERSION
command -v gcc-13 g++-13 cmake qmake6
```

期望：VERSION_ID 为 24.04、代号 noble、架构 x86_64；两个编译器都为 13.3.0，CMake 为 3.28.3，Qt 为 6.4.2。再保存实际安装包及系统信息，建议放在 `docs/environment/` 的成员记录中（首次配置时创建）：

```bash
dpkg-query -W -f='${binary:Package}\t${Version}\n' gcc-13 g++-13 cmake cmake-data qt6-base-dev qt6-base-dev-tools qtcreator
uname -r
```

Qt Creator 配置 Desktop Kit 时检查：Qt 6.4.2、`/usr/bin/gcc-13`、`/usr/bin/g++-13`、实际版本为 3.28.3 的 CMake 路径。仅安装软件或成功打开 IDE 不等于工程已验证。

后续创建工程时应在 CMake 声明 C++17，并通过 `find_package(Qt6 6.4.2 EXACT REQUIRED COMPONENTS Widgets)` 锁定 Qt。配置时显式指定 GCC/G++ 13 路径，并检查 CMake 输出中的实际编译器版本；具体构建命令待工程存在后实测写入 README，不能将本节示例当作已经可运行的项目。

## 4. 升级和验收

- 五人不可单独更换发行版系列、架构、Qt、GCC 或 CMake 版本。系统更新涉及基线变动时，由 1 号先验证并记录变更原因/影响，通知成员统一更新本文与基础镜像/清单。
- 不以长期关闭系统安全更新维持一致；发行版修订变化需要记录并统一关键依赖。算法对比和 G6 联调报告必须写实际包版本、环境与资源，不能仅引用本约定。
- 环境验收至少包含：四项版本核对、最小 Qt Widgets 窗口在 Ubuntu 桌面启动、CMake 使用正确 Kit、另一位成员复现。PAL 服务与完整导航按各模块另行验收。

| 成员 | 虚拟机与版本核对 | 最小 Qt/CMake 工程 | PAL/设备能力与证据 |
| --- | --- | --- | --- |
| 1 | 已验证：Ubuntu 24.04.4 / Qt 6.4.2 / GCC 13.3.0 / CMake 3.28.3 | 独立 Qt Widgets 工程桌面运行、Qt Creator Kit 构建通过 | [实测记录](environment/member-1-2026-10-26.md)；OpenGL ES/音频/定位与 PAL 未验证 |
| 2 | 未验证 | 未验证 | 待复现 |
| 3 | 未验证 | 未验证 | 待复现 |
| 4 | 未验证 | 未验证 | 待复现 |
| 5 | 未验证 | 未验证 | 待复现 |
