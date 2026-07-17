function (target_conditional_sources project condition)
    foreach (input_file IN LISTS ARGN)
        if (NOT IS_ABSOLUTE ${input_file})
            set(input_file "${CMAKE_CURRENT_SOURCE_DIR}/${input_file}")
        endif ()
        
        set(output_file "${CMAKE_CURRENT_BINARY_DIR}/generated/${input_file}")
        add_custom_command(
            OUTPUT "${output_file}"
            COMMAND
            ${CMAKE_COMMAND}
            "-Dcondition=${condition}"
            "-Dinput_file=${input_file}"
            "-Doutput_file=${output_file}"
            -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/DefineWrapFile.cmake"
            DEPENDS "${input_file}"
            VERBATIM
        )
        target_sources(${project} PRIVATE "${output_file}")
    endforeach ()
endfunction ()
