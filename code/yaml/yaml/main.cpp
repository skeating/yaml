#include "stdafx.h"
#include <iostream>
#include <map>
#include <yaml-cpp\yaml.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "yamlspec.h"

using namespace std;

const char* names_types[5] =
{
  "not defined",
  "null",
  "scaler",
  "sequence",
  "map"
};




std::vector<std::string>
getNames(YAML::Node node)
{
  std::vector<std::string> top_level;
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    top_level.push_back(it->first.as<std::string>());
  }
  return top_level;
}

int test_filename()
{
  int fails = 0;
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test.yaml";
  YamlSpec *spec = new YamlSpec(filename);
  if (spec == NULL) fails += 1;
  if (spec->getTopLevel() != NULL) fails += 1;
  if ((spec->getChildClasses()).size() != 0) fails += 1;
  if (spec->getNumChildClasses() != 0) fails += 1;
  if (spec->getFilename() != filename) fails += 1;

  return fails;
}

int test_parsing()
{
  int fails = 0;
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test1.yaml";
  YamlSpec *spec = new YamlSpec(filename);
  spec->parse();
  if (spec == NULL) fails += 1;
  if (spec->getTopLevel() == NULL) fails += 1;
  if ((spec->getChildClasses()).size() == 0) fails += 1;
  if (spec->getNumChildClasses() != 1) fails += 1;
  if (spec->getFilename() != filename) fails += 1;

  return fails;
}


int main() {
  int result = 0;
  unsigned int number_of_tests = 0;

  std::cout << "Tests started!" << std::endl;
  
  number_of_tests += 5;
  result += test_filename();

  number_of_tests += 5;
  result += test_parsing();  

  std::cout << "Tests finished!" << std::endl;
  std::cout << result << " tests failed!" << std::endl;


  return result; // You can put a 1 here to see later that it would generate an error
}

