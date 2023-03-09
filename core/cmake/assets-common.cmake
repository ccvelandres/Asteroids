# Copyright (c) 2022, Cedric Velandres
# SPDX-License-Identifier: MIT

include_guard(GLOBAL)

if(__CORE_ASSETS_CMAKE_INCLUDE_GUARD__)
    return()
endif()

function(core_set_asset_directory path)
    message(STATUS "Setting asset build directory: ${path}")

    set_property(GLOBAL PROPERTY CORE_ASSET_BINARY_DIR ${path})
endfunction()

function(core_get_asset_directory path)
    get_property(_asset_bindir GLOBAL PROPERTY CORE_ASSET_BINARY_DIR)
    set(${path} ${_asset_bindir} PARENT_SCOPE)
endfunction()

function(core_add_asset_directory path)
    message(STATUS "Adding asset directory: ${path}")

    set_property(GLOBAL APPEND PROPERTY CORE_ASSET_DIRECTORIES ${path})
endfunction()