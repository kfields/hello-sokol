include_guard()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Standard includes
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)

set(ATT_STATIC_PIC ON)

#
# Directories
#
set(ATT_ROOT ${CMAKE_SOURCE_DIR})

set(SDL_ROOT ${ATT_ROOT}/3rdparty/sdl)
set(GLM_ROOT ${ATT_ROOT}/3rdparty/glm)
set(GLAD_ROOT ${ATT_ROOT}/glad)
set(SOKOL_ROOT ${ATT_ROOT}/3rdparty/sokol)

#
# Platform
#
set(ATT_PLATFORM ${CMAKE_SYSTEM_NAME})

set(ATT_PLATFORM_WINDOWS OFF)
set(ATT_PLATFORM_LINUX OFF)

if(${ATT_PLATFORM} STREQUAL "Windows")
    set(ATT_PLATFORM_WINDOWS ON)
elseif(${ATT_PLATFORM} STREQUAL "Linux")
    set(ATT_PLATFORM_LINUX ON)
endif()

#
# Renderer
#

# option(ATT_RENDERER_GL "Use GL" OFF)
# cmake_dependent_option(ATT_RENDERER_VULKAN "Use Vulkan" ON
# "ATT_PLATFORM_LINUX; NOT ATT_RENDERER_GL" OFF)
set(ATT_RENDERER_GL ON)
set(ATT_RENDERER_VULKAN OFF)

option(ATT_WM_WAYLAND "Use Wayland" OFF)
cmake_dependent_option(ATT_WM_X11 "Use X11" ON "ATT_PLATFORM_LINUX; NOT ATT_WM_WAYLAND" OFF)

set(ATT_COMPILE_DEFS
    NOMINMAX=1
)

if(${ATT_WM_X11})
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_X11=1
    )
elseif(${ATT_WM_WAYLAND})
    set(ATT_COMPILE_DEFS ${ATT_COMPILE_DEFS}
        SDL_VIDEO_DRIVER_WAYLAND=1
    )
endif()
