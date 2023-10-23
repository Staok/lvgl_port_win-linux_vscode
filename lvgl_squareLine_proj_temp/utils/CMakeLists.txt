cmake_minimum_required(VERSION 3.0)

project(lvgl_utils)

# 搜索当前 cmakelist 目录下的所有源文件，放到 SRC 变量 里
# aux_source_directory(./ SRC)

# 添加本目录下所有源文件
set(SOURCES_FILES
    demosAndExamples.c
    win_hal_init.c
    periodic_running_basic.c
)

# set(HEAD_FILES_DIR 生成库用不到带着头文件
#     .
# )

add_library(${PROJECT_NAME} 
    STATIC
        ${SOURCES_FILES}
        # ${SRC}
)

# target_include_directories(${PROJECT_NAME}
#     ${PROJECT_SOURCE_DIR}
#     ${HEAD_FILES_DIR}
# )


