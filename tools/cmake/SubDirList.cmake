macro(SubDirList curdir result)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)

    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND ${result} ${child})
        endif()
    endforeach()
endmacro()
