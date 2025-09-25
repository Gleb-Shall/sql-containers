#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cassert>
#include "skip_list.h"

using namespace stl_containers;
using const_iterator = skip_list<int>::const_iterator;

void test_basic_operations() {
    std::cout << "Testing basic operations..." << std::endl;
    
    skip_list<int> sl;
    assert(sl.empty());
    assert(sl.size() == 0);
    
    // Test insert
    auto result = sl.insert(42);
    assert(result.second);
    assert(*result.first == 42);
    assert(sl.size() == 1);
    assert(!sl.empty());
    
    // Test insert duplicate
    auto result2 = sl.insert(42);
    assert(!result2.second);
    assert(*result2.first == 42);
    assert(sl.size() == 1);
    
    // Test insert multiple elements
    sl.insert(10);
    sl.insert(30);
    sl.insert(20);
    assert(sl.size() == 4);
    
    std::cout << "✓ Basic operations passed" << std::endl;
}

void test_iteration() {
    std::cout << "Testing iteration..." << std::endl;
    
    skip_list<int> sl = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Test forward iteration
    std::vector<int> result;
    for (const auto& elem : sl) {
        result.push_back(elem);
    }
    
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(result == expected);
    
    // Test const iteration
    const skip_list<int> const_sl = {1, 2, 3};
    std::vector<int> const_result;
    for (auto it = const_sl.cbegin(); it != const_sl.cend(); ++it) {
        const_result.push_back(*it);
    }
    assert(const_result.size() == 3);
    assert(const_result[0] == 1);
    assert(const_result[1] == 2);
    assert(const_result[2] == 3);
    
    std::cout << "✓ Iteration passed" << std::endl;
}

void test_search() {
    std::cout << "Testing search operations..." << std::endl;
    
    skip_list<int> sl = {1, 3, 5, 7, 9, 11, 13, 15};
    
    // Test find
    auto it = sl.find(7);
    assert(it != sl.end());
    assert(*it == 7);
    
    auto it2 = sl.find(6);
    assert(it2 == sl.end());
    
    // Test count
    assert(sl.count(7) == 1);
    assert(sl.count(6) == 0);
    
    // Test lower_bound
    auto lb = sl.lower_bound(6);
    assert(lb != sl.end());
    assert(*lb == 7);
    
    // Test upper_bound
    auto ub = sl.upper_bound(6);
    assert(ub != sl.end());
    assert(*ub == 7);
    
    // Test equal_range
    auto range = sl.equal_range(7);
    assert(*range.first == 7);
    assert(*range.second == 9);
    
    std::cout << "✓ Search operations passed" << std::endl;
}

void test_erase() {
    std::cout << "Testing erase operations..." << std::endl;
    
    skip_list<int> sl = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Test erase by iterator
    auto it = sl.find(5);
    assert(it != sl.end());
    
    auto next_it = sl.erase(it);
    assert(sl.size() == 8);
    assert(*next_it == 6);
    
    // Test erase by value
    size_t erased = sl.erase(3);
    assert(erased == 1);
    assert(sl.size() == 7);
    assert(sl.find(3) == sl.end());
    
    // Test erase non-existing value
    erased = sl.erase(10);
    assert(erased == 0);
    assert(sl.size() == 7);
    
    std::cout << "✓ Erase operations passed" << std::endl;
}

void test_constructors() {
    std::cout << "Testing constructors..." << std::endl;
    
    // Default constructor
    skip_list<int> sl1;
    assert(sl1.empty());
    
    // Fill constructor (skip list doesn't add duplicates)
    skip_list<int> sl2(5, 42);
    assert(sl2.size() == 1); // Only one element because duplicates are not added
    
    // Range constructor (skip list doesn't add duplicates)
    std::vector<int> values = {3, 1, 4, 1, 5};
    skip_list<int> sl3(values.begin(), values.end());
    assert(sl3.size() == 4); // Only 4 unique elements because duplicates are not added
    
    // Initializer list constructor (skip list doesn't add duplicates)
    skip_list<int> sl4 = {3, 1, 4, 1, 5};
    assert(sl4.size() == 4); // Only 4 unique elements because duplicates are not added
    
    // Copy constructor
    skip_list<int> sl5(sl4);
    assert(sl5.size() == sl4.size());
    assert(sl5 == sl4);
    
    // Move constructor
    skip_list<int> sl6(std::move(sl5));
    assert(sl6.size() == 4); // Only 4 unique elements because duplicates are not added
    assert(sl5.empty());
    
    std::cout << "✓ Constructors passed" << std::endl;
}

void test_comparison() {
    std::cout << "Testing comparison operations..." << std::endl;
    
    skip_list<int> sl1 = {1, 2, 3};
    skip_list<int> sl2 = {1, 2, 3};
    skip_list<int> sl3 = {1, 2, 4};
    
    // Test equality
    assert(sl1 == sl2);
    assert(sl1 != sl3);
    
    // Test ordering
    assert(sl1 < sl3);
    assert(sl3 > sl1);
    assert(sl1 <= sl2);
    assert(sl1 >= sl2);
    
    std::cout << "✓ Comparison operations passed" << std::endl;
}

void test_error_handling() {
    std::cout << "Testing error handling..." << std::endl;
    
    skip_list<int> sl = {1, 2, 3, 4, 5};
    
    // Test dereferencing end iterator
    auto it = sl.end();
    try {
        *it;
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        std::cout << "  Caught expected exception: " << e.what() << std::endl;
    }
    
    // Test erasing end iterator
    try {
        sl.erase(it);
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        std::cout << "  Caught expected exception: " << e.what() << std::endl;
    }
    
    std::cout << "✓ Error handling passed" << std::endl;
}

void test_performance() {
    std::cout << "Testing performance..." << std::endl;
    
    constexpr size_t N = 10000;
    skip_list<int> sl;
    
    // Insert performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        sl.insert(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    assert(sl.size() == N);
    std::cout << "  Insert " << N << " elements in " << duration.count() << " microseconds" << std::endl;
    
    // Search performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i += 10) {
        auto it = sl.find(i);
        assert(it != sl.end());
        assert(*it == i);
    }
    end = std::chrono::high_resolution_clock::now();
    
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "  Search " << N/10 << " elements in " << duration.count() << " microseconds" << std::endl;
    
    std::cout << "✓ Performance test passed" << std::endl;
}

void test_custom_type() {
    std::cout << "Testing custom type..." << std::endl;
    
    struct TestStruct {
        int value;
        std::string name;
        
        TestStruct() : value(0), name("") {}
        TestStruct(int v, std::string n) : value(v), name(std::move(n)) {}
        
        bool operator<(const TestStruct& other) const {
            return value < other.value;
        }
        
        bool operator==(const TestStruct& other) const {
            return value == other.value && name == other.name;
        }
    };
    
    skip_list<TestStruct> sl;
    
    sl.insert(TestStruct(1, "one"));
    sl.insert(TestStruct(2, "two"));
    sl.insert(TestStruct(0, "zero"));
    
    assert(sl.size() == 3);
    
    auto it = sl.find(TestStruct(1, "one"));
    assert(it != sl.end());
    assert(it->value == 1);
    assert(it->name == "one");
    
    std::cout << "✓ Custom type test passed" << std::endl;
}

void test_emplace() {
    std::cout << "Testing emplace..." << std::endl;
    
    skip_list<std::string> sl;
    auto result = sl.emplace("test");
    
    assert(result.second);
    assert(*result.first == "test");
    assert(sl.size() == 1);
    
    std::cout << "✓ Emplace test passed" << std::endl;
}

void test_swap() {
    std::cout << "Testing swap..." << std::endl;
    
    skip_list<int> sl1 = {1, 2, 3};
    skip_list<int> sl2 = {4, 5, 6};
    
    size_t size1 = sl1.size();
    size_t size2 = sl2.size();
    
    sl1.swap(sl2);
    
    assert(sl1.size() == size2);
    assert(sl2.size() == size1);
    
    std::cout << "✓ Swap test passed" << std::endl;
}

int main() {
    std::cout << "=== Running SkipList Tests ===" << std::endl;
    std::cout << "Starting SkipList tests..." << std::endl;
    std::cout << "=========================" << std::endl;
    
    try {
        test_basic_operations();
        test_iteration();
        test_search();
        test_erase();
        test_constructors();
        test_comparison();
        test_error_handling();
        test_performance();
        test_custom_type();
        test_emplace();
        test_swap();
        
        std::cout << "=========================" << std::endl;
        std::cout << "All tests passed successfully!" << std::endl;
        std::cout << "=== All tests passed! ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cout << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}