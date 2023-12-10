# Google Test (for unit tests)
if(MGL_BUILD_TESTS)
    add_subdirectory(gtest)
endif()

# SDL2
add_subdirectory(sdl2)

# mojoAL
add_subdirectory(mojoAL)

# spdlog
add_subdirectory(spdlog)

# glm
add_subdirectory(glm)

# glad
add_subdirectory(glad)
