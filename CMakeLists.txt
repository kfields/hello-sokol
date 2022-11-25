cmake_minimum_required(VERSION 3.4)

set(THIS HelloSokol)
project(${THIS} LANGUAGES CXX)

include(./cmake/Config.cmake)

#
# fmt
#
add_subdirectory(3rdparty/fmt)

#
# GLM
#
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library" FORCE)

add_subdirectory(3rdparty/glm)

#
# SDL - Minimal build for now
#
set(HAVE_LIBC ON)
set(SDL_TEST OFF CACHE BOOL "Build the SDL2_test library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
#set(SDL_STATIC_PIC ON CACHE BOOL "Static version of the library should be built with Position Independent Code" FORCE)

set(SDL_RENDER OFF CACHE BOOL "Use render subsystem" FORCE)
set(SDL_AUDIO OFF CACHE BOOL "Use audio subsystem" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "Use haptic subsystem" FORCE)
set(HIDAPI OFF CACHE BOOL "Use HIDAPI for low level joystick drivers" FORCE)

set(VIDEO_X11 ${ATT_WM_X11} CACHE BOOL "Enable X11 support" FORCE)
set(VIDEO_WAYLAND ${ATT_WM_WAYLAND} CACHE BOOL "Enable Wayland support" FORCE)

add_subdirectory(3rdparty/sdl)

#
# Glad
#
if(${ATT_RENDERER_GL})
    add_subdirectory(glad)
endif()

add_executable(${THIS} "main.cpp")

include(./cmake/Fmt.cmake)
USES_FMT(${THIS})
include(./cmake/Sdl.cmake)
USES_SDL(${THIS})
include(./cmake/Glad.cmake)
USES_GLAD(${THIS})
include(./cmake/Sokol.cmake)
USES_SOKOL(${THIS})
