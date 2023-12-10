# Google Test (for unit tests)
if(MGL_BUILD_TESTS)
    # Set the path where you want to download Google Test
    set(GTEST_INSTALL_DIR "${CMAKE_BINARY_DIR}/googletest" CACHE PATH "Path to Google Test installation directory")

    # Use FetchContent to download Google Test
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0 
    )

    # FetchContent will download Google Test and build it at configure time
    FetchContent_MakeAvailable(googletest)
endif()

# physfs
option(PHYSFS_ARCHIVE_7Z OFF)
option(PHYSFS_ARCHIVE_GRP OFF)
option(PHYSFS_ARCHIVE_WAD OFF)
option(PHYSFS_ARCHIVE_HOG OFF)
option(PHYSFS_ARCHIVE_MVL OFF)
option(PHYSFS_ARCHIVE_QPAK OFF)
option(PHYSFS_ARCHIVE_SLB OFF)
option(PHYSFS_ARCHIVE_ISO9660 OFF)
option(PHYSFS_ARCHIVE_VDF OFF)
option(PHYSFS_ARCHIVE_VDF OFF)
option(PHYSFS_BUILD_SHARED OFF)
option(PHYSFS_BUILD_TEST OFF)
option(PHYSFS_BUILD_DOCS OFF)
add_subdirectory(physfs)

# # glfw
# option(GLFW_BUILD_EXAMPLES "" OFF)
# option(GLFW_BUILD_TESTS "" OFF)
# option(GLFW_BUILD_DOCS "" OFF)
# option(GLFW_INSTALL "" OFF)
# option(GLFW_USE_WAYLAND "" ON)
# add_subdirectory(glfw)

# mojoAL
add_subdirectory(mojoAL)

# spdlog
add_subdirectory(spdlog)

# glm
add_subdirectory(glm)

# glad
add_subdirectory(glad)
