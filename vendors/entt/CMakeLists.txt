# Set the path where you want to download Google Test
set(ENTT_INSTALL_DIR "${CMAKE_BINARY_DIR}/entt" CACHE PATH "Path to entt installation directory")

# Use FetchContent to download entt
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.12.2 
)

# FetchContent will download entt and build it at configure time
FetchContent_MakeAvailable(entt)

set(ENTT_SOURCE_DIR "${entt_SOURCE_DIR}" CACHE PATH "Path to entt source directory")
set(ENTT_INCLUDE_DIR "${ENTT_SOURCE_DIR}/single_include" CACHE PATH "Path to entt include directory")