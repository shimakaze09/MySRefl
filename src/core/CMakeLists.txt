SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030")
ENDIF ()

SET(libs "")
SET(defines "")
IF (MySRefl_UseNameof)
    LIST(APPEND libs nameof::nameof)
ELSE ()
    LIST(APPEND defines MY_MYSREFL_NOT_USE_NAMEOF)
ENDIF ()

ADD_TARGET(
        MODE INTERFACE
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
        LIB
        ${libs}
        DEFINE
        ${defines}
)