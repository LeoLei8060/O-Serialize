
add_executable(unit_tests main.cpp json_test.h xml_test.h ini_test.h)
target_link_libraries(unit_tests PRIVATE O-Serialize)
