option(ENABLE_COMMONCRYPTO "Enable use of CommonCrypto" OFF)
option(ENABLE_GNUTLS "Enable use of GnuTLS" ON)
option(ENABLE_MBEDTLS "Enable use of mbed TLS" OFF)
option(ENABLE_OPENSSL "Enable use of OpenSSL" ON)
option(ENABLE_WINDOWS_CRYPTO "Enable use of Windows cryptography libraries" ON)

option(ENABLE_BZIP2 "Enable use of BZip2" OFF)
option(ENABLE_LZMA "Enable use of LZMA" OFF)
option(ENABLE_ZSTD "Enable use of Zstandard" OFF)

option(ENABLE_FDOPEN "Enable zip_fdopen, which is not allowed in Microsoft CRT secure libraries" ON)

option(BUILD_TOOLS "Build tools in the src directory (zipcmp, zipmerge, ziptool)" OFF)
option(BUILD_REGRESS "Build regression tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_DOC "Build documentation" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


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