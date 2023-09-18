# 关于 SDL2 库的配置看 “1-SDL2_lib_text_show” 文件夹里面

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
set(CMAKE_CXX_STANDARD 11)
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




