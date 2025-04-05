CMAKE_MINIMUM_REQUIRED(VERSION 3.20 FATAL_ERROR)

PROJECT(MySRefl VERSION 0.8.0 LANGUAGES CXX)
MESSAGE(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")

INCLUDE(cmake/InitMyCMake.cmake)
INCLUDE(cmake/MySReflAutoRefl.cmake)

INIT_MYCMAKE()

INIT_PROJECT()

OPTION(MySRefl_Build_AutoRefl "Build AutoRefl" OFF)
IF (MySRefl_Build_AutoRefl)
    ADD_DEP(MyANTLR 0.2.0)
ENDIF ()

ADD_DEP(MyTemplate 0.5.3)

ADD_SUB_DIRS_REC(include)
ADD_SUB_DIRS_REC(src)

SET(INSTALL_FILES "")
IF (MySRefl_Build_AutoRefl)
    LIST(APPEND INSTALL_FILES "cmake/MySReflAutoRefl.cmake")
ENDIF ()

INSTALL(
        FILES
        ${INSTALL_FILES}
        DESTINATION "${PACKAGE_NAME}/cmake"
)

EXPORT_TARGETS(
        TARGET
        DIRECTORIES
        "include"
)