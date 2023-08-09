#include "stdafx.h"
#include <vector>
#include <iostream>
#include <yaml-cpp\yaml.h>

using namespace std;

typedef vector<pair<string, string>> idTypePairs;

class YamlUtils
{
public:
  static YAML::Node getChildNode(YAML::Node node, const std::string& name);

  static void addIdTypePairs(YAML::const_iterator it, idTypePairs& values);

  static std::string getType(YAML::Node node);

  static void printAtts(idTypePairs att);


};

class YamlClass
{
public:
  YamlClass();
  
  YamlClass(const std::string name);

  void print();

  void addAttributes(YAML::Node node);

  void addChildren(YAML::Node node);

private:
  const std::string mName;
  idTypePairs mAttributes;
  idTypePairs mChildren;
};

class YamlSpec
{
public:
  YamlSpec(const std::string& filename);

  void parse();

  void print();

private:
  const std::string& mFilename;
  std::vector<YamlClass> mChildClasses;
  YamlClass* mTopLevel;
};

