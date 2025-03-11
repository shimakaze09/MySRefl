SET(c_options "")
IF (MSVC)
    LIST(APPEND c_options "/wd5030;/permissive-")
ENDIF ()

ADD_TARGET(
        TEST
        MODE EXE
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MySRefl_99.h"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        C_OPTION
        ${c_options}
)