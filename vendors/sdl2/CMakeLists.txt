option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)
option(SDL_TESTS "" OFF)
option(SDL2_DISABLE_UNINSTALL "" ON)
option(SDL2_DISABLE_SDL2MAIN "" ON)
if (MGL_LINUX_OS)
    # Check for Wayland development libraries
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-protocols wayland-egl libdecor-0)
    
    if(NOT WAYLAND_FOUND)
        message(FATAL_ERROR "Wayland development libraries not found.")
    endif()

    # Check for X11 development libraries
    pkg_check_modules(X11 REQUIRED x11 xext xrandr)
    
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 development libraries not found.")
    endif()
    
    option(SDL_WAYLAND "" ON)
    option(SDL_X11 "" ON)
endif()

# Use FetchContent to download sdl2
FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.28.5
)

# FetchContent will download sdl2 and build it at configure time
FetchContent_MakeAvailable(sdl2)

set(SDL2_LIB_DEPENDS "${SDL2-static_LIB_DEPENDS}" CACHE PATH "Path to sdl2 dependencies")