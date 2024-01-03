# generate_shader_headers.cmake
# This script will be used to convert GLSL files to C arrays using xxd
function(generate_shader_header input output)
    get_filename_component(filename ${input} NAME_WE)
    execute_process(
        COMMAND xxd -i -n "${filename}_glsl" "${input}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_FILE ${output}
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endfunction()
