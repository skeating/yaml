#include "stdafx.h"
#include "yamlspec.h"
#include <fstream>

YamlClass::YamlClass() :
  mName(""),
  mAttributes(NULL),
  mChildren(NULL)
{
}

YamlClass::YamlClass(const std::string name) :
  mName(name),
  mAttributes(NULL),
  mChildren(NULL)
{
}

void 
YamlClass::print() 
{
  cout << "class name: " << mName << endl;
  cout << "attributes: " << endl;
  YamlUtils::printAtts(mAttributes);
  cout << "children:" << endl;
  YamlUtils::printAtts(mChildren);
}

void 
YamlClass::addAttributes(YAML::Node node)
{
  YAML::Node name_node = YamlUtils::getChildNode(node, "allowed_parameters");
  for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
  {
    YamlUtils::addIdTypePairs(it, mAttributes);
  }
}
  
void 
YamlClass::addChildren(YAML::Node node)
{
  YAML::Node name_node = YamlUtils::getChildNode(node, "allowed_children");
  for (YAML::const_iterator it = name_node.begin(); it != name_node.end(); ++it)
  {
    YamlUtils::addIdTypePairs(it, mChildren);
  }

}


YamlSpec::YamlSpec(const std::string& filename) :
  mFilename(filename),
  mTopLevel(NULL),
  mChildClasses(NULL)
{
}

void 
YamlSpec::parse() 
{
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
}

void 
YamlSpec::print()
{
  mTopLevel->print();
  cout << "*******\n";
  for (unsigned int n = 0; n < mChildClasses.size(); ++n)
  {
    mChildClasses.at(n).print();
    cout << "*******\n";
  }
}

std::string
YamlUtils::getType(YAML::Node node)
{
  return (getChildNode(node, "type")).as<std::string>();
}

void
YamlUtils::addIdTypePairs(YAML::const_iterator it, idTypePairs& values)
{
  std::string name = it->first.as<std::string>();
  std::string type = getType(it->second);
  values.push_back(std::make_pair(name, type));
}



YAML::Node
YamlUtils::getChildNode(YAML::Node node, const std::string& name)
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

void 
YamlUtils::printAtts(idTypePairs att)
{
  for (unsigned int i = 0; i < att.size(); ++i)
  {
    cout << i << " : name: " << att.at(i).first << " type: " << att.at(i).second << endl;
  }
}

