project(basic_application_example)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# generate shader headers
generate_shader_headers()

# Create the executable target "window"
add_executable(basic_application_example ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}/inc
)

# Add include directories
include_directories(basic_application_example PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(basic_application_example PRIVATE mgl::static)
