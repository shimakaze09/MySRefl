CMAKE_MINIMUM_REQUIRED(VERSION 3.20 FATAL_ERROR)

PROJECT(MySRefl VERSION 0.7.2 LANGUAGES CXX)
MESSAGE(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")

INCLUDE(cmake/InitMyCMake.cmake)
INCLUDE(cmake/MySReflAutoRefl.cmake)

INIT_MYCMAKE()

INIT_PROJECT()

OPTION(MySRefl_Build_AutoRefl "Build AutoRefl" OFF)
IF (MySRefl_Build_AutoRefl)
    ADD_DEP(MyANTLR 0.2.0)
ENDIF ()

SET(OPTION_CPM "")

OPTION(MySRefl_UseNameof "not use nameof" OFF)
IF (MySRefl_UseNameof)
    SET(OPTION_CPM "CPM")
    CPMAddPackage(
            NAME nameof
            GITHUB_REPOSITORY Neargye/nameof
            GIT_TAG 0e8bec0
    )
ENDIF ()

ADD_SUB_DIRS_REC(include)
ADD_SUB_DIRS_REC(src)

SET(INSTALL_FILES "cmake/InitMyCMake.cmake")
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
        ${OPTION_CPM}
        DIRECTORIES
        "include"
        "cmake"
)