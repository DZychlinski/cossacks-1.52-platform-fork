# Copy every file from SRC into DST unless DST already has it (keeps files the game rewrote).
# Usage: cmake -DSRC=<dir> -DDST=<dir> -P copy_missing.cmake
file(GLOB _files RELATIVE "${SRC}" "${SRC}/*")
foreach(_f IN LISTS _files)
    if(NOT IS_DIRECTORY "${SRC}/${_f}" AND NOT EXISTS "${DST}/${_f}")
        file(COPY "${SRC}/${_f}" DESTINATION "${DST}")
    endif()
endforeach()
