# Set the path where you want to download Google Test
set(JSON_INSTALL_DIR "${CMAKE_BINARY_DIR}/json" CACHE PATH "Path to json installation directory")

# Use FetchContent to download json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3 
)

# FetchContent will download json and build it at configure time
FetchContent_MakeAvailable(json)

set(JSON_SOURCE_DIR "${json_SOURCE_DIR}" CACHE PATH "Path to json source directory")
set(JSON_INCLUDE_DIR "${JSON_SOURCE_DIR}/single_include" CACHE PATH "Path to json include directory")

install(TARGETS nlohmann_json
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)