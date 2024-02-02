cmake_minimum_required(VERSION 3.16)

project(app)

# APP 是后端，应该没有依赖，可独立编译、独立运行

# all src file(s)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ APP_SRC)

# make app static lib
add_library(${PROJECT_NAME} STATIC
    ${APP_SRC}
)

# add app extra compile options
target_compile_options(${PROJECT_NAME} PRIVATE 
    -pedantic -Wpedantic
)

# export all app_backend api head file(s)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

