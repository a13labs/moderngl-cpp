# generate_shader_headers.cmake

find_package(Python3 REQUIRED)

# This script will be used to convert GLSL files to C arrays using xxd
set(SHADER_TYPES 
    vertex
    fragment
    CACHE STRING "Shader types to generate headers for"
)

function(get_shader_type_extension SHADER_TYPE SHADER_EXTENSION)
    if (${SHADER_TYPE} STREQUAL "vertex")
        set(${SHADER_EXTENSION} "vs" PARENT_SCOPE)
    elseif (${SHADER_TYPE} STREQUAL "fragment")
        set(${SHADER_EXTENSION} "fs" PARENT_SCOPE)
    elseif (${SHADER_TYPE} STREQUAL "geometry")
        set(${SHADER_EXTENSION} "gs" PARENT_SCOPE)
    elseif (${SHADER_TYPE} STREQUAL "tess_control")
        set(${SHADER_EXTENSION} "tcs" PARENT_SCOPE)
    elseif (${SHADER_TYPE} STREQUAL "tess_evaluation")
        set(${SHADER_EXTENSION} "tes" PARENT_SCOPE)
    elseif (${SHADER_TYPE} STREQUAL "compute")
        set(${SHADER_EXTENSION} "cs" PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Unknown shader type ${SHADER_TYPE}")
    endif()
endfunction()

function(generate_shader_header INPUT_SHADER SHADER_TYPE OUTPUT_HEADER)
    set(VARIABLE_NAME "${SHADER_TYPE}_shader_source")
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${MGL_SCRIPTS_DIR}/gen_header.py --name ${VARIABLE_NAME} ${INPUT_SHADER}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_FILE ${OUTPUT_HEADER}
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    file(READ ${OUTPUT_HEADER} FILE_CONTENT)
    file(WRITE ${OUTPUT_HEADER} "${FILE_CONTENT}")    
endfunction()

function(generate_shader SHADER_TYPE) 
    # Get the extension for the shader type
    get_shader_type_extension(${SHADER_TYPE} SHADER_EXTENSION)
    file(GLOB_RECURSE SHADER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER_TYPE}/*.${SHADER_EXTENSION}")
    # Create a directory to store generated headers
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/inc/shaders/${SHADER_TYPE}")

    # Generate header files for each shader using the custom script
    foreach(SHADER_SOURCE ${SHADER_SOURCES})
        get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME_WE)
        set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/inc/shaders/${SHADER_TYPE}/${SHADER_NAME}.hpp")
        generate_shader_header(${SHADER_SOURCE} ${SHADER_TYPE} ${SHADER_HEADER})
    endforeach()
endfunction()

function(generate_shader_headers)
    # Generate headers for each shader type
    message(STATUS "Generating shader headers")
    set(SHADER_HEADERS_TMP "")  
    foreach(SHADER_TYPE ${SHADER_TYPES})
        generate_shader(${SHADER_TYPE} SHADER_HEADERS_TMP)
    endforeach()
endfunction()