#ifndef STL_TEST_H
#define STL_TEST_H

#include "o_serialize/json.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <tuple>
#include <utility>
#include <variant>

using namespace OSerialize;

namespace StlTest {

    void test_int() {
        std::cout << "Testing int..." << std::endl;
        int original = 123;
        std::string json = JSON::obj_to_string(original);
        int parsed = JSON::string_to_obj<int>(json);
        assert(original == parsed);
    }

    void test_double() {
        std::cout << "Testing double..." << std::endl;
        double original = 123.456;
        std::string json = JSON::obj_to_string(original);
        double parsed = JSON::string_to_obj<double>(json);
        // simple float comparison
        assert(abs(original - parsed) < 0.0001);
    }

    void test_string() {
        std::cout << "Testing std::string..." << std::endl;
        std::string original = "Hello World";
        std::string json = JSON::obj_to_string(original);
        std::string parsed = JSON::string_to_obj<std::string>(json);
        assert(original == parsed);
    }

    void test_vector() {
        std::cout << "Testing std::vector..." << std::endl;
        std::vector<int> original = {1, 2, 3, 4, 5};
        std::string json = JSON::obj_to_string(original);
        std::vector<int> parsed = JSON::string_to_obj<std::vector<int>>(json);
        assert(original == parsed);
    }

    void test_list() {
        std::cout << "Testing std::list..." << std::endl;
        std::list<std::string> original = {"a", "b", "c"};
        std::string json = JSON::obj_to_string(original);
        std::list<std::string> parsed = JSON::string_to_obj<std::list<std::string>>(json);
        assert(original == parsed);
    }

    void test_deque() {
        std::cout << "Testing std::deque..." << std::endl;
        std::deque<float> original = {1.1f, 2.2f};
        std::string json = JSON::obj_to_string(original);
        std::deque<float> parsed = JSON::string_to_obj<std::deque<float>>(json);
        assert(original.size() == parsed.size());
        assert(abs(original[0] - parsed[0]) < 0.0001);
    }

    void test_set() {
        std::cout << "Testing std::set..." << std::endl;
        std::set<int> original = {1, 5, 2, 4}; // 1, 2, 4, 5
        std::string json = JSON::obj_to_string(original);
        std::set<int> parsed = JSON::string_to_obj<std::set<int>>(json);
        assert(original == parsed);
    }

    void test_map() {
        std::cout << "Testing std::map..." << std::endl;
        std::map<std::string, int> original = {{"one", 1}, {"two", 2}};
        std::string json = JSON::obj_to_string(original);
        std::map<std::string, int> parsed = JSON::string_to_obj<std::map<std::string, int>>(json);
        assert(original == parsed);
    }

    void test_pair() {
        std::cout << "Testing std::pair..." << std::endl;
        std::pair<int, std::string> original = {42, "answer"};
        std::string json = JSON::obj_to_string(original);
        std::pair<int, std::string> parsed = JSON::string_to_obj<std::pair<int, std::string>>(json);
        assert(original == parsed);
    }

    void test_tuple() {
        std::cout << "Testing std::tuple..." << std::endl;
        std::tuple<int, double, std::string> original = {1, 3.14, "tuple"};
        std::string json = JSON::obj_to_string(original);
        auto parsed = JSON::string_to_obj<std::tuple<int, double, std::string>>(json);
        assert(original == parsed);
    }

    void test_shared_ptr() {
        std::cout << "Testing std::shared_ptr..." << std::endl;
        std::shared_ptr<int> original = std::make_shared<int>(999);
        std::string json = JSON::obj_to_string(original);
        auto parsed = JSON::string_to_obj<std::shared_ptr<int>>(json);
        assert(*original == *parsed);
        
        std::shared_ptr<int> nullPtr;
        json = JSON::obj_to_string(nullPtr);
        parsed = JSON::string_to_obj<std::shared_ptr<int>>(json);
        assert(parsed == nullptr);
    }

    struct AllStlTypes {
        int i;
        double d;
        std::string s;
        std::vector<int> vec;
        std::map<std::string, int> map;
        std::pair<int, std::string> pair;
        std::tuple<int, double> tuple;
        std::shared_ptr<int> ptr;
        
        bool operator==(const AllStlTypes& other) const {
            return i == other.i &&
                   std::abs(d - other.d) < 0.001 &&
                   s == other.s &&
                   vec == other.vec &&
                   map == other.map &&
                   pair == other.pair &&
                   tuple == other.tuple &&
                   ((!ptr && !other.ptr) || (ptr && other.ptr && *ptr == *other.ptr));
        }
    };
}

O_SERIALIZE_STRUCT(StlTest::AllStlTypes, i, d, s, vec, map, pair, tuple, ptr);

namespace StlTest {
    void test_all_stl_types() {
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
        // std::cout << "JSON: " << json << std::endl;
        AllStlTypes parsed = JSON::string_to_obj<AllStlTypes>(json);

        assert(original == parsed);
    }

    void run_all() {
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
    }
}

#endif
