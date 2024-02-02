cmake_minimum_required(VERSION 3.16)

project(appui)

# 这个文件夹是 ui 框架 强相关的，提供 ui hal 初始化相关 API，以及 各个 按键等控件 的回调函数

set(SRC_DIR_APPUI_UI               ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(SRC_DIR_APPUI_TRANS            ${CMAKE_CURRENT_SOURCE_DIR}/trans)

### all src file(s)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}            UI_INTERFACE_SRC)
aux_source_directory(${SRC_DIR_APPUI_TRANS}                 UI_TRANS_SRC)

### make appui static lib and add extra compile options
add_library(${PROJECT_NAME} STATIC
    ${UI_INTERFACE_SRC}
    ${UI_TRANS_SRC}
)

# add appui extra compile options
# "ui" cannot pass these compile options
target_compile_options(${PROJECT_NAME} PRIVATE
    -pedantic -Wpedantic
)

### add needed lib(s)
add_subdirectory(${SRC_DIR_APPUI_UI})

    # "ui" needed head dirs
    target_include_directories(ui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${SRC_DIR_APPUI_TRANS}
        ${SRC_DIR_LVGL_TOP}
        ${SRC_DIR_LVGL}
    )

### link needed lib(s)
if(WIN32)
    if(USE_WIN_SDL2LIB)
        target_link_libraries(${PROJECT_NAME} PRIVATE mingw32 SDL2main SDL2)
    endif()
elseif(UNIX)
    if(USE_LINUX_DRM)
        # in new version of lv_drivers, no libdrm linked in its CMakeLists.txt...if no then here add it
        find_package(PkgConfig)
        pkg_check_modules(PKG_LIBDRM REQUIRED libdrm)
        target_include_directories(lv_drivers PUBLIC ${PKG_LIBDRM_INCLUDE_DIRS})
        target_link_libraries(lv_drivers PUBLIC ${PKG_LIBDRM_LIBRARIES})
    endif()
endif()

# lv_drivers linked lvgl, so here link lv_drivers
target_link_libraries(${PROJECT_NAME} PRIVATE
    ui
    lv_drivers
    pthread
)

if(USE_DEMOS_EXAMPLES)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        lvgl_examples
        lvgl_demos
    )
endif()

### include all appui needed head file(s) of other target(s)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR_APP}
)

if(USE_DEMOS_EXAMPLES)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${SRC_DIR_LVGL}/demos
        ${SRC_DIR_LVGL}/examples
    )
endif()

### export all appui api head file(s)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${SRC_DIR_APPUI_UI}
    ${SRC_DIR_APPUI_TRANS}
    ${SRC_DIR_LVGL_TOP}
    ${SRC_DIR_LVGL} # sb lvgl cmake not export its inc dirs
)
