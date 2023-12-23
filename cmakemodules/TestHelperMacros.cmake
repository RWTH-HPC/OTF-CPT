########################################################################
#
# Creates a check target that executes lit test suites.
#
# Signature:
# ```
# add_check_target(
#   SUITES lit_test_path1 [lit_test_path2 ...]
#   [NAME target_name]
#   [COMMENT comment]
#   )
# ```
#
# The options are:
# - `SUITES`: Required. One or more paths of lit test suites.
# - `NAME`: The name of the target. Defaults to `check-<lit_test_path1>`.
# - `COMMENT`: The comment displayed by the target.
#
########################################################################

function(add_check_target)
    # Let CMakes's parser do the heavy lifting
    set(oneValueArgs NAME COMMENT)
    set(multiValueArgs SUITES DEPENDS)
    cmake_parse_arguments(PARSE_ARGV 0 ADD_CHECK_TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}")

    # Again some input validation. Devs cannot be trusted.
    list(LENGTH ADD_CHECK_TARGET_SUITES suiteCount)
    if(suiteCount EQUAL 0)
        message(FATAL_ERROR "You have to pass at least one lit test suite path.")
    endif()

    # Process the optional target name argument
    if(ADD_CHECK_TARGET_NAME)
        set(target-name ${ADD_CHECK_TARGET_NAME})
    else()
        list(GET ADD_CHECK_TARGET_SUITES 0 firstSuite)
        set(target-name check-${firstSuite})
    endif()

    if(NOT ADD_CHECK_TARGET_COMMENT)
        list(GET ADD_CHECK_TARGET_SUITES 0 firstSuite)
        set(ADD_CHECK_TARGET_COMMENT "Running test suites: OTF-CPT::${firstSuite}")
        # Get the lists tail
        set(tmpList ${ADD_CHECK_TARGET_SUITES})
        list(REMOVE_AT tmpList 0)
        foreach(suite IN LISTS tmpList)
            set(ADD_CHECK_TARGET_COMMENT "${ADD_CHECK_TARGET_COMMENT}, OTF-CPT::${suite}")
        endforeach()
    endif()

    # Prepare escaped paths (who doesn't love spaces in paths?)
    foreach(suite_path IN LISTS ADD_CHECK_TARGET_SUITES)
        set(suitePaths ${suitePaths} "${suite_path}")
    endforeach()
    add_custom_target("${target-name}"
            COMMAND ${CMAKE_COMMAND} -E env
            "${LIT_EXECUTABLE}"
            --output "${target-name}.out.json"
            --xunit-xml-output "${target-name}.xunit.xml"
            --verbose
            --workers ${LIT_TEST_WORKERS}
            --time-tests
            ${suitePaths}
            VERBATIM
            BYPRODUCTS ${target-name}.out.json ${target-name}.xunit.xml
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "${ADD_CHECK_TARGET_COMMENT}"
            USES_TERMINAL)
    if (ADD_CHECK_TARGET_DEPENDS)
        add_dependencies(${target-name} ${ADD_CHECK_TARGET_DEPENDS})
    endif()
endfunction()

macro(pythonize_bool var)
    if (${var})
        set(${var} True)
    else()
        set(${var} False)
    endif()
endmacro()

