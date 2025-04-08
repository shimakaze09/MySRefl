IF (NOT MySRefl_Build_AutoRefl)
    RETURN()
ENDIF ()

ADD_TARGET(
        CXX_STANDARD 17
        MODE EXE
        OUTPUT_NAME AutoRefl
        LIB
        My::MyANTLR_ParserCpp14
        INC
        "${PROJECT_SOURCE_DIR}/include"
)