# find unit tests in test directory and generate a test runner

function(find_unit_tests) 
    # Get all tests availables in the test folder and add the test
    file(GLOB GAMELIB_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")
    foreach(FILE_PATH ${GAMELIB_TESTS})
        # Get the filename without extension using get_filename_component
        get_filename_component(TEST_NAME ${FILE_PATH} NAME_WE)
  
        # Perform actions for each file, for example, displaying the file path and filename without extension
        message(STATUS "Adding unit test: ${TEST_NAME}")
  
        # Create the unit test executable
        add_executable(${PROJECT_NAME}_${TEST_NAME} ${FILE_PATH})

        # Link mgl_registry and gtest library to the unit test executable
        target_link_libraries(
            ${PROJECT_NAME}_${TEST_NAME} 
          PRIVATE
            ${ARGV}
            gtest
          )
  
          # Add a custom test target to run the unit tests
        add_test(NAME ${PROJECT_NAME}_ctest_${TEST_NAME} COMMAND ${PROJECT_NAME}_${TEST_NAME})
    endforeach()

    add_custom_target(${PROJECT_NAME}_tests_data ALL)
    # Get all tests data available in the test folder and copy it
    file(GLOB_RECURSE TESTS_DATA "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/*.*")
    set(TESTS_DATA_FOLDER "${CMAKE_CURRENT_BINARY_DIR}")
    foreach(FILE_PATH ${TESTS_DATA})
        # Get the relative path of the file
        file(RELATIVE_PATH RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests ${FILE_PATH})
        # Get the destination folder path
        get_filename_component(DESTINATION_FOLDER "${TESTS_DATA_FOLDER}/${RELATIVE_PATH}" DIRECTORY)
        # Create the destination folder
        file(MAKE_DIRECTORY ${DESTINATION_FOLDER})
        # Copy the file to the destination folder
        add_custom_command(
            TARGET ${PROJECT_NAME}_tests_data
            POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            ARGS -E copy  ${FILE_PATH} "${DESTINATION_FOLDER}"
        )
    endforeach(FILE_PATH)
endfunction(find_unit_tests)