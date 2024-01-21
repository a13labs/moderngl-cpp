# Google Test (for unit tests)
if(MGL_BUILD_TESTS)
    add_subdirectory(gtest)
endif()

# Header-only libraries
add_subdirectory(headers_only)

# SDL2
add_subdirectory(sdl2)

# mojoAL
add_subdirectory(mojoAL)

# spdlog
add_subdirectory(spdlog)

# json
add_subdirectory(json)

# glm
add_subdirectory(glm)

# glad
add_subdirectory(glad)

# imgui
add_subdirectory(imgui)

# physfs
add_subdirectory(libzip)

# entt
add_subdirectory(entt)
