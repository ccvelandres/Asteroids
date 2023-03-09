# Copyright (c) 2022, Cedric Velandres
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

if(__CORE_ASSETS_CMAKE_INCLUDE_GUARD__)
    return()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/assets-common.cmake)

function(core_process_asset_audio path)
    core_get_asset_directory(_asset_bindir)
    file(COPY ${path} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
endfunction()

function(core_process_asset_env path)
    core_get_asset_directory(_asset_bindir)
    file(COPY ${path} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
endfunction()

function(core_process_asset_fonts path)
    core_get_asset_directory(_asset_bindir)
    file(COPY ${path} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
endfunction()

function(core_process_asset_models path)
    core_get_asset_directory(_asset_bindir)
    file(COPY ${path} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
endfunction()

function(core_process_asset_spritesheets path)
    core_get_asset_directory(_asset_bindir)
    file(COPY ${path} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
endfunction()

include(${CMAKE_CURRENT_LIST_DIR}/assets-shaders.cmake)

function(core_process_asset_textures path)
    core_get_asset_directory(_asset_bindir)
    file(COPY ${path} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
endfunction()

function(core_process_assets)
    message(STATUS "Processing assets...")

    get_property(_core_asset_dirs GLOBAL PROPERTY CORE_ASSET_DIRECTORIES)
    
    unset(assetlist)
    set(assetSubdirTypes audio env fonts models spritesheets shaders textures)

    # make the binary asset dir
    core_get_asset_directory(_asset_bindir)
    file(MAKE_DIRECTORY ${_asset_bindir})

    foreach(assetDir ${_core_asset_dirs})
        message(DEBUG "  -- ${assetDir}")
        file(GLOB assetSubdir LIST_DIRECTORIES true ${assetDir}/*)
        foreach(asset ${assetSubdir})
            unset(_assetlist)
            get_filename_component(_basename ${asset} NAME)
            message(DEBUG "  -- ${asset}")
            if(_basename IN_LIST assetSubdirTypes)
                cmake_language(CALL core_process_asset_${_basename} ${asset} _assetlist)
            else()
                file(COPY ${asset} DESTINATION ${_asset_bindir} FOLLOW_SYMLINK_CHAIN)
            endif()
            list(APPEND assetlist ${_assetlist})
        endforeach()
    endforeach()

    # Create target for assets
    add_custom_target(core_assets DEPENDS ${assetlist})
endfunction()

define_property(GLOBAL PROPERTY CORE_ASSET_DIRECTORIES BRIEF_DOCS "List of asset directories")

cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL core_process_assets)