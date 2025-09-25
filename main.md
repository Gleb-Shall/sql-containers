# STL Container "Skip List"

STL-compatible Skip List container implementation with full support for C++20 standard concepts, iterators, and error handling.

## Features

-  **STL Compatibility**: Full support for standard STL interfaces
-  **C++20 Concepts**: Support for Container, SequenceContainer and other concepts
-  **Iterators**: Forward iterators with full functionality
-  **Error Handling**: Exceptions for incorrect operations
-  **Performance**: O(log n) for search, insert and delete operations
-  **Memory**: Efficient memory usage with probabilistic structure
-  **Tests**: Full test coverage without external dependencies

## Project Structure

```
sql-containers/
├── .github/
│   └── workflows/
│       └── ci.yml             # Continuous Integration
├── include/
│   └── skip_list.h            # Main header file
├── tests/
│   └── simple_test.cpp        # Tests
├── docs/
│   └── html/                  # Generated documentation
├── Makefile                   # Build system
└── main.md                    # This file
```

## Quick Start

### Building with Makefile

```bash
# Build all components
make

# Run tests
make test

# Generate documentation
make docs

# Clean build files
make clean

# Show help
make help
```

### Demonstration

```bash
# Run tests
make test
```

## Usage

### Basic Operations

```cpp
#include "skip_list.h"
using namespace stl_containers;

// Create container
skip_list<int> sl;

// Insert elements
sl.insert(5);
sl.insert(2);
sl.insert(8);

// Search
auto it = sl.find(5);
if (it != sl.end()) {
    std::cout << "Found: " << *it << std::endl;
}

// Iteration
for (const auto& value : sl) {
    std::cout << value << " ";
}
// Output: 2 5 8

// Erase
sl.erase(5);
```

### Initialization

```cpp
// Empty container
skip_list<int> sl1;

// With specified number of elements
skip_list<int> sl2(10, 42);

// From range
std::vector<int> v = {3, 1, 4, 1, 5};
skip_list<int> sl3(v.begin(), v.end());

// From initializer list
skip_list<int> sl4 = {3, 1, 4, 1, 5};
```

### Search and Bounds

```cpp
skip_list<int> sl = {1, 3, 5, 7, 9, 11, 13, 15};

// Find element
auto it = sl.find(7);

// Lower bound
auto lb = sl.lower_bound(6);  // Returns iterator to 7

// Upper bound
auto ub = sl.upper_bound(6);  // Returns iterator to 7

// Equal range
auto [first, last] = sl.equal_range(7);
```

### Iterators

```cpp
skip_list<int> sl = {1, 2, 3, 4, 5};

// Forward iteration
for (auto it = sl.begin(); it != sl.end(); ++it) {
    std::cout << *it << " ";
}

// Range-based for loop
for (const auto& value : sl) {
    std::cout << value << " ";
}
```

## Performance

### Time Complexity

- **Search**: O(log n) average case
- **Insert**: O(log n) average case
- **Delete**: O(log n) average case
- **Iteration**: O(n) for full traversal

### Space Complexity

- **Memory**: O(n) average case
- **Additional memory**: O(n) for pointers at different levels

### Comparison with std::set

| Operation | SkipList | std::set |
|-----------|----------|----------|
| Search | O(log n) | O(log n) |
| Insert | O(log n) | O(log n) |
| Delete | O(log n) | O(log n) |
| Memory | O(n) | O(n) |
| Cache Locality | Worse | Better |

## Error Handling

The container throws exceptions in the following cases:

- `std::runtime_error` - when dereferencing null iterator
- `std::runtime_error` - when incrementing/decrementing null iterator
- `std::out_of_range` - when erasing end iterator

## Continuous Integration

The project includes simple CI with GitHub Actions for build and test verification:

### 🔄 **Automatic Checks:**
- **Project build** on Ubuntu
- **Test execution** and examples
- **Build artifacts** verification

## Requirements

- C++20 or newer
- Make (or equivalent)
- Compiler with C++20 support (g++, clang++, MSVC)

## Documentation

The project includes comprehensive documentation generated with Doxygen:

### Online Documentation
- **GitHub Pages**: [View Live Documentation](https://gleb-shall.github.io/sql-containers/)
- **Auto-deployed**: Documentation is automatically updated on every push to main branch

### Local Documentation
```bash
# Generate documentation
make docs

# View documentation
open docs/html/index.html
```

### Documentation Features
- **Complete API Reference**: All classes, methods, and functions documented
- **Code Examples**: Usage examples for all major features
- **Performance Analysis**: Time and space complexity information
- **Search Functionality**: Full-text search across all documentation
- **Cross-references**: Links between related functions and classes

## License

MIT License

## Author

Gleb Shikunov  
Contacts: st128274@student.spbu.ru
