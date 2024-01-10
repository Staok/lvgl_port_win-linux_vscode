# 构建 & 编译 & 运行，使用 git bash 运行下述命令:

# 创建并进入 build 文件夹 
# mkdir build && cd build

# 在 build 文件夹里面 执行（每次改变 option 后需要全清 build 目录重新执行下述命令）：
# cmake -G "Unix Makefiles" ..
# cmake --build . -j16 && ./lvgl_proj.exe

# linux 下执行，需要通过命令设置 编译器完整路径，设置变量不行
# cmake -G "Unix Makefiles" .. -DCMAKE_C_COMPILER=/home/haoyang.xu/rk/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=/home/haoyang.xu/rk/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++

# 清理 重新编译
# cmake --build . --target 'clean'

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
    # use sdl2 need pre-install sdl2 lib in system
    option(USE_WIN_SDL2LIB      "in win platform use sdl2 lib as disp and indev drv"                OFF  )
    option(USE_WIN_WINDRV       "in win platform use win_drv in lv_drivers as disp and indev drv"   ON )
elseif(UNIX)
    # use drm need pre-install libdrm in system
    option(USE_LINUX_DRM        "in linux platform use drm as display method"                       ON  )
    option(USE_LINUX_FB         "in linux platform use fb as display method"                        OFF )
endif()

option(USE_DEMOS_EXAMPLES       "use demos and examples"                                            ON  )

option(COMPILE_DISTINGUISH_COMPILERS "Distinguish compilers and perform special flags"              OFF )
option(COMPILE_WERROR           "Turn compiler warnings into errors"                                OFF )
option(COMPILE_DEBUG            "Turn on debug compile"                                             OFF )

###### compile flags .etc
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 基本编译选项，lvgl 库过不了 -pedantic -Wpedantic 选项
set(CMAKE_C_CXX_FLAGS_PRE "-O2 -Wall -Wextra")

if(COMPILE_DISTINGUISH_COMPILERS)
    MESSAGE(STATUS "### option: Distinguish compilers and perform special flags")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_C_CXX_FLAGS_PRE "${CMAKE_C_CXX_FLAGS_PRE} -Wno-psabi")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
    endif()
endif()

if(COMPILE_WERROR)
    MESSAGE(STATUS "### option: Turn compiler warnings into errors")
    # set(CMAKE_C_CXX_FLAGS_PRE "${CMAKE_C_CXX_FLAGS_PRE} -Werror") lvgl 库过不了 -Werror 选项
endif()

if(COMPILE_DEBUG)
    MESSAGE(STATUS "### option: Turn on debug compile")
    set(CMAKE_C_CXX_FLAGS_PRE "${CMAKE_C_CXX_FLAGS_PRE} -g -ggdb")
endif()

###### add flags to compiler

# for this proj
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_CXX_FLAGS_PRE}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${CMAKE_C_CXX_FLAGS_PRE}")

# for all proj
# add_compile_options(${CMAKE_C_CXX_FLAGS_PRE}) 里面多于两个选项就报错

MESSAGE(STATUS "### CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "### CMAKE_C_FLAGS   " ${CMAKE_C_FLAGS})

###### add defines to program
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE) # lvgl 用
add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

### platform dependent
if(WIN32)
    if(USE_WIN_SDL2LIB)
        if(USE_WIN_WINDRV)
            message(FATAL_ERROR "Can not run when both set: USE_WIN_SDL2LIB and USE_WIN_WINDRV.")
        endif()
        MESSAGE(STATUS "### in win platform use sdl2 lib as disp and indev drv")
        add_compile_definitions(USE_SDL=1)
    endif()
    if(USE_WIN_WINDRV)
        if(USE_WIN_SDL2LIB)
            message(FATAL_ERROR "Can not run when both set: USE_WIN_SDL2LIB and USE_WIN_WINDRV.")
        endif()
        MESSAGE(STATUS "### in win platform use win_drv in lv_drivers as disp and indev drv")
        add_compile_definitions(USE_WINDOWS=1)
    endif()
endif()

if(UNIX)
    add_compile_definitions(USE_EVDEV=1)
    if(USE_LINUX_DRM)
        if(USE_LINUX_FB)
            message(FATAL_ERROR "Can not run when both set: USE_LINUX_DRM and USE_LINUX_FB.")
        endif()
        MESSAGE(STATUS "### in linux platform use drm as display method")
        add_compile_definitions(USE_DRM=1)
    endif()
    if(USE_LINUX_FB)
        if(USE_LINUX_DRM)
            message(FATAL_ERROR "Can not run when both set: USE_LINUX_DRM and USE_LINUX_FB.")
        endif()
        MESSAGE(STATUS "### in linux platform use fb as display method")
        add_compile_definitions(USE_FBDEV=1)
    endif()
endif()

if(USE_DEMOS_EXAMPLES)
    MESSAGE(STATUS "### use demos and examples")
    add_compile_definitions(LV_USE_DEMOS=1 LV_BUILD_EXAMPLES=1)
endif()

##################################################################################################
## Source
##################################################################################################

MESSAGE(STATUS "### PROJECT_SOURCE_DIR       "  ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "### CMAKE_CURRENT_SOURCE_DIR "  ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_FILE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/screen/main.c
)

##################################################################################################
## Include
##################################################################################################

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/screen
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/screen/lvgl
    ${CMAKE_CURRENT_SOURCE_DIR}/screen/lvgl/demos
    ${CMAKE_CURRENT_SOURCE_DIR}/screen/lvgl/examples
    ${CMAKE_CURRENT_SOURCE_DIR}/screen/ui
)

##################################################################################################
## src compile & link libs
##################################################################################################

add_executable(${PROJECT_NAME}
    ${SRC_FILE_LIST}
)

add_subdirectory(utils)
add_subdirectory(screen/lvgl)
add_subdirectory(screen/lv_drivers)
add_subdirectory(screen/ui)

target_link_libraries(${PROJECT_NAME} PRIVATE lvgl_utils)
target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lv_drivers ui)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

if(USE_DEMOS_EXAMPLES)
    target_link_libraries(${PROJECT_NAME} PRIVATE lvgl_examples lvgl_demos)
endif()

if(WIN32)
    if(USE_WIN_SDL2LIB)
        target_link_libraries(${PROJECT_NAME} PRIVATE mingw32)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
    endif()
elseif(UNIX)
endif()

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
