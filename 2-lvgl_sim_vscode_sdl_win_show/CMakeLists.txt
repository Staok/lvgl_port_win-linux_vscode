# 关于 SDL2 库的配置看 “1-SDL2_lib_text_show” 文件夹里面

# 构建 & 编译 & 运行
# 在 build 文件夹 里面 执行：
# cd build
# cmake -G "Unix Makefiles" ..
# cmake --build . -j4
# ./test_lvgl_win_sdl2.exe

# 清理 重新编译
# cmake --build . --target 'clean'

# 并且修改的地方
# 在 lvgl 文件夹 里面 cmake 文件，最后添加库的地方，
# 加一句 file(GLOB_RECURSE SOURCES_EXAMPLE examples/*.c)，把这个也编译，这样才能用 lvgl/example

cmake_minimum_required(VERSION 3.0.0)

project(test_lvgl_win_sdl2 VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 为所有目标添加头文件目录，因为本程序不止 ${PROJECT_NAME}  一个目标，还有 lvgl、lv_drivers 等
# 否则 lv_drivers 等目标 会找不到 lvgl 文件夹里面的头文件
# 而且要放在最前面
include_directories(
    ${PROJECT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} 
    main.c

    # 在 hal_init() 里面注册鼠标图片，但会有重影，相关语句注释掉了，没用到
    mouse_cursor_icon.c 
)

# 指示 lv_conf.h 和 lv_drv_conf.h 配置文件 在 顶层目录里，lvgl 编译需要这个宏定义
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)

add_subdirectory(lvgl)
add_subdirectory(lv_drivers)
add_subdirectory(lv_examples)

target_link_libraries(${PROJECT_NAME} mingw32)
target_link_libraries(${PROJECT_NAME} lvgl lv_drivers lv_examples)
target_link_libraries(${PROJECT_NAME} SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2)



# add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME})


