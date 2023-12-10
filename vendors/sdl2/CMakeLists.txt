option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)
option(SDL_TESTS "" OFF)
if (MGL_LINUX_OS)
    option(SDL_WAYLAND "" ON)
    option(SDL_X11 "" OFF)
endif()


# Set the path where you want to download Google Test
set(SDL2_INSTALL_DIR "${CMAKE_BINARY_DIR}/sdl2" CACHE PATH "Path to sdl2 installation directory")

# Use FetchContent to download sdl2
FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.28.5
)

# FetchContent will download sdl2 and build it at configure time
FetchContent_MakeAvailable(sdl2)

set(SDL2_BINARY_DIR "${sdl2_BINARY_DIR}" CACHE PATH "Path to sdl2 binary directory")
set(SDL2_INCLUDE_DIR "${SDL2_SOURCE_DIR}/include" CACHE PATH "Path to sdl2 include directory")

message(STATUS "SDL2_SOURCE_DIR: ${SDL2_SOURCE_DIR}")
message(STATUS "SDL2_BINARY_DIR: ${SDL2_BINARY_DIR}")
message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
