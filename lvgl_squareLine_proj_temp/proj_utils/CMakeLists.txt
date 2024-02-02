cmake_minimum_required(VERSION 3.16)

project(proj_utils)

# 本目录是与当前 proj 相关的杂项东西

# all src file(s)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ PROJ_UTILS_SRC)

# make proj_utils static lib
add_library(${PROJECT_NAME} STATIC
    ${PROJ_UTILS_SRC}
)

# add app extra compile options
target_compile_options(${PROJECT_NAME} PRIVATE
    -pedantic -Wpedantic
)

# export all proj_utils api head file(s)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)


