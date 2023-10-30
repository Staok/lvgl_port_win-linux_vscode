# 如果在 lv_drv_conf.h 里面 打开宏 USE_SDL，那么需要系统先有 SDL2 库，按照以下步骤
# win 下，加入 SDL2 库
# 首先下载到 SDL2 包 "SDL2-devel-2.28.2-mingw"
# 将其中的 "x86_64-w64-mingw32" 文件夹 直接 解压放到 "mingw64" 下的 "x86_64-w64-mingw32" 文件夹下！
# 确保 ".../mingw64/bin/" 已经加入到 环境变量 "Path" 中
# 并且！ 将 ".../mingw64/x86_64-w64-mingw32/bin" 加入到 环境变量 "Path" 中，因为用到了 该路径下的 SDL2.dll
# 添加 环境变量 后， win 下可能需要重启才能生效

# 构建 & 编译 & 运行
# 在 build 文件夹 里面 执行：
# cd build
# cmake -G "Unix Makefiles" ..
# cmake --build . -j4
# ./lvgl_proj.exe

# 清理 重新编译
# cmake --build . --target 'clean'

cmake_minimum_required(VERSION 3.10)

project(lvgl_proj C)

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/screen
    ${PROJECT_SOURCE_DIR}/utils
)

add_executable(${PROJECT_NAME} screen/main.c)

add_compile_definitions(LV_CONF_INCLUDE_SIMPLE) # lvgl 用

add_subdirectory(screen/lvgl)
add_subdirectory(screen/lv_drivers)
add_subdirectory(screen/ui)
add_subdirectory(utils)

target_link_libraries(${PROJECT_NAME} lvgl_utils)
target_link_libraries(${PROJECT_NAME} lvgl lv_drivers ui)
target_link_libraries(${PROJECT_NAME} lvgl_examples lvgl_demos)
target_link_libraries(${PROJECT_NAME} mingw32)
target_link_libraries(${PROJECT_NAME} SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} pthread)




