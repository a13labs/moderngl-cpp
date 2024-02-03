project(mgl_core)
                    
# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_core"
add_library(mgl_core_static STATIC ${LIB_SOURCES})
add_library(mgl::core::static ALIAS mgl_core_static)

set(MGL_CORE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::core include directory")

set (DEFAULT_INCLUDE_DIRS 
    ${SIMPLELOG_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${LIBZIP_INCLUDE_DIR}
    ${LIBZIP_BINARY_DIR}
    ${JSON_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${MGL_CORE_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}/inc
)

# Add include directories
target_include_directories(
    mgl_core_static 
PRIVATE 
    ${DEFAULT_INCLUDE_DIRS}
)

# Link library to required libraries
target_link_libraries(
    mgl_core_static 
    PRIVATE 
    $<TARGET_OBJECTS:simplelog::static>
    nlohmann_json::nlohmann_json
    libzip::zip 
)

# compiler and OS definitions
target_compile_definitions(mgl_core_static 
    PRIVATE 
    ${MGL_COMPILER_DEFINITION} 
    ${MGL_BUILD_TYPE_DEFINITIONS}
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
            add_executable(mgl_core_${TEST_NAME} WIN32 ${FILE_PATH})
        else()
            add_executable(mgl_core_${TEST_NAME} ${FILE_PATH})
        endif()

        # Link mgl_core and Check library to the unit test executable
        target_link_libraries(mgl_core_${TEST_NAME} PRIVATE mgl_core_static gtest)
        target_include_directories(mgl_core_${TEST_NAME} PRIVATE 
                        ${DEFAULT_INCLUDE_DIRS} 
                        ${GTEST_INSTALL_DIR}/include)
        # Add a custom test target to run the unit tests
        add_test(NAME mgl_core_ctest_${TEST_NAME} COMMAND mgl_core_${TEST_NAME})
    endforeach()

    add_custom_target(mgl_core_test_data ALL)
    # Get all tests data available in the test folder and copy it
    file(GLOB GAMELIB_TESTS_DATA "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.*")
    set(TESTS_DATA_FOLDER "${CMAKE_CURRENT_BINARY_DIR}")
    foreach(FILE_PATH ${GAMELIB_TESTS_DATA})
        add_custom_command(
            TARGET mgl_core_test_data
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            ARGS -E copy ${FILE_PATH} "${TESTS_DATA_FOLDER}"
        )
    endforeach(FILE_PATH)    
endif()

install(TARGETS mgl_core_static
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)