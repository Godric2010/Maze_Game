include_guard(GLOBAL)

function(add_catch2_tests)
    # Usage:
    # add_catch2_tests(
    #   TARGET <name>
    #   SOURCES <list...>
    #   LINK <list ...>
    #   PREFIX <ecs.>
    #   DISABLE_REPORTS

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
    set(_EXTRA_ARGS
            --reporter;compact
    )

    if (NOT APP_DISABLE_REPORTS)
        if (NOT DEFINED APP_TEST_REPORT_DIR)
            set(APP_TEST_REPORT_DIR "${CMAKE_BINARY_DIR}/test-reports")
        endif ()
        file(MAKE_DIRECTORY "${APP_TEST_REPORT_DIR}")

        catch_discover_tests(${APP_TARGET}
                TEST_PREFIX "${APP_PREFIX}"
                EXTRA_ARGS ${_EXTRA_ARGS}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    else ()
        catch_discover_tests(${APP_TARGET}
                TEST_PREFIX "${APP_PREFIX}"
                EXTRA_ARGS ${_EXTRA_ARGS}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif ()

    #    set_tests_properties(${APP_PREFIX}${APP_TARGET} PROPERTIES TIMEOUT 15)
endfunction()