project(mgl_window)

# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_window"
add_library(mgl_window STATIC ${LIB_SOURCES})
add_library(mgl::window::static ALIAS mgl_window)

set(MGL_WINDOW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::window include directory")

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${SDL2_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${LIBZIP_INCLUDE_DIR}
    ${LIBZIP_BINARY_DIR}
    ${MGL_WINDOW_INCLUDE_DIR}
    ${MGL_REGISTRY_INCLUDE_DIR}
)

# Add include directories
target_include_directories(mgl_window PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link library to required libraries
target_link_libraries(mgl_window PRIVATE mgl::core::static mgl::opengl::static mgl::registry::static)

# Link library to required external libraries
target_link_libraries(
        mgl_window 
    PRIVATE 
        $<TARGET_OBJECTS:SDL2::SDL2-static>
        imgui::static
)

# compiler and OS definitions
target_compile_definitions(mgl_window 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
    ${MGL_WINDOW_DEFINITIONS}
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
        add_executable(mgl_window_${TEST_NAME} ${FILE_PATH})
        # Link mgl_window and Check library to the unit test executable
        target_link_libraries(mgl_window_${TEST_NAME} PRIVATE mgl_window gtest)
        target_include_directories(mgl_window_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_window_ctest_${TEST_NAME} COMMAND mgl_window_${TEST_NAME})
    endforeach()
endif()

# Add the install target
install(TARGETS mgl_window
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)