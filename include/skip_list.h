/**
 * @file skip_list.h
 * @brief STL container "Skip List" implementation
 * @author Shikunov Gleb
 * @version 1.0
 */

#pragma once

#include <memory>
#include <random>
#include <stdexcept>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <vector>

namespace stl_containers {

/**
 * @brief STL container "Skip List"
 * 
 * Skip List is a probabilistic data structure that allows performing
 * search, insert and delete operations in O(log n) time on average.
 * 
 * @tparam T Type of elements stored in the list
 */
template<typename T>
class skip_list {
public:
    /** @brief Element type */
    using value_type = T;
    /** @brief Size type */
    using size_type = std::size_t;
    /** @brief Difference type */
    using difference_type = std::ptrdiff_t;
    /** @brief Reference type */
    using reference = value_type&;
    /** @brief Const reference type */
    using const_reference = const value_type&;
    /** @brief Pointer type */
    using pointer = value_type*;
    /** @brief Const pointer type */
    using const_pointer = const value_type*;

private:
    /**
     * @brief Skip list node
     */
    struct skip_node {
        /** @brief Node data */
        value_type data;
        /** @brief Array of pointers to next nodes at each level */
        std::vector<std::shared_ptr<skip_node>> forward;
        /** @brief Node level (number of connections) */
        size_type level;
        
        /**
         * @brief Node constructor with data copying
         * @param val Value to store
         * @param lvl Node level
         */
        skip_node(const value_type& val, size_type lvl = 0) 
            : data(val), level(lvl) {
            forward.resize(lvl + 1);
        }
        
        /**
         * @brief Node constructor with data moving
         * @param val Value to move
         * @param lvl Node level
         */
        skip_node(value_type&& val, size_type lvl = 0) 
            : data(std::move(val)), level(lvl) {
            forward.resize(lvl + 1);
        }
    };

    /** @brief Node pointer type */
    using node_ptr = std::shared_ptr<skip_node>;

    // Constants
    /** @brief Maximum list level */
    static constexpr size_type MAX_LEVEL = 32;
    /** @brief Probability of increasing node level */
    static constexpr double PROBABILITY = 0.25;

    // Data
    /** @brief Head node (dummy) */
    node_ptr head_;
    /** @brief Number of elements in the list */
    size_type size_;
    /** @brief Current maximum level */
    size_type max_level_;
    /** @brief Random number generator */
    std::mt19937 generator_;
    /** @brief Random number distribution */
    std::uniform_real_distribution<double> distribution_;

    /**
     * @brief Generate random level for new node
     * @return Random level from 0 to MAX_LEVEL-1
     */
    size_type random_level() {
        size_type level = 0;
        while (distribution_(generator_) < PROBABILITY && level < MAX_LEVEL - 1) {
            ++level;
        }
        return level;
    }

public:
    /**
     * @brief Iterator for skip_list
     */
    class iterator {
    public:
        /** @brief Iterator category */
        using iterator_category = std::bidirectional_iterator_tag;
        /** @brief Value type */
        using value_type = T;
        /** @brief Difference type */
        using difference_type = std::ptrdiff_t;
        /** @brief Pointer type */
        using pointer = value_type*;
        /** @brief Reference type */
        using reference = value_type&;

    private:
        /** @brief Pointer to current node */
        node_ptr node_;

    public:
        /** @brief Default constructor */
        iterator() : node_(nullptr) {}
        /** @brief Constructor with node */
        explicit iterator(node_ptr node) : node_(node) {}

        /**
         * @brief Dereference operator
         * @return Reference to node data
         * @throws std::runtime_error if iterator is invalid
         */
        reference operator*() const {
            if (!node_) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return node_->data;
        }

        /**
         * @brief Member access operator
         * @return Pointer to node data
         * @throws std::runtime_error if iterator is invalid
         */
        pointer operator->() const {
            if (!node_) {
                throw std::runtime_error("Accessing null iterator");
            }
            return &(node_->data);
        }

        /**
         * @brief Prefix increment
         * @return Reference to updated iterator
         */
        iterator& operator++() {
            if (node_ && !node_->forward.empty()) {
                node_ = node_->forward[0];
            } else {
                node_ = nullptr;
            }
            return *this;
        }

        /**
         * @brief Postfix increment
         * @return Copy of iterator before increment
         */
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Equality comparison operator
         * @param other Other iterator
         * @return true if iterators are equal
         */
        bool operator==(const iterator& other) const {
            return node_ == other.node_;
        }

        /**
         * @brief Inequality comparison operator
         * @param other Other iterator
         * @return true if iterators are not equal
         */
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        /**
         * @brief Get node pointer
         * @return Pointer to current node
         */
        node_ptr get_node() const { return node_; }
    };

    /**
     * @brief Const iterator for skip_list
     */
    class const_iterator {
    public:
        /** @brief Iterator category */
        using iterator_category = std::bidirectional_iterator_tag;
        /** @brief Value type */
        using value_type = T;
        /** @brief Difference type */
        using difference_type = std::ptrdiff_t;
        /** @brief Const pointer type */
        using pointer = const value_type*;
        /** @brief Const reference type */
        using reference = const value_type&;

    private:
        /** @brief Pointer to current node */
        node_ptr node_;

    public:
        /** @brief Default constructor */
        const_iterator() : node_(nullptr) {}
        /** @brief Constructor with node */
        explicit const_iterator(node_ptr node) : node_(node) {}
        /** @brief Copy constructor from regular iterator */
        const_iterator(const iterator& it) : node_(it.get_node()) {}

        /**
         * @brief Dereference operator
         * @return Const reference to node data
         * @throws std::runtime_error if iterator is invalid
         */
        reference operator*() const {
            if (!node_) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return node_->data;
        }

        /**
         * @brief Member access operator
         * @return Const pointer to node data
         * @throws std::runtime_error if iterator is invalid
         */
        pointer operator->() const {
            if (!node_) {
                throw std::runtime_error("Accessing null iterator");
            }
            return &(node_->data);
        }

        /**
         * @brief Prefix increment
         * @return Reference to updated iterator
         */
        const_iterator& operator++() {
            if (node_ && !node_->forward.empty()) {
                node_ = node_->forward[0];
            } else {
                node_ = nullptr;
            }
            return *this;
        }

        /**
         * @brief Postfix increment
         * @return Copy of iterator before increment
         */
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Equality comparison operator
         * @param other Other iterator
         * @return true if iterators are equal
         */
        bool operator==(const const_iterator& other) const {
            return node_ == other.node_;
        }

        /**
         * @brief Inequality comparison operator
         * @param other Other iterator
         * @return true if iterators are not equal
         */
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

        /**
         * @brief Get node pointer
         * @return Pointer to current node
         */
        node_ptr get_node() const { return node_; }
    };

    // Constructors
    /**
     * @brief Default constructor
     * Creates an empty skip list
     */
    skip_list() : size_(0), max_level_(0), generator_(std::random_device{}()), distribution_(0.0, 1.0) {
        head_ = std::make_shared<skip_node>(value_type{}, MAX_LEVEL);
    }

    /**
     * @brief Fill constructor
     * @param count Number of elements to insert
     * @param value Value to insert
     */
    explicit skip_list(size_type count, const value_type& value = value_type{}) 
        : skip_list() {
        for (size_type i = 0; i < count; ++i) {
            // Force add element, ignoring duplicates
            insert_impl(value);
        }
    }

    /**
     * @brief Range constructor
     * @tparam InputIt Input iterator type
     * @param first Beginning of range
     * @param last End of range
     */
    template<typename InputIt, typename = std::enable_if_t<!std::is_integral_v<InputIt>>>
    skip_list(InputIt first, InputIt last) : skip_list() {
        for (; first != last; ++first) {
            insert_impl(*first);
        }
    }

    /**
     * @brief Initializer list constructor
     * @param init Initializer list
     */
    skip_list(std::initializer_list<value_type> init) : skip_list() {
        for (const auto& value : init) {
            insert_impl(value);
        }
    }

    /**
     * @brief Copy constructor
     * @param other Other list to copy
     */
    skip_list(const skip_list& other) : skip_list() {
        for (const auto& value : other) {
            insert_impl(value);
        }
    }

    /**
     * @brief Move constructor
     * @param other Other list to move
     */
    skip_list(skip_list&& other) noexcept 
        : head_(std::move(other.head_))
        , size_(other.size_)
        , max_level_(other.max_level_)
        , generator_(std::move(other.generator_))
        , distribution_(std::move(other.distribution_)) {
        other.size_ = 0;
        other.max_level_ = 0;
    }

    /** @brief Destructor */
    ~skip_list() = default;

    /**
     * @brief Copy assignment operator
     * @param other Other list to copy
     * @return Reference to current object
     */
    skip_list& operator=(const skip_list& other) {
        if (this != &other) {
            clear();
            for (const auto& value : other) {
                insert_impl(value);
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator
     * @param other Other list to move
     * @return Reference to current object
     */
    skip_list& operator=(skip_list&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = std::move(other.head_);
            size_ = other.size_;
            max_level_ = other.max_level_;
            generator_ = std::move(other.generator_);
            distribution_ = std::move(other.distribution_);
            other.size_ = 0;
            other.max_level_ = 0;
        }
        return *this;
    }

    // Iterator access methods
    /** @brief Get begin iterator */
    iterator begin() noexcept {
        return iterator(head_->forward[0]);
    }

    /** @brief Get const begin iterator */
    const_iterator begin() const noexcept {
        return const_iterator(head_->forward[0]);
    }

    /** @brief Get const begin iterator */
    const_iterator cbegin() const noexcept {
        return begin();
    }

    /** @brief Get end iterator */
    iterator end() noexcept {
        return iterator(nullptr);
    }

    /** @brief Get const end iterator */
    const_iterator end() const noexcept {
        return const_iterator(nullptr);
    }

    /** @brief Get const end iterator */
    const_iterator cend() const noexcept {
        return end();
    }

    // Capacity
    /** @brief Check if list is empty */
    bool empty() const noexcept {
        return size_ == 0;
    }

    /** @brief Get number of elements */
    size_type size() const noexcept {
        return size_;
    }

    /** @brief Get maximum possible size */
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    // Modifiers
    /** @brief Clear all elements */
    void clear() noexcept {
        head_->forward.clear();
        head_->forward.resize(MAX_LEVEL);
        size_ = 0;
        max_level_ = 0;
    }

    /** @brief Insert element by copying */
    std::pair<iterator, bool> insert(const value_type& value) {
        return insert_impl(value);
    }

    /** @brief Insert element by moving */
    std::pair<iterator, bool> insert(value_type&& value) {
        return insert_impl(std::move(value));
    }

    /** @brief Emplace element with given arguments */
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return insert(value_type(std::forward<Args>(args)...));
    }

    /** @brief Erase element at given position */
    iterator erase(const_iterator pos) {
        if (pos == end()) {
            throw std::out_of_range("Cannot erase end iterator");
        }

        node_ptr node_to_erase = pos.get_node();
        std::vector<node_ptr> update(MAX_LEVEL, head_);
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->data < node_to_erase->data) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        
        // Check that node is actually found
        if (!current->forward[0] || current->forward[0] != node_to_erase) {
            throw std::runtime_error("Node not found in skip list");
        }

        for (size_type i = 0; i <= node_to_erase->level; ++i) {
            update[i]->forward[i] = node_to_erase->forward[i];
        }

        while (max_level_ > 0 && head_->forward[max_level_] == nullptr) {
            --max_level_;
        }

        --size_;
        return iterator(node_to_erase->forward[0]);
    }

    /** @brief Erase element with given key */
    size_type erase(const value_type& key) {
        auto it = find(key);
        if (it != end()) {
            erase(const_iterator(it.get_node()));
            return 1;
        }
        return 0;
    }

    /** @brief Swap with another list */
    void swap(skip_list& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(size_, other.size_);
        std::swap(max_level_, other.max_level_);
        std::swap(generator_, other.generator_);
        std::swap(distribution_, other.distribution_);
    }

    // Search
    /** @brief Find element by key */
    iterator find(const value_type& key) {
        return find_impl(key);
    }

    /** @brief Find element by key (const version) */
    const_iterator find(const value_type& key) const {
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            node_ptr prev = current;
            while (current->forward[i] && current->forward[i]->data < key) {
                prev = current;
                current = current->forward[i];
                // Protection against infinite loop
                if (current == prev) {
                    break;
                }
            }
        }

        current = current->forward[0];

        if (current && current->data == key) {
            return const_iterator(current);
        }

        return const_iterator(nullptr);
    }

    /** @brief Count elements with given key */
    size_type count(const value_type& key) const {
        return find(key) != end() ? 1 : 0;
    }

    /** @brief Find lower bound for key */
    iterator lower_bound(const value_type& key) {
        return lower_bound_impl(key);
    }

    /** @brief Find lower bound for key (const version) */
    const_iterator lower_bound(const value_type& key) const {
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->data < key) {
                current = current->forward[i];
            }
        }

        return const_iterator(current->forward[0]);
    }

    /** @brief Find upper bound for key */
    iterator upper_bound(const value_type& key) {
        return upper_bound_impl(key);
    }

    /** @brief Find upper bound for key (const version) */
    const_iterator upper_bound(const value_type& key) const {
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            while (current->forward[i] && !(key < current->forward[i]->data)) {
                current = current->forward[i];
            }
        }

        return const_iterator(current->forward[0]);
    }

    /** @brief Get equal range for key */
    std::pair<iterator, iterator> equal_range(const value_type& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    /** @brief Get equal range for key (const version) */
    std::pair<const_iterator, const_iterator> equal_range(const value_type& key) const {
        return {lower_bound(key), upper_bound(key)};
    }

private:
    /**
     * @brief Internal insert implementation
     * @tparam U Value type (can be reference or rvalue reference)
     * @param value Value to insert
     * @return Pair of iterator and boolean indicating if insertion occurred
     */
    template<typename U>
    std::pair<iterator, bool> insert_impl(U&& value) {
        std::vector<node_ptr> update(MAX_LEVEL, head_);
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->data < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current && current->data == value) {
            return {iterator(current), false};
        }

        size_type new_level = random_level();
        if (new_level > max_level_) {
            for (size_type i = max_level_ + 1; i <= new_level; ++i) {
                update[i] = head_;
            }
            max_level_ = new_level;
        }

        auto new_node = std::make_shared<skip_node>(std::forward<U>(value), new_level);

        for (size_type i = 0; i <= new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }

        ++size_;
        return {iterator(new_node), true};
    }

    /**
     * @brief Internal find implementation
     * @param key Key to search for
     * @return Iterator to found element or end iterator
     */
    iterator find_impl(const value_type& key) const {
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            node_ptr prev = current;
            while (current->forward[i] && current->forward[i]->data < key) {
                prev = current;
                current = current->forward[i];
                // Protection against infinite loop
                if (current == prev) {
                    break;
                }
            }
        }

        current = current->forward[0];

        if (current && current->data == key) {
            return iterator(current);
        }

        return iterator(nullptr);
    }

    /**
     * @brief Internal lower bound implementation
     * @param key Key to search for
     * @return Iterator to lower bound
     */
    iterator lower_bound_impl(const value_type& key) const {
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            while (current->forward[i] && current->forward[i]->data < key) {
                current = current->forward[i];
            }
        }

        return iterator(current->forward[0]);
    }

    /**
     * @brief Internal upper bound implementation
     * @param key Key to search for
     * @return Iterator to upper bound
     */
    iterator upper_bound_impl(const value_type& key) const {
        node_ptr current = head_;

        for (int i = max_level_; i >= 0; --i) {
            while (current->forward[i] && !(key < current->forward[i]->data)) {
                current = current->forward[i];
            }
        }

        return iterator(current->forward[0]);
    }
};

// Comparison operators
/** @brief Equality comparison operator */
template<typename T>
bool operator==(const skip_list<T>& lhs, const skip_list<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/** @brief Inequality comparison operator */
template<typename T>
bool operator!=(const skip_list<T>& lhs, const skip_list<T>& rhs) {
    return !(lhs == rhs);
}

/** @brief Less than comparison operator */
template<typename T>
bool operator<(const skip_list<T>& lhs, const skip_list<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/** @brief Less than or equal comparison operator */
template<typename T>
bool operator<=(const skip_list<T>& lhs, const skip_list<T>& rhs) {
    return !(rhs < lhs);
}

/** @brief Greater than comparison operator */
template<typename T>
bool operator>(const skip_list<T>& lhs, const skip_list<T>& rhs) {
    return rhs < lhs;
}

/** @brief Greater than or equal comparison operator */
template<typename T>
bool operator>=(const skip_list<T>& lhs, const skip_list<T>& rhs) {
    return !(lhs < rhs);
}

/** @brief Swap function */
template<typename T>
void swap(skip_list<T>& lhs, skip_list<T>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace stl_containers