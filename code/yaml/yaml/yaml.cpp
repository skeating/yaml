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

typedef vector<pair<string, string>> idTypePairs;

void printAtts(idTypePairs att)
{
  for (unsigned int i = 0; i < att.size(); ++i)
  {
    cout << i << " : name: " << att.at(i).first << " type: " << att.at(i).second << endl;
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
  YAML::Node top_node = getChildNode(node, top);
  if (top_node.IsDefined() != true)
  {
    return values;
  }
  YAML::Node name_node = getChildNode(top_node, name);
  if (name_node.IsDefined() != true)
  {
    return values;
  }

  values = getNames(name_node);
  return values;
}

// assume this node is an allowed parameter with a type field
std::string 
getType(YAML::Node node)
{
  return (getChildNode(node, "type")).as<std::string>();
}

void
addIdTypePairs(YAML::const_iterator it, idTypePairs& values)
{ 
  std::string name = it->first.as<std::string>(); 
  std::string type = getType(it->second);
  values.push_back(std::make_pair(name, type));
}


idTypePairs
getIdTypePairs(YAML::Node node, const std::string &name, const std::string &type)
{
  idTypePairs values;
  YAML::Node top_node = getChildNode(node, name);
  if (top_node.IsDefined() != true)
  {
    return values;
  }
  YAML::Node name_node = getChildNode(top_node, type);
  if (name_node.IsDefined() != true)
  {
    return values;
  }
  for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
  {
    addIdTypePairs(it, values);
  }

  return values;
}


//int main()
//{
//  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test.yaml";
//  std::ifstream fin;
//  fin.open(filename);
//
//  
//  YAML::Node doc = YAML::Load(fin);
//
//  std::string top = "network";
//
//  std::string name = "allowed_children";
//
//  bool done = false;
//
//  idTypePairs a = getidTypePairs(doc, top, name);
//  printAtts(a);
//
////  //std::string value = get(doc, name, top, (done));
////
////  //cout << top << " : " << name << " is: " << value << endl;
////
////  //cout << "**********************\n\n";
////
////  std::vector<std::string> top_level = getNamesImmediateChildren(doc, top, name);
////
////  for (unsigned int i = 0; i < top_level.size(); ++i)
////  {
////    cout << i << " : " << top_level.at(i) << endl;
////  }
////  
////  cout << "**********************\n\n";
//////  printMap(doc, 0, "root");
////  cout << "**********************\n\n";
////
////  std::string name = "allowed_parameters";
////
////  YAML::Node tn = getChildNode(doc, top);
////  YAML::Node cn = getChildNode(tn, name);
////
////
////  printMap(cn, 0, name);
//  return 0;
//}
class YamlClass
{
public:
  YamlClass() :
    mName(""),
    mAttributes(NULL),
    mChildren(NULL)
  {};

  YamlClass(const std::string name) :
    mName(name),
    mAttributes(NULL),
    mChildren(NULL)
  {
  };

  void print() {
    cout << "class name: " << mName << endl;
    cout << "attributes: " << endl;
    printAtts(mAttributes);
    cout << "children:" << endl;
    printAtts(mChildren);
  };

  void addAttributes(YAML::Node node)
  {
    YAML::Node name_node = getChildNode(node, "allowed_parameters");
    for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
    {
      addIdTypePairs(it, mAttributes);
    }

  };
  void addChildren(YAML::Node node)
  {
    YAML::Node name_node = getChildNode(node, "allowed_children");
    for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
    {
      addIdTypePairs(it, mChildren);
    }

  };


  void setAttributes(idTypePairs pairs)
  {
    mAttributes = pairs;
  };

  void setChildren(idTypePairs pairs)
  {
    mChildren = pairs;
  };

private:
  const std::string mName;
  idTypePairs mAttributes;
  idTypePairs mChildren;

};

class YamlSpec
{
public:
  YamlSpec(const std::string& filename) :
    mFilename(filename),
    mTopLevel(NULL),
    mChildClasses(NULL)
  {
  };

  void parse() {
    std::ifstream fin;
    fin.open(mFilename);
    YAML::Node doc = YAML::Load(fin);
    bool first = true;
    for (YAML::const_iterator it = doc.begin(); it != doc.end(); ++it)
    {
      if (first)
      {
        mTopLevel = new YamlClass(it->first.as<string>());
        mTopLevel->addAttributes(it->second);
        mTopLevel->addChildren(it->second);
        first = false;
      }
      else
      {
        YamlClass yc = YamlClass(it->first.as<string>());
        yc.addAttributes(it->second);
        yc.addChildren(it->second);
        mChildClasses.push_back(yc);
      }
    }
  };

  void print() {
    mTopLevel->print();
    cout << "*******\n";
    for (unsigned int n = 0; n < mChildClasses.size(); ++n)
    {
      mChildClasses.at(n).print();
      cout << "*******\n";
    }
  };
private:
  const std::string& mFilename;

  std::vector<YamlClass> mChildClasses;
  YamlClass* mTopLevel;
};

void
main()
{
  std::string filename = "C:\\Development\\COMBINE\\yaml\\yamlspec\\test.yaml";
  YamlSpec spec = YamlSpec(filename);
  spec.parse();
  spec.print();
}


