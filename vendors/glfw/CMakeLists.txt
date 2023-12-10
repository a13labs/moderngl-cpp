option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_INSTALL "" OFF)
if (MGL_LINUX_OS)
    option(GLFW_USE_WAYLAND "" ON)
endif()

# Set the path where you want to download Google Test
set(GLFW_INSTALL_DIR "${CMAKE_BINARY_DIR}/glfw" CACHE PATH "Path to glfw installation directory")

# Use FetchContent to download glfw
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8 
)

# FetchContent will download glfw and build it at configure time
FetchContent_MakeAvailable(glfw)

set(GLFW_SOURCE_DIR "${glfw_SOURCE_DIR}" CACHE PATH "Path to glfw source directory")
