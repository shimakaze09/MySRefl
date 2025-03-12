IF (NOT MySRefl_Build_Parser)
    RETURN()
ENDIF ()

ADD_TARGET(
        MODE EXE
        LIB
        My::MyANTLR_ParserCpp14
)