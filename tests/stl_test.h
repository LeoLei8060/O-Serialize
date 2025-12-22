#ifndef STL_TEST_H
#define STL_TEST_H

#include "o_serialize/ini.h"
#include "o_serialize/json.h"
#include "o_serialize/o_serialize.h"
#include "o_serialize/xml.h"
#include <cassert>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <variant>

// Custom struct for testing
struct Person {
  std::string name;
  int age;
  std::vector<std::string> skills;

  bool operator==(const Person &other) const {
    return name == other.name && age == other.age && skills == other.skills;
  }
};

O_SERIALIZE_STRUCT(Person, name, age, skills)

struct Group {
  std::string groupName;
  Person leader;
  std::vector<Person> members;

  bool operator==(const Group &other) const {
    return groupName == other.groupName && leader == other.leader &&
           members == other.members;
  }
};

O_SERIALIZE_STRUCT(Group, groupName, leader, members)

enum class Color { Red, Green, Blue };

namespace StlTest {

void test_basic_types() {
  std::cout << "[STL] Testing Basic Types..." << std::endl;

  // JSON
  int i = 42;
  std::string i_json = OSerialize::JSON::stringify(i);
  assert(OSerialize::JSON::parse<int>(i_json) == 42);

  // XML
  std::string i_xml = OSerialize::XML::stringify(i, "value");
  assert(OSerialize::XML::parse<int>(i_xml, "value") == 42);

  // INI
  std::string i_ini = OSerialize::INI::stringify(i, "default");
  assert(OSerialize::INI::parse<int>(i_ini, "default") == 42);
  
  // Enum
  Color c = Color::Green;
  // JSON Enum
  std::string c_json = OSerialize::JSON::stringify(c);
  assert(OSerialize::JSON::parse<Color>(c_json) == Color::Green);
  
  // XML Enum
  std::string c_xml = OSerialize::XML::stringify(c, "color");
  assert(OSerialize::XML::parse<Color>(c_xml, "color") == Color::Green);

  std::cout << "[STL] Basic Types Passed!" << std::endl;
}

void test_containers() {
  std::cout << "[STL] Testing Containers..." << std::endl;

  std::vector<int> vec = {1, 2, 3};
  std::list<std::string> lst = {"A", "B"};
  std::set<int> st = {10, 20};

  // JSON
  assert(OSerialize::JSON::parse<std::vector<int>>(OSerialize::JSON::stringify(vec)) == vec);
  assert(OSerialize::JSON::parse<std::list<std::string>>(OSerialize::JSON::stringify(lst)) == lst);
  assert(OSerialize::JSON::parse<std::set<int>>(OSerialize::JSON::stringify(st)) == st);

  // XML
  assert(OSerialize::XML::parse<std::vector<int>>(OSerialize::XML::stringify(vec, "list"), "list") == vec);
  assert(OSerialize::XML::parse<std::list<std::string>>(OSerialize::XML::stringify(lst, "list"), "list") == lst);
  // Set in XML order might vary? Set is ordered. XML order is insertion order from iteration. Should match.
  assert(OSerialize::XML::parse<std::set<int>>(OSerialize::XML::stringify(st, "set"), "set") == st);

  // INI
  assert(OSerialize::INI::parse<std::vector<int>>(OSerialize::INI::stringify(vec, "vec"), "vec") == vec);

  std::cout << "[STL] Containers Passed!" << std::endl;
}

void test_maps() {
    std::cout << "[STL] Testing Maps..." << std::endl;
    
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}};
    // Unordered map might have different order in serialization string, but object comparison handles it.
    std::unordered_map<std::string, int> um = {{"apple", 5}, {"banana", 10}}; 

    // JSON
    auto m_json = OSerialize::JSON::parse<std::map<std::string, int>>(OSerialize::JSON::stringify(m));
    assert(m_json == m);
    
    auto um_json = OSerialize::JSON::parse<std::unordered_map<std::string, int>>(OSerialize::JSON::stringify(um));
    assert(um_json == um);
    
    // XML
    auto m_xml = OSerialize::XML::parse<std::map<std::string, int>>(OSerialize::XML::stringify(m, "map"), "map");
    assert(m_xml == m);
    
    auto um_xml = OSerialize::XML::parse<std::unordered_map<std::string, int>>(OSerialize::XML::stringify(um, "map"), "map");
    assert(um_xml == um);
    
    // INI
    auto m_ini = OSerialize::INI::parse<std::map<std::string, int>>(OSerialize::INI::stringify(m, "map"), "map");
    assert(m_ini == m);
    
    std::cout << "[STL] Maps Passed!" << std::endl;
}

void test_smart_pointers() {
    std::cout << "[STL] Testing Smart Pointers..." << std::endl;
    
    std::shared_ptr<int> p1 = std::make_shared<int>(100);
    std::shared_ptr<int> p2 = nullptr;
    
    // JSON
    auto p1_json = OSerialize::JSON::parse<std::shared_ptr<int>>(OSerialize::JSON::stringify(p1));
    assert(*p1_json == 100);
    auto p2_json = OSerialize::JSON::parse<std::shared_ptr<int>>(OSerialize::JSON::stringify(p2));
    assert(p2_json == nullptr);
    
    // XML
    auto p1_xml = OSerialize::XML::parse<std::shared_ptr<int>>(OSerialize::XML::stringify(p1, "val"), "val");
    assert(*p1_xml == 100);
    
    // INI (Smart pointer points to single value)
    // auto p1_ini = OSerialize::INI::parse<std::shared_ptr<int>>(OSerialize::INI::stringify(p1, "val"), "val");
    // assert(*p1_ini == 100);
    
    std::cout << "[STL] Smart Pointers Passed!" << std::endl;
}

void test_variant() {
    std::cout << "[STL] Testing Variant (Write Only)..." << std::endl;
    // Variant read support needs visitor or explicit type handling which is tricky in generic deserialize.
    // Our implementation currently supports to_json/to_xml.
    
    std::variant<int, std::string> v1 = 123;
    std::variant<int, std::string> v2 = "hello";
    
    std::cout << "Variant Int JSON: " << OSerialize::JSON::stringify(v1) << std::endl;
    std::cout << "Variant String JSON: " << OSerialize::JSON::stringify(v2) << std::endl;
    
    std::cout << "[STL] Variant Passed!" << std::endl;
}

void test_custom_structs() {
  std::cout << "[STL] Testing Custom Structs..." << std::endl;

  Person p{"Alice", 30, {"C++", "Qt"}};

  // JSON
  std::string p_json = OSerialize::JSON::stringify(p);
  assert(OSerialize::JSON::parse<Person>(p_json) == p);

  // XML
  std::string p_xml = OSerialize::XML::stringify(p, "Person");
  assert(OSerialize::XML::parse<Person>(p_xml, "Person") == p);

  // INI
  std::string p_ini = OSerialize::INI::stringify(p, "Person");
  assert(OSerialize::INI::parse<Person>(p_ini, "Person") == p);

  std::cout << "[STL] Custom Structs Passed!" << std::endl;
}

void test_mulStory_structs() {
  std::cout << "[STL] Testing mulStory (Nested) Structs..." << std::endl;

  Group g;
  g.groupName = "Engineering";
  g.leader = {"Alice", 30, {"C++", "Management"}};
  g.members.push_back({"Bob", 25, {"Java", "Python"}});
  g.members.push_back({"Charlie", 28, {"Go", "Rust"}});

  // JSON
  std::string g_json = OSerialize::JSON::stringify(g);
  Group g_parsed_json = OSerialize::JSON::parse<Group>(g_json);
  assert(g_parsed_json == g);

  // XML
  std::string g_xml = OSerialize::XML::stringify(g, "Group");
  Group g_parsed_xml = OSerialize::XML::parse<Group>(g_xml, "Group");
  assert(g_parsed_xml == g);

  // INI
  // INI serialization for nested structs (Group -> Person) is not supported 
  // because INI is flat and we don't have operator<< for Person.
  // std::string g_ini = OSerialize::INI::stringify(g, "Group");
  // assert(OSerialize::INI::parse<Group>(g_ini, "Group") == g);

  std::cout << "[STL] mulStory Structs Passed!" << std::endl;
}

void run_all() {
  test_basic_types();
  test_containers();
  test_maps();
  test_smart_pointers();
  test_variant();
  test_custom_structs();
  test_mulStory_structs();
}
} // namespace StlTest

#endif // STL_TEST_H
