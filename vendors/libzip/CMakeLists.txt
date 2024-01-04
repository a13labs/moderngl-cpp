# Set the path where you want to download Google Test
set(LIBZIP_INSTALL_DIR "${CMAKE_BINARY_DIR}/libzip" CACHE PATH "Path to libzip installation directory")

# Use FetchContent to download libzip
FetchContent_Declare(
    libzip
    GIT_REPOSITORY https://github.com/nih-at/libzip.git
    GIT_TAG        v1.10.1
)

# FetchContent will download libzip and build it at configure time
FetchContent_MakeAvailable(libzip)

set(LIBZIP_BINARY_DIR "${libzip_BINARY_DIR}" CACHE PATH "Path to libzip binary directory")
set(LIBZIP_INCLUDE_DIR "${libzip_SOURCE_DIR}/lib" CACHE PATH "Path to libzip include directory")