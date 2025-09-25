# Makefile для сборки SkipList проекта
# Простая и эффективная система сборки без внешних зависимостей

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2
INCLUDES = -Iinclude
LDFLAGS = 

# Директории
SRC_DIR = .
BUILD_DIR = build
INCLUDE_DIR = include
TESTS_DIR = tests

# Исходные файлы
HEADER = $(INCLUDE_DIR)/skip_list.h
TEST_SRC = $(TESTS_DIR)/simple_test.cpp

# Исполняемые файлы
TEST_EXEC = $(BUILD_DIR)/skip_list_tests

# Цели по умолчанию
.PHONY: all clean test help docs

all: $(TEST_EXEC)

# Создание директории сборки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Сборка тестов
$(TEST_EXEC): $(TEST_SRC) $(HEADER) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $<

# Запуск тестов
test: $(TEST_EXEC)
	@echo "=== Running SkipList Tests ==="
	@$(TEST_EXEC)
	@echo "=== All tests passed! ==="

# Очистка
clean:
	rm -rf $(BUILD_DIR)

# Генерация документации
docs:
	@echo "=== Generating Documentation ==="
	@doxygen Doxyfile
	@echo "=== Documentation generated in docs/html/ ==="
	@echo "Open docs/html/index.html in your browser to view the documentation"


# Справка
help:
	@echo "Available targets:"
	@echo "  all        - Build all executables (default)"
	@echo "  test       - Build and run tests"
	@echo "  docs       - Generate documentation with Doxygen"
	@echo "  clean      - Remove build directory"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build everything"
	@echo "  make test         # Run tests only"
	@echo "  make docs         # Generate documentation"
	@echo "  make clean        # Clean build files"

# Информация о проекте
info:
	@echo "SkipList STL Container Project"
	@echo "=============================="
	@echo "C++ Standard: C++20"
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Include directory: $(INCLUDE_DIR)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo ""
	@echo "Source files:"
	@echo "  Header: $(HEADER)"
	@echo "  Tests: $(TEST_SRC)"
	@echo "  Examples: $(EXAMPLE_BASIC_SRC), $(EXAMPLE_PERF_SRC)"
