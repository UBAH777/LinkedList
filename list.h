#pragma once
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

namespace BMSTU {
    template<typename Type>
    class linked_list {
    public:

#pragma region Node
        struct Node {
            Type value;

            Node() = default;

            Node(Node *prev, const Type &val, Node *next) :
                    prev_node(prev),
                    value(val),
                    next_node(next) {}

            Node *next_node = nullptr;
            Node *prev_node = nullptr;
        };
#pragma endregion
#pragma region Итератор
        template<class ValueType>
        struct iterator {
        public:
            friend class linked_list;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = ValueType;
            using pointer = ValueType *;
            using reference = ValueType &;


            explicit iterator(Node * node) : node_(node) {}

            iterator() = default;

            iterator(const iterator<Type> &other) noexcept : node_(other.node_) {}

#pragma region Операторы
            iterator &operator=(const iterator &rhs) = default;

            bool operator==(const iterator<const Type> &rhs) const noexcept {
                return this->node_ == rhs.node_;
            }

            bool operator!=(const iterator<const Type> &rhs) const noexcept {
                return this->node_ != rhs.node_;
            }

            bool operator>(const iterator<const Type> &rhs) const noexcept {
                return this->node_ > rhs.node_;
            }

            bool operator>=(const iterator<const Type> &rhs) const noexcept {
                return this->node_ >= rhs.node_;
            }

            bool operator<(const iterator<const Type> &rhs) const noexcept {
                return this->node_ < rhs.node_;
            }

            bool operator<=(const iterator<const Type> &rhs) const noexcept {
                return this->node_ <= rhs.node_;
            }

            reference operator*() const noexcept {
                return node_->value;
            }

            pointer operator->() const noexcept {
                return &node_->value;
            }

            iterator &operator++() noexcept {
                assert(node_ != nullptr);
                node_ = node_->next_node;
                return *this;
            }

            iterator &operator--() noexcept {
                assert(node_ != nullptr);
                node_ = node_->prev_node;
                return *this;
            }

            iterator operator++(int) noexcept {
                auto this_copy(*this);
                ++(*this);
                return this_copy;
            }

            iterator operator--(int) noexcept {
                auto this_copy(*this);
                --(*this);
                return this_copy;
            }

            iterator operator+(const difference_type value) noexcept {
                iterator copy(*this);
                for (auto i = 1; i <= value; ++i) {
                    ++copy;
                }
                return copy;
            }

            iterator operator+=(const difference_type value) noexcept {
                return ((*this) + value);
            }

            iterator operator-(const difference_type value) noexcept {
                iterator copy(*this);
                for (auto i = 1; i <= value; ++i) {
                    --copy;
                }
                return copy;
            }

            friend difference_type operator-(const iterator &end, const iterator &begin) {
                difference_type result{};
                iterator copy(begin);
                for (result = 0; copy != end; ++result) {
                    copy = copy + 1;
                }
                return result;
            }

#pragma endregion

            Node *node_ = nullptr;
        };
#pragma endregion
#pragma region Юзинги
        using value_type = Type;
        using reference = value_type &;
        using const_reference = const value_type &;
        using Iterator = iterator<Type>;
        using ConstIterator = iterator<const Type>;
#pragma endregion
#pragma region Access

        Iterator begin() noexcept {
            return Iterator{head_->next_node};
        }

        Iterator end() noexcept {
            return Iterator{tail_};
        }

        ConstIterator begin() const noexcept {
            return Iterator{head_->next_node};
        }

        ConstIterator end() const noexcept {
            return Iterator{tail_};
        }

        ConstIterator cbegin() const noexcept {
            return Iterator{head_->next_node};
        }

        ConstIterator cend() const noexcept {
            return Iterator{tail_};
        }

#pragma endregion

        linked_list() {
            head_ = new Node();
            tail_ = new Node();
            head_->next_node = tail_;
            tail_->prev_node = head_;
        }

        template <typename it>
        linked_list(it begin, it end) {
            linked_list<Type> tmp;
            it beg_copy(begin);
            while (beg_copy != end) {
                tmp.push_back(*beg_copy);
                ++beg_copy;
            }
            swap(tmp);
        }

        linked_list(std::initializer_list<Type> values) {
            linked_list<Type> tmp;
            for (const auto &val : values) {
                tmp.push_back(val);
            }
            swap(tmp);
        }

        void push_front(const Type &value) {
            Node * first = head_->next_node;
            Node * new_first = new Node(head_, value, std::move(first));
            head_->next_node = new_first;
            first->prev_node = new_first;
            ++size_;
        }

        void push_back(const Type &value) {
            Node * last = tail_->prev_node;
            Node * new_last = new Node(std::move(last), value, tail_);
            tail_->prev_node = new_last;
            last->next_node = new_last;
            ++size_;
        }

        bool empty() const noexcept {
            return size_ == 0u;
        }

        size_t size() const {
            return size_;
        }

        void clear() noexcept {
            if (empty()) {
                return;
            }
            while (head_->next_node != tail_) {
                Node *next = head_->next_node;
                head_->next_node = next->next_node;
                delete next;
            }
            size_ = 0;
        }

        void swap(linked_list &other) {
            std::swap(head_, other.head_);
            std::swap(tail_, other.tail_);
            std::swap(size_, other.size_);
        }

        friend void swap(linked_list &lhs, linked_list &rhs) {
            lhs.swap(rhs);
        }

        Type operator[](size_t pos) const {
            return *(begin() + pos);
        }

        Type &operator[](size_t pos) {
            return *(begin() + pos);
        }

        friend bool operator==(const linked_list &own, const linked_list &other) {
            if (own.size_ != other.size_) {
                return false;
            } else {
                auto this_begin = own.begin();
                auto other_begin = other.begin();

                while (this_begin != own.end() && other.begin() != other.end()) {
                    if (*this_begin != *other_begin) {
                        return false;
                    }
                    ++this_begin;
                    ++other_begin;
                }
            }
            return true;
        }

        friend bool operator!=(const linked_list &own, const linked_list &other) {
            return !(own == other);
        }


        ~linked_list() {
            clear();
            delete head_;
            delete tail_;
        }

    private:
        size_t size_ = 0;
        Node *head_ = nullptr;
        Node *tail_ = nullptr;
    };
}