
macro(define_target_option TARGET OPTION DEFINITION)
    if (${OPTION})
        set(_VALUE 1)
    else ()
        set(_VALUE 0)
    endif ()
    target_compile_options(${TARGET} PUBLIC -D${DEFINITION}=${_VALUE})
endmacro()