#include "json_test.h"
#include "xml_test.h"
#include "ini_test.h"
#include <iostream>

int main() {
  std::cout << "=== Running All Tests ===" << std::endl;

  JsonTest::run_all();
  XmlTest::run_all();
  IniTest::run_all();

  std::cout << "=== All Tests Passed ===" << std::endl;
  return 0;
}
