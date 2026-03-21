include_guard(GLOBAL)

option(TEST_JUNIT_REPORTS "Write Catch2 JUnit XML reports" OFF)
function(add_catch2_tests)
    # Usage:
    # add_catch2_tests(
    #   TARGET <name>
    #   SOURCES <list...>
    #   LINK <list ...>
    #   PREFIX <ecs.>


    set(opts)
    set(one TARGET PREFIX LINK WORKING_DIRECTORY)
    set(multi SOURCES)

    cmake_parse_arguments(APP "${opts}" "${one}" "${multi}" ${ARGN})

    if (NOT APP_TARGET)
        message(FATAL_ERROR "add_catch2_tests: TARGET is required")
    endif ()
    if (NOT APP_PREFIX)
        message(FATAL_ERROR "add_catch2_tests: PREFIX is required")
    endif ()
    if (NOT APP_LINK)
        message(FATAL_ERROR "add_catch2_tests: LINK (target lib) is required")
    endif ()
    if (NOT APP_SOURCES)
        message(FATAL_ERROR "add_catc2_tests: SOURCES are required")
    endif ()

    if (NOT BUILD_TESTING)
        return()
    endif ()

    add_executable(${APP_TARGET} ${APP_SOURCES})
    target_compile_features(${APP_TARGET} PRIVATE cxx_std_20)
    target_link_libraries(${APP_TARGET} PRIVATE Catch2::Catch2WithMain ${APP_LINK})

    include(Catch)

    if (COMMAND enable_coverage)
        if (TARGET ${APP_TARGET})
            enable_coverage(${APP_TARGET})
        endif ()
    endif ()

    set(_discover_args
            TEST_PREFIX "${APP_PREFIX}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            EXTRA_ARGS --reporter console
    )

    if (TEST_JUNIT_REPORTS)
        set(_JUNIT_DIR "${CMAKE_BINARY_DIR}/junit")
        file(MAKE_DIRECTORY "${_JUNIT_DIR}")

        list(APPEND _discover_args
                REPORTER junit
                OUTPUT_DIR "${_JUNIT_DIR}"
                OUTPUT_PREFIX "${APP_TARGET}"
                OUTPUT_SUFFIX ".xml"
        )
    endif ()

    catch_discover_tests(${APP_TARGET} ${_discover_args})
endfunction()