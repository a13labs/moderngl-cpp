project(mgl_core)
                    
# Add the C source files for the library
file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create the static library target "mgl_core"
add_library(mgl_core_static STATIC ${LIB_SOURCES})
add_library(mgl::core::static ALIAS mgl_core_static)

set(MGL_CORE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE PATH "Path to mgl::core include directory")

# Add include directories
target_include_directories(
    mgl_core_static 
  PUBLIC
    $<BUILD_INTERFACE:${GLM_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${MGL_CORE_INCLUDE_DIR}>
  PRIVATE 
    ${SIMPLELOG_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${LIBZIP_INCLUDE_DIR}
    ${LIBZIP_BINARY_DIR}
    ${JSON_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}/inc
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