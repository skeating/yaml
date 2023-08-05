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

// gets the value of 'name' for the top level element given
// note this assumes there is only one value 'name' and that it is a string
std::string
get(YAML::Node node, const std::string& name, const std::string& top, bool& correctBranch)
{
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    unsigned int t1 = it->first.Type();
    unsigned int t2 = it->second.Type();
    if (t1 == 2)
    {
      // we are at the top of a branch 
      if (it->first.as<std::string>() == top)
      {
        correctBranch = true;
      }
    }

    if (correctBranch)
    {
      if (t2 == 2)
      {
        if (it->first.as<std::string>() == name)
        {
          return it->second.as<std::string>();
        }
      }
      else
      {
        return get(it->second, name, top, correctBranch);
      }
    }

  }
}

std::vector<std::string>
getTopLevelNames(YAML::Node node)
{
  std::vector<std::string> top_level;
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    top_level.push_back(it->first.as<std::string>());
  }
  return top_level;
}

// gets the node named top from the overall document

YAML::Node
getTopLevelNode(YAML::Node node, const std::string& top)
{
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    unsigned int t1 = it->first.Type();
    if (t1 == 2)
    {
      // we are at the top of a branch 
      if (it->first.as<std::string>() == top)
      {
        return it->second;
      }
    }
  }
  return YAML::Node(YAML::NodeType::Undefined);
}

YAML::Node
getChildNode(YAML::Node node, const std::string& name)
{
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    unsigned int t1 = it->first.Type();
    if (t1 == 2)
    {
      // we are at the node 'name'
      if (it->first.as<std::string>() == name)
      {
        return it->second;
      }
    }
  }
  return YAML::Node(YAML::NodeType::Undefined);
}

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

std::vector<std::string>
getNamesImmediateChildren(YAML::Node node, const std::string& top, const std::string& name)
{
  std::vector<std::string> values;

  // get top node
  YAML::Node top_node = getTopLevelNode(node, top);
  if (top_node.IsDefined() != true)
  {
    return values;
  }
  YAML::Node name_node = getChildNode(top_node, name);
  if (name_node.IsDefined() != true)
  {
    return values;
  }

  for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
  {
    unsigned int t1 = it->first.Type();
    if (t1 == 2)
    {
      values.push_back(it->first.as<std::string>());
    }
  }
  return values;
}

//std::vector<std::string>
//getValuesFor(YAML::Node node, const std::string& name, const std::string& top)
//{
//  std::vector<std::string> values;
//  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
//  {
//    values.push_back(it->first.as<std::string>());
//  }
//  return values;
//
//}
//

int main()
{
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test.yaml";
  std::ifstream fin;
  fin.open(filename);

  
  YAML::Node doc = YAML::Load(fin);

  std::string top = "population";

  std::string name = "allowed_parameters";

  bool done = false;

  //std::string value = get(doc, name, top, (done));

  //cout << top << " : " << name << " is: " << value << endl;

  //cout << "**********************\n\n";

  std::vector<std::string> top_level = getNamesImmediateChildren(doc, "Sarah", name);

  for (unsigned int i = 0; i < top_level.size(); ++i)
  {
    cout << i << " : " << top_level.at(i) << endl;
  }
  
  cout << "**********************\n\n";
//  printMap(doc, 0, "root");
  cout << "**********************\n\n";

//  std::string name = "allowed_parameters";

  //YAML::Node tn = getTopLevelNode(doc, top);
  //YAML::Node cn = getChildNode(tn, "Sarah");


  //printMap(cn, 0, name);
  return 0;
}
