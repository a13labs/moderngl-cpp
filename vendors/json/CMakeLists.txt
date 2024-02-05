# Use FetchContent to download json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3 
)

# FetchContent will download json and build it at configure time
FetchContent_MakeAvailable(json)

install(TARGETS nlohmann_json
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)