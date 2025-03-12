FUNCTION(MYSREFL_AUTO_REFL SRC DST)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${DST}
            COMMAND My::MySRefl_AutoRefl ARGS ${SRC} ${DST}
            DEPENDS My::MySRefl_AutoRefl
    )
ENDFUNCTION()