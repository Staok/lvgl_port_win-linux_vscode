# lvgl_port_win_vscode
LVGL移植win端（SDL或windrv）模拟显示流畅解决方案，并且同时支持linux端（DRM或FB）。VsCode工程，cmake 构建，可 F5 一键运行。使用 SquareLine 生成前端 UI 文件。

相比官方的 lvgl 移植到 win 端的几种方法 [Simulator on PC — LVGL documentation](https://docs.lvgl.io/8.3/get-started/platforms/pc-simulator.html)，本仓库带来更好的 lvgl 在 win 端的模拟显示 体验！

使用 SquareLine 生成前端 UI 文件，win 上直接跑。

仓库约 240MB 左右，因为直接包含了 LVGL 相关库源文件，直接 git clone 或者 下载 压缩包 就能开始运行。

全部使用 手写的 cmake 构建，具有 学习 和 实践使用 意义。



本仓库地址：

github 仓库 https://github.com/Staok/lvgl_port_win_vscode

gitee 仓库 https://gitee.com/staok/lvgl_port_win_vscode

## 使用说明

### 环境

首先，需要 win 有 mingw-w64 和 cmake 等基本环境，以及 VsCode（带 `C/C++`、`C/C++ Extension Pack`、 `CMake` 等相关常用插件）。

我的一个 VsCode 跑一跑 C/C++ 程序的工程模板：见 [Staok/coding-style-and-more(github.com)](https://github.com/Staok/coding-style-and-more) 或 [Gitee](https://gitee.com/staok/coding-style-and-more) ，在 VsCode temp Proj 文件夹。



**win 下基本工具下载地址汇总**

p.s 工具下载后 bin 目录放到 环境变量里，win 和 linux 同理。



**mingw-w64**

[Downloads - MinGW-w64](https://www.mingw-w64.org/downloads/)。

往下拉找到 `Tarballs for the mingw-w64 sources are hosted on SourceForge.`，点开其中的 `SourceForge`。

进入 [MinGW-w64 - for 32 and 64 bit Windows - Browse Files at SourceForge.net](https://sourceforge.net/projects/mingw-w64/files/)，下面找到 `MinGW-W64-install.exe`，安装即可。

具体安装流程介绍 [MinGW-w64的安装及配置教程_mingw64-CSDN博客](https://blog.csdn.net/didi_ya/article/details/111240502)。

或者用 clang 编也行。



**make**

[Make for Windows (sourceforge.net)](https://gnuwin32.sourceforge.net/packages/make.htm)。

这个win gnu make下载地址最后是3.81(2006年)，最新的可以用 mingw-w64 里面的 mingw32-make.exe，复制一份改名 make.exe 即可



**cmake**

[Download CMake](https://cmake.org/download/#latest)，win 平台下载 `Windows x64 ZIP` 即可。

p.s 在 在 VSCode 里面，要使用 gitbash 来执行 cmake 等命令，使用 powershell 执行 可能会有各种问题。。



### 使用

三个文件夹，为三个独立工程，分别可独立 编译 & 运行；每个文件夹里面都有说明，要看。

- 1-SDL2_lib_test_show，这是 测试运行 SDL2 库（win 端的创建窗口应用的库）的，装 SDL2 库、编译 和 运行 看 CMakeList 文件里面。
- 2-lvgl_sim_vscode_sdl_win_show，这是在官方 [VSCode with SDL driver](https://github.com/lvgl/lv_sim_vscode_sdl) 上的魔改，借鉴了一些网友的构建和例子，程序里均给出了参考链接。
- lvgl_squareLine_proj_temp，作为通用的 lvgl 工程模板。这是 集合官方最新 lvgl 库、lv_drivers 库、SDL2、SquareLine 生成的前端 UI 文件 的工程，以及添加了一些功能，cmake 构建，添加了 VsCode 的 task，一键 F5 运行。SquareLine 是 可视化的 lvgl 界面设置 软件，其直接生成 lvgl 可以运行的 界面代码。



lvgl_squareLine_proj_temp 目前还只支持在 win 端运行。

lvgl_squareLine_proj_temp 作为通用的 lvgl 工程模板，目标是 一个文件夹， win 端 和 嵌入式linux 端 或 mcu 端（经交叉编译）均可以跑，即 pc 上 模拟显示 & 反复调整，下载到 嵌入式 linux 板子 或 mcu 板子 就跑。并且 UI 的前后端分离。做到通用化的一个 lvgl 小模板、小框架。



### 效果

一键 F5 —— 启动！

![run_show](README.assets/run_show.png)
