IF (NOT MySRefl_Build_AutoRefl)
    RETURN()
ENDIF ()

MYSREFL_AUTO_REFL(
        "${CMAKE_CURRENT_SOURCE_DIR}/Vec.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Vec_AutoRefl.inl"
)

SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030;/permissive-")
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/Vec_AutoRefl.inl"
        LIB
        My::MySRefl_core
        C_OPTION
        ${c_options}
)