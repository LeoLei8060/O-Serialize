
add_executable(unit_tests main.cpp qt_test.h stl_test.h)
target_link_libraries(unit_tests PRIVATE O-Serialize)
