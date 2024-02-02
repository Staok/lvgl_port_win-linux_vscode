如果这里打开 USE_WIN_SDL2LIB 选项（即 lv_drv_conf.h 里面 打开宏 USE_SDL），那么需要系统先有 SDL2 库，按照以下步骤

win 下，加入 SDL2 库

1. 首先下载到 SDL2 包 "SDL2-devel-2.28.2-mingw"
2. 将其中的 "x86_64-w64-mingw32" 文件夹 直接 解压放到 "mingw64" 下的 "x86_64-w64-mingw32" 文件夹下！
3. 确保 ".../mingw64/bin/" 已经加入到 环境变量 "Path" 中
4. 并且！ 将 ".../mingw64/x86_64-w64-mingw32/bin" 加入到 环境变量 "Path" 中，因为用到了 该路径下的 SDL2.dll
5. 添加 环境变量 后， win 下可能需要重启才能生效