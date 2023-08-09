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

// gets the node named top from the overall document


// GET THE NAMES OF THE IMMEDIATE CHILDREN
// eg
// population:
//  definition: Some description...
//  allowed_parameters :
//    id :
//      type : str
//      description : The id of the population
//    component :
//      type: str
//      description : the component to use in the population
//     size :
//       type: int
//        description : the size of the population
// 
// getNamesImmediateChildren(node, "population", "allowed_parameters")
// returns {"id", "component", "size"}

//std::vector<std::string>
//getNamesImmediateChildren(YAML::Node node, const std::string& top, const std::string& name)
//{
//  std::vector<std::string> values;
//
//  // get top node
//  YAML::Node top_node = getChildNode(node, top);
//  if (top_node.IsDefined() != true)
//  {
//    return values;
//  }
//  YAML::Node name_node = getChildNode(top_node, name);
//  if (name_node.IsDefined() != true)
//  {
//    return values;
//  }
//
//  values = getNames(name_node);
//  return values;
//}



//idTypePairs
//getIdTypePairs(YAML::Node node, const std::string &name, const std::string &type)
//{
//  idTypePairs values;
//  YAML::Node top_node = getChildNode(node, name);
//  if (top_node.IsDefined() != true)
//  {
//    return values;
//  }
//  YAML::Node name_node = getChildNode(top_node, type);
//  if (name_node.IsDefined() != true)
//  {
//    return values;
//  }
//  for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
//  {
//    addIdTypePairs(it, values);
//  }
//
//  return values;
//}



void
main()
{
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test.yaml";
  YamlSpec spec = YamlSpec(filename);
  spec.parse();
  spec.print();
}


