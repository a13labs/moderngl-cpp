project(mgl_core)
                    
# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_core"
add_library(mgl_core_static STATIC ${LIB_SOURCES})
add_library(mgl::core::static ALIAS mgl_core_static)

# Add include directories
target_include_directories(
    mgl_core_static 
  PUBLIC
    $<TARGET_PROPERTY:glm::static,INTERFACE_INCLUDE_DIRECTORIES>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  PRIVATE     
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/inc
    ${CMAKE_BINARY_DIR}/inc
    $<TARGET_PROPERTY:simplelog::static,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:libzip::zip,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
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
    find_unit_tests(mgl_core_static)    
endif()

install(TARGETS mgl_core_static
    EXPORT mgl-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION inc
)