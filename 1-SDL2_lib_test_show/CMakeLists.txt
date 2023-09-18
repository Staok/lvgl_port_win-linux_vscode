
# win 下，加入 SDL2 库
# 首先下载到 SDL2 包 "SDL2-devel-2.28.2-mingw"
# 将其中的 "x86_64-w64-mingw32" 文件夹 直接 解压放到 "mingw64" 下的 "x86_64-w64-mingw32" 文件夹下！
# 确保 ".../mingw64/bin/" 已经加入到 环境变量 "Path" 中
# 并且！ 将 ".../mingw64/x86_64-w64-mingw32/bin" 加入到 环境变量 "Path" 中，因为用到了 该路径下的 SDL2.dll
# 添加 环境变量 后， win 下可能需要重启才能生效

# 构建 & 编译
# 在 build 文件夹 里面
# cmake -G "Unix Makefiles" ..
# cmake --build . -j4
# ./test_SDL2.exe 
# "./test_SDL2.exe" 在 git bash 里面跑不起来，直接双击 或者 powerShell 里面都能跑起来

cmake_minimum_required(VERSION 3.0.0)

project(test_SDL2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} mingw32)
target_link_libraries(${PROJECT_NAME} SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2)


