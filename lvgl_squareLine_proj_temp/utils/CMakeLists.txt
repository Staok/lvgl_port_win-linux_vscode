cmake_minimum_required(VERSION 3.0)

project(lvgl_utils)

# 搜索当前 cmakelist 目录下的所有源文件，放到 SRC 变量 里
# aux_source_directory(./ SRC)

# 添加本目录下所有源文件
set(SOURCES_FILES
    lv_demosAndExamples.c
    lv_proj_hal_init.c
)

# set(HEAD_FILES_DIR
# )

add_library(${PROJECT_NAME} STATIC
    ${SOURCES_FILES}
    # ${SRC}
)

# for this proj
# target_include_directories(${PROJECT_NAME}
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${HEAD_FILES_DIR}
# )

# for all proj
# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     # ${HEAD_FILES_DIR}
# )


