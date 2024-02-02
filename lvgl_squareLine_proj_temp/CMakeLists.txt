# 构建 & 编译 & 运行，使用 git bash 运行下述命令 (build cmd, use gitbash)
# 当目录结构改变或者 option 选项改变，就必须删除 build 目录，再重新 "cmake & make" 编译 (when dir strut or option changed, must clean "build" dir and redo "cmake & make")

# 创建并进入 build 文件夹 (creat a "build" dir and get in)
# mkdir build && cd build

# 在 build 文件夹里面 执行（每次改变 option 后需要全清 build 目录重新执行下述命令） (run cmake & make cmd in win)
# cmake -G "MinGW Makefiles" .. (or cmake -G "Unix Makefiles" .., this for unix environment)
# cmake --build . -j16 && ./lvgl_proj.exe

# linux 下执行，需要通过命令设置 编译器完整路径，设置变量不行 (in linux, use this to specifying compilers)
# cmake -G "Unix Makefiles" .. -DCMAKE_C_COMPILER=.../arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=.../arm-linux-gnueabihf-g++

# 清理，也可以直接删除 build 目录 (clean build, or just directly delete the "build" dir)
# cmake --build . --target 'clean'

# 目录结构
# ├─.vscode                  win 端 VsCode IDE 配置
# ├─build                    win 上，cmake 构建存放的目录
# ├─3rdpart                  三方库
# │  ├─official              lvgl 源码库
# │  │  ├─lvgl
# │  │  └─lv_drivers
# │  └─rockchip              rk 对 lvgl 的 hal 层移植，目前用这个初始化 lvgl hal
# ├─proj_main                main，程序入口
# ├─proj_conf                工程配置文件
# ├─App                      后端，要与前端解耦，可单独编译、单独运行
# ├─PppUI                    前端，要与 Win 或 linux 等平台无关，plain c/c++ 编译
# │  ├─trans                 放 lv_i18n 的翻译文件，多语言的文案
# │  ├─ui                    squareLine 生成的前端 code
# │  └─ui_backend            前端相关交互code、接口封装。即 squareLine 实现不了的效果（比如 add event 后 要给 image recolor 在 squareLine 里面做不了），还有按页面来引出接口，都放到这个里面
# └─proj_utils               utils, stuffs .etc
# └─tests                    test things .etc

cmake_minimum_required(VERSION 3.16)

project(lvgl_proj LANGUAGES C CXX)

message(STATUS "### PROJECT_NAME: ${PROJECT_NAME}")

##################################################################################################
## Compile Options
##################################################################################################

###### specifying compiler

if(WIN32)
elseif(UNIX)
#    set (CMAKE_C_COMPILER   .../arm-linux-gnueabihf-gcc)
#    set (CMAKE_CXX_COMPILER .../arm-linux-gnueabihf-g++)
endif()

###### options

### platform display method choose
if(WIN32)
    # win 端使用 sdl2 库进行显示，需要预安装此库，win 端 安装 sdl2 库见 "misc/win下加入sdl2库说明.txt"
    # use sdl2 need pre-install sdl2 lib in system
    option(USE_WIN_SDL2LIB      "in win platform use sdl2 lib as disp and indev drv"                OFF  )
    option(USE_WIN_WINDRV       "in win platform use win_drv in lv_drivers as disp and indev drv"   ON )
elseif(UNIX)
    # 这里 linux 端 需系统 预安装 drm 库
    # use drm need pre-install libdrm in system
    option(USE_LINUX_DRM        "in linux platform use drm as display method"                       ON  )
    option(USE_LINUX_FB         "in linux platform use fb  as display method"                       OFF )
endif()

option(USE_DEMOS_EXAMPLES       "use demos and examples"                                            ON  )

### compile extra flags options
option(COMPILE_DISTINGUISH_COMPILERS "Distinguish compilers and perform special flags"              OFF )
option(COMPILE_WERROR           "Turn compiler warnings into errors"                                OFF )
option(COMPILE_DEBUG            "Turn on debug compile"                                             OFF )

###### compile flags .etc
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### compile basic flags, add flags to compiler

# for all proj
add_compile_options(
    -O2
    -Wall -Wextra
)

if(COMPILE_DISTINGUISH_COMPILERS)
    MESSAGE(STATUS "### option: Distinguish compilers and perform special flags")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wno-psabi)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
    endif()
endif()

if(COMPILE_WERROR)
    MESSAGE(STATUS "### option: Turn compiler warnings into errors")
    # add_compile_options(-Werror) demo, drivers, ui 等库过不了 -Werror 选项
endif()

if(COMPILE_DEBUG)
    MESSAGE(STATUS "### option: Turn on debug compile")
    add_compile_options(-g -ggdb)
endif()

### platform dependent
if(WIN32)
    if(USE_WIN_SDL2LIB)
        if(USE_WIN_WINDRV)
            message(FATAL_ERROR "Can not run when both set: USE_WIN_SDL2LIB and USE_WIN_WINDRV.")
        endif()
        MESSAGE(STATUS "### option: in win platform use sdl2 lib as disp and indev drv")
        add_compile_definitions(USE_SDL=1)
    endif()
    if(USE_WIN_WINDRV)
        if(USE_WIN_SDL2LIB)
            message(FATAL_ERROR "Can not run when both set: USE_WIN_SDL2LIB and USE_WIN_WINDRV.")
        endif()
        MESSAGE(STATUS "### option: in win platform use win_drv in lv_drivers as disp and indev drv")
        add_compile_definitions(USE_WINDOWS=1)
    endif()
endif()

if(UNIX)
    add_compile_definitions(USE_EVDEV=1)
    if(USE_LINUX_DRM)
        if(USE_LINUX_FB)
            message(FATAL_ERROR "Can not run when both set: USE_LINUX_DRM and USE_LINUX_FB.")
        endif()
        MESSAGE(STATUS "### option: in linux platform use drm as display method")
        add_compile_definitions(USE_DRM=1)
    endif()
    if(USE_LINUX_FB)
        if(USE_LINUX_DRM)
            message(FATAL_ERROR "Can not run when both set: USE_LINUX_DRM and USE_LINUX_FB.")
        endif()
        MESSAGE(STATUS "### option: in linux platform use fb as display method")
        add_compile_definitions(USE_FBDEV=1)
    endif()
endif()

if(USE_DEMOS_EXAMPLES)
    MESSAGE(STATUS "### option: use demos and examples")
    add_compile_definitions(LV_USE_DEMOS=1 LV_BUILD_EXAMPLES=1)
endif()

###### add defines to all targets
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE) # lvgl 用
add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

# MESSAGE(STATUS "### CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
# MESSAGE(STATUS "### CMAKE_C_FLAGS   " ${CMAKE_C_FLAGS})

##################################################################################################
## Source And Its Compile Flags
##################################################################################################

# MESSAGE(STATUS "### PROJECT_SOURCE_DIR       "  ${PROJECT_SOURCE_DIR})
# MESSAGE(STATUS "### CMAKE_CURRENT_SOURCE_DIR "  ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_DIR_PROJ_CONF           ${CMAKE_CURRENT_SOURCE_DIR}/proj_conf)
set(SRC_DIR_APP                 ${CMAKE_CURRENT_SOURCE_DIR}/App)
set(SRC_DIR_APPUI               ${CMAKE_CURRENT_SOURCE_DIR}/AppUI)
set(SRC_DIR_PROJ_UTILS          ${CMAKE_CURRENT_SOURCE_DIR}/proj_utils)
set(SRC_DIR_LVGL_TOP            ${CMAKE_CURRENT_SOURCE_DIR}/3rdpart/lvgl)
set(SRC_DIR_LVGL                ${CMAKE_CURRENT_SOURCE_DIR}/3rdpart/lvgl/lvgl)
set(SRC_DIR_LV_DRIVERS          ${CMAKE_CURRENT_SOURCE_DIR}/3rdpart/lvgl/lv_drivers)

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/proj_main/main.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

### specific target include
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/proj_main
)

### common include
### all files may use APIs from these head files
include_directories(
    ${SRC_DIR_PROJ_CONF}
)

target_compile_options(${PROJECT_NAME} PRIVATE 
    -pedantic -Wpedantic
)

##################################################################################################
## Linking libs
##################################################################################################

# 调用关系（同一层的是并行关系，之间保持无调用）
#                            main
#                             ↓
# appui(ui lib + src files)      proj_utils
#          ↓    
#   lv_drivers app
#          ↓
#        lvgl

###### add subdirectories / add dependent lib(s) ######

add_subdirectory(${SRC_DIR_LVGL})         # for "lv_drivers" to link with
add_subdirectory(${SRC_DIR_LV_DRIVERS})   # for "appui" to link with

    # "lv_drivers" needed head dirs
    target_include_directories(lv_drivers PRIVATE
        ${SRC_DIR_LVGL_TOP}
        ${SRC_DIR_LVGL}
    )

# target_compile_options(lvgl             PRIVATE -pedantic -Wpedantic)   # add libs extra compile options
# target_compile_options(lv_drivers       PRIVATE -pedantic -Wpedantic)

# add_subdirectory(${SRC_DIR_PROJ_UTILS}) no file in "proj_utils" so not add it for now
add_subdirectory(${SRC_DIR_APPUI})
add_subdirectory(${SRC_DIR_APP})

###### link libraries ######
target_link_libraries(${PROJECT_NAME} PRIVATE 
    # proj_utils
    appui
    app
)

##################################################################################################
## Install
##################################################################################################

if(UNIX)

    include(GNUInstallDirs)
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

endif()
