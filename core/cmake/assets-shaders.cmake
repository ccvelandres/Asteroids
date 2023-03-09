# Copyright (c) 2022, Cedric Velandres
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

if(__CORE_ASSETS_CMAKE_INCLUDE_GUARD__)
    return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/assets-common.cmake)

function(core_compile_shader_vulkan in_file out_file ret)
    # we do not support vulkan shaders yet
    # needs some fixing on the graphics system
endfunction()

function(core_compile_shader_opengl in_file out_file ret)
    # @todo: add packaging here?
    get_filename_component(_outdir ${out_file} DIRECTORY)
    file(MAKE_DIRECTORY ${_outdir})

    # For now just copy shader sources to out directory
    add_custom_command(OUTPUT "${out_file}"
        MAIN_DEPENDENCY "${in_file}"
        COMMAND ${CMAKE_COMMAND} -E copy "${in_file}" "${out_file}")

    set(${ret} TRUE PARENT_SCOPE)
endfunction()

function(core_compile_shader type in_file out_file ret)
    message(DEBUG "core_compile_shader")
    message(DEBUG "  type:      ${type}")
    message(DEBUG "  in_file:   ${in_file}")
    message(DEBUG "  out_file:  ${out_file}")
    string(TOLOWER "${type}" type)

    if(COMMAND "core_compile_shader_${type}")
        cmake_language(CALL core_compile_shader_${type} "${in_file}" "${out_file}" _ret)
    else()
        message(FATAL_ERROR "Invalid shader type. Valid values are (${shader_types})")
    endif()

    set(${ret} ${_ret} PARENT_SCOPE)
endfunction()

function(core_process_asset_shaders path assetlist)
    core_get_asset_directory(_asset_bindir)
    set(shader_outpath "${_asset_bindir}/shaders")
    file(MAKE_DIRECTORY ${shader_outpath})
    unset(shader_list)

    # Shaders must be grouped by graphics api
    # ie. shaders/<opengl | vulkan>/shader.vert, etc

    # For now we only support opengl shaders
    set(shader_api "opengl")

    file(GLOB shader_subdir LIST_DIRECTORIES true ${path}/*)
    foreach(subdir ${shader_subdir})
        get_filename_component(_subdir ${subdir} NAME )
        string(TOLOWER "${_subdir}" _subdir)
        if(COMMAND "core_compile_shader_${_subdir}")
            file(GLOB_RECURSE shaders ${subdir}/**)
            foreach(shader ${shaders})
                unset(_ret)
                file(RELATIVE_PATH _relpath "${path}" "${shader}")
                set(outfile "${shader_outpath}/${_relpath}")
                core_compile_shader("${_subdir}" ${shader} "${outfile}" _ret)
                if(_ret)
                    list(APPEND shader_list "${outfile}")
                endif()
            endforeach()
        else()
            message(WARNING "No handler for shader type: ${subdir}")
        endif()
    endforeach()

    set(${assetlist} ${shader_list} PARENT_SCOPE)
endfunction()