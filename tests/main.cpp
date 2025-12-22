#include "qt_test.h"
#include "stl_test.h"
#include <iostream>

int main() {
  std::cout << "=== Running All Tests ===" << std::endl;

  StlTest::run_all();
  QtTest::run_all();

  std::cout << "=== All Tests Passed ===" << std::endl;
  return 0;
}
