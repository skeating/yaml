#include <iostream>
#include "../yaml/yamlspec.h"

int test_filename()
{
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test.yaml";
  YamlSpec *spec = new YamlSpec();

  return 10;
}

int main() {
  int result = 0;

  std::cout << "Tests started!" << std::endl;
  result += test_filename();

  std::cout << "Tests finished!" << std::endl;
  std::cout << result << " tests failed!" << std::endl;


  return result; // You can put a 1 here to see later that it would generate an error
}