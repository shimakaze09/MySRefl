IF (NOT MySRefl_Build_AutoRefl)
    RETURN()
ENDIF ()

ADD_TARGET(
        MODE EXE
        OUTPUT_NAME AutoRefl
        LIB
        My::MyANTLR_ParserCpp14
)