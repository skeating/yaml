#include "stdafx.h"
#include <iostream>
#include <map>
#include <yaml-cpp\yaml.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const char* names_types[5] =
{
  "not defined",
  "null",
  "scaler",
  "sequence",
  "map"
};

void
doMap(YAML::const_iterator it)
{
  YAML::Node map2 = it->second;
  unsigned int size = it->second.size();
  YAML::const_iterator itm = map2.begin();
  for (unsigned int j = 0; j < size; ++j)
  {
    if (itm->first.Type() == 2)
    {
      std::cout << "[" << it->first.as<std::string>() << "][" << j << "]: " <<
        itm->first.as<std::string>() << "\n";
    }
    else
    {
      std::cout << "[" << it->first.as<std::string>() << "][" << j << "]: " << "MAP\n";
    }
    itm++;
  }
}

void
printDirectChildren(YAML::Node node, const std::string &name)
{
  std::cout << "node " << name << " is map size " << node.size() << "\n";
  unsigned int s = 0;
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    s++;
    std::cout << "[" << name << "][" << s << "]: " << it->first.as<std::string>() << "\n";
  }
}

void
printMap(YAML::Node node, int n, const std::string &name)
{
  printDirectChildren(node, name);
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {

    unsigned int t1 = it->first.Type();
    unsigned int t2 = it->second.Type();

    if (t1 == 2 && t2 == 4)
    {
      printMap(it->second, n + 1, it->first.as<std::string>());
    }

    if (t2 == 2)
    {
      std::cout << "[" << name << "][" << it->first.as<std::string>() << "]: " << it->second.as<std::string>() << "\n";
    }
  }


}

std::string
get(YAML::Node node, const std::string& name)
{
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    unsigned int t1 = it->first.Type();
    unsigned int t2 = it->second.Type();
   if (t2 == 2)
    {
      if (it->first.as<std::string>() == name)
      {
        return it->second.as<std::string>();
      }
    }
    else
    {
      return get(it->second, name);
    }

  }
}

int main()
{
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\t1.yaml";
  std::ifstream fin;
  fin.open(filename);

  
  YAML::Node doc = YAML::Load(fin);

  std::string name = "children";

  std::string value = get(doc, name);
  cout << "partner is: " << value << endl;

//  printMap(doc, 0, "root");
  return 0;
}
