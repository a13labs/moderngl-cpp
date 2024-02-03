project(mgl_opengl)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_opengl"
add_library(mgl_opengl_static STATIC ${LIB_SOURCES})
add_library(mgl::opengl::static ALIAS mgl_opengl_static)

set(MGL_OPENGL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::opengl include directory")

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    mgl_opengl_static 
    PRIVATE 
    ${DEFAULT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

# Link library to required libraries
target_link_libraries(
        mgl_opengl_static 
    PRIVATE 
        mgl::core::static 
        glm::static
        $<TARGET_OBJECTS:glad::static>
        ${OPENGL_LIBRARIES}
)

if(WIN32)
    message(STATUS "OpenGL Platform: Windows")
    set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_WGL")
elseif (UNIX)
    if (NOT APPLE)
        set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_EGL")
    else()
        message(STATUS "OpenGL Platform: MacOS")
        set(MGL_OPENGL_PLATFORM_DEFINITION "-DMGL_OPENGL_CGL")
    endif()
endif()

# compiler and OS definitions
target_compile_definitions(mgl_opengl_static 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_OPENGL_PLATFORM_DEFINITION}
)

if (MGL_BUILD_TESTS)
    # Get all tests availables in the test folder and add the test
    file(GLOB GAMELIB_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")
    foreach(FILE_PATH ${GAMELIB_TESTS})
        # Get the filename without extension using get_filename_component
        get_filename_component(TEST_NAME ${FILE_PATH} NAME_WE)
        # Perform actions for each file, for example, displaying the file path and filename without extension
        message(STATUS "GameLib Test found: ${TEST_NAME}")
        # Create the unit test executable
        if (WIN32)
            add_executable(mgl_opengl_${TEST_NAME} WIN32 ${FILE_PATH})
        else()
            add_executable(mgl_opengl_${TEST_NAME} ${FILE_PATH})
        endif()
        # Link mgl_opengl and Check library to the unit test executable
        target_link_libraries(mgl_opengl_${TEST_NAME} PRIVATE mgl_opengl_static gtest)
        target_include_directories(mgl_opengl_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_opengl_ctest_${TEST_NAME} COMMAND mgl_opengl_${TEST_NAME})
    endforeach()
endif()

install(TARGETS mgl_opengl_static
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
)