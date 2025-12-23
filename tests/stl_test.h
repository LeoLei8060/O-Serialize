#ifndef STL_TEST_H
#define STL_TEST_H

#include "o_serialize/json.h"
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

using namespace OSerialize;

namespace StlTest {

void test_int()
{
    std::cout << "Testing int..." << std::endl;
    int         original = 123;
    std::string json = JSON::obj_to_string(original);
    int         parsed = JSON::string_to_obj<int>(json);
    assert(original == parsed);
}

void test_double()
{
    std::cout << "Testing double..." << std::endl;
    double      original = 123.456;
    std::string json = JSON::obj_to_string(original);
    double      parsed = JSON::string_to_obj<double>(json);
    // simple float comparison
    assert(abs(original - parsed) < 0.0001);
}

void test_string()
{
    std::cout << "Testing std::string..." << std::endl;
    std::string original = "Hello World";
    std::string json = JSON::obj_to_string(original);
    std::string parsed = JSON::string_to_obj<std::string>(json);
    assert(original == parsed);
}

void test_vector()
{
    std::cout << "Testing std::vector..." << std::endl;
    std::vector<int> original = {1, 2, 3, 4, 5};
    std::string      json = JSON::obj_to_string(original);
    std::vector<int> parsed = JSON::string_to_obj<std::vector<int>>(json);
    assert(original == parsed);
}

void test_list()
{
    std::cout << "Testing std::list..." << std::endl;
    std::list<std::string> original = {"a", "b", "c"};
    std::string            json = JSON::obj_to_string(original);
    std::list<std::string> parsed = JSON::string_to_obj<std::list<std::string>>(json);
    assert(original == parsed);
}

void test_deque()
{
    std::cout << "Testing std::deque..." << std::endl;
    std::deque<float> original = {1.1f, 2.2f};
    std::string       json = JSON::obj_to_string(original);
    std::deque<float> parsed = JSON::string_to_obj<std::deque<float>>(json);
    assert(original.size() == parsed.size());
    assert(abs(original[0] - parsed[0]) < 0.0001);
}

void test_set()
{
    std::cout << "Testing std::set..." << std::endl;
    std::set<int> original = {1, 5, 2, 4}; // 1, 2, 4, 5
    std::string   json = JSON::obj_to_string(original);
    std::set<int> parsed = JSON::string_to_obj<std::set<int>>(json);
    assert(original == parsed);
}

void test_map()
{
    std::cout << "Testing std::map..." << std::endl;
    std::map<std::string, int> original = {{"one", 1}, {"two", 2}};
    std::string                json = JSON::obj_to_string(original);
    std::map<std::string, int> parsed = JSON::string_to_obj<std::map<std::string, int>>(json);
    assert(original == parsed);
    std::map<std::string, int> original_qstr = {{"one", 1}, {"two", 2}};
    std::string                json_qstr = JSON::obj_to_string(original_qstr);
    std::map<std::string, int> parsed_qstr = JSON::string_to_obj<std::map<std::string, int>>(json_qstr);
    assert(original == parsed_qstr);

    // 测试无效map类型
    // std::map<int, int> test_map;
    // std::string        test_json = JSON::obj_to_string(test_map);
    // std::map<int, int> parsed_test = JSON::string_to_obj<std::map<int, int>>(test_json);
}

void test_pair()
{
    std::cout << "Testing std::pair..." << std::endl;
    std::pair<int, std::string> original = {42, "answer"};
    std::string                 json = JSON::obj_to_string(original);
    std::pair<int, std::string> parsed = JSON::string_to_obj<std::pair<int, std::string>>(json);
    assert(original == parsed);
}

void test_tuple()
{
    std::cout << "Testing std::tuple..." << std::endl;
    std::tuple<int, double, std::string> original = {1, 3.14, "tuple"};
    std::string                          json = JSON::obj_to_string(original);
    auto                                 parsed = JSON::string_to_obj<std::tuple<int, double, std::string>>(json);
    assert(original == parsed);
}

void test_shared_ptr()
{
    std::cout << "Testing std::shared_ptr..." << std::endl;
    std::shared_ptr<int> original = std::make_shared<int>(999);
    std::string          json = JSON::obj_to_string(original);
    auto                 parsed = JSON::string_to_obj<std::shared_ptr<int>>(json);
    assert(*original == *parsed);

    std::shared_ptr<int> nullPtr;
    json = JSON::obj_to_string(nullPtr);
    parsed = JSON::string_to_obj<std::shared_ptr<int>>(json);
    assert(parsed == nullptr);
}

struct AllStlTypes
{
    int                         i;
    double                      d;
    std::string                 s;
    std::vector<int>            vec;
    std::map<std::string, int>  map;
    std::pair<int, std::string> pair;
    std::tuple<int, double>     tuple;
    std::shared_ptr<int>        ptr;

    bool operator==(const AllStlTypes &other) const
    {
        return i == other.i && std::abs(d - other.d) < 0.001 && s == other.s && vec == other.vec && map == other.map
               && pair == other.pair && tuple == other.tuple
               && ((!ptr && !other.ptr) || (ptr && other.ptr && *ptr == *other.ptr));
    }
};
} // namespace StlTest

O_SERIALIZE_STRUCT(StlTest::AllStlTypes, i, d, s, vec, map, pair, tuple, ptr);

namespace StlTest {
void test_all_stl_types()
{
    std::cout << "Testing AllStlTypes struct..." << std::endl;
    AllStlTypes original;
    original.i = 100;
    original.d = 99.9;
    original.s = "Complete";
    original.vec = {10, 20, 30};
    original.map = {{"key", 100}};
    original.pair = {5, "five"};
    original.tuple = {1, 2.0};
    original.ptr = std::make_shared<int>(777);

    std::string json = JSON::obj_to_string(original);
    AllStlTypes parsed = JSON::string_to_obj<AllStlTypes>(json);

    QSharedPointer<AllStlTypes> original_qptr = QSharedPointer<AllStlTypes>::create(original);
    std::string                 json_qptr = JSON::obj_to_string(original_qptr);
    QSharedPointer<AllStlTypes> parsed_qptr = JSON::string_to_obj<QSharedPointer<AllStlTypes>>(json_qptr);

    assert(original == parsed);
    assert(*original_qptr.get() == *parsed_qptr.get());
}

void test_file_io()
{
    std::cout << "Testing file IO..." << std::endl;
    AllStlTypes original;
    original.i = 888;
    original.d = 123.456;
    original.s = "File IO Test";
    original.vec = {1, 2, 3};
    original.map = {{"file", 1}};
    original.pair = {10, "ten"};
    original.tuple = {2, 3.3};
    original.ptr = std::make_shared<int>(555);

    std::string filepath = "test_output.json";

    // Test obj_to_file
    bool success = JSON::obj_to_file(original, filepath);
    assert(success);

    // Test file_to_obj
    AllStlTypes parsed = JSON::file_to_obj<AllStlTypes>(filepath);
    assert(original == parsed);

    // Cleanup
    // std::remove(filepath.c_str());
}

void run_all()
{
    test_int();
    test_double();
    test_string();
    test_vector();
    test_list();
    test_deque();
    test_set();
    test_map();
    test_pair();
    test_tuple();
    test_shared_ptr();
    test_all_stl_types();
    test_file_io();
}
} // namespace StlTest

#endif
