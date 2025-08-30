option(FMT_DOC "Build documentation" OFF)
option(FMT_TEST "Build tests" OFF)
option(FMT_INSTALL "Enable installation of fmtlib" ON)
option(FMT_OS "Enable OS-specific functionality" ON)
option(FMT_EXCEPTIONS "Enable exception support" ON)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.2.1
)

FetchContent_MakeAvailable(fmt)