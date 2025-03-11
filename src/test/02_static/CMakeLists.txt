SET(c_options "")
IF (MSVC)
    list(APPEND c_options "/wd5030;/permissive-")
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        LIB
        My::MySRefl_core
        C_OPTION
        ${c_options}
)