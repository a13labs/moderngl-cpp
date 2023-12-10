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