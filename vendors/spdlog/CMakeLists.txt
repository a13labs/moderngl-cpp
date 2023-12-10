# Set the path where you want to download Google Test
set(SPDLOG_INSTALL_DIR "${CMAKE_BINARY_DIR}/spdlog" CACHE PATH "Path to spdlog installation directory")

# Use FetchContent to download spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.12.0 
)

# FetchContent will download spdlog and build it at configure time
FetchContent_MakeAvailable(spdlog)

set(SPDLOG_SOURCE_DIR "${spdlog_SOURCE_DIR}" CACHE PATH "Path to spdlog source directory")
set(SPDLOG_INCLUDE_DIR "${SPDLOG_SOURCE_DIR}/include" CACHE PATH "Path to spdlog include directory")