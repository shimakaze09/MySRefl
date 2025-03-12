IF (NOT MySRefl_Build_AutoRefl)
    RETURN()
ENDIF ()

ADD_TARGET(
        MODE EXE
        LIB
        My::MyANTLR_ParserCpp14
)