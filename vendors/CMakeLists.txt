# Google Test (for unit tests)
if(MGL_BUILD_TESTS)
    add_subdirectory(gtest)
endif()

# Header-only libraries
message(STATUS "Configuring header-only libraries")
add_subdirectory(headers_only)

# SDL2
message(STATUS "Configuring SDL2")
add_subdirectory(sdl2)

# mojoAL
message(STATUS "Configuring mojoAL")
add_subdirectory(mojoAL)

# spdlog
message(STATUS "Configuring spdlog")
add_subdirectory(simplelog)

# json
message(STATUS "Configuring json")
add_subdirectory(json)

# glm
message(STATUS "Configuring glm")
add_subdirectory(glm)

# glad
message(STATUS "Configuring glad")
add_subdirectory(glad)

# imgui
message(STATUS "Configuring imgui")
add_subdirectory(imgui)

# libzip
message(STATUS "Configuring libzip")
add_subdirectory(libzip)

# entt
message(STATUS "Configuring entt")
add_subdirectory(entt)
