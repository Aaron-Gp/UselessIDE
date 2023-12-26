enable_testing()

function(SETUP_TESTS)
   foreach(_testname ${ARGN})
       add_executable(${_testname} test${_testname}.cpp)
        qt_add_qml_module(${_testname}
            URI UselessIDE
            VERSION 1.0
            IMPORTS UselessIDE
            IMPORT_PATH ${CMAKE_BINARY_DIR}/UselessIDE
            OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${_testname}
        )
        add_test(NAME ${_testname} COMMAND ${_testname})
       link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
       target_link_libraries(${_testname} PRIVATE Qt6::Test Qt6::QuickTest IDELib)
       add_dependencies(${_testname} UselessIDE)
   endforeach()
endfunction()

SETUP_TESTS(
   compiler ui
)
