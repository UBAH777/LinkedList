#include "list.h"
#include <gtest/gtest.h>
#include <string>

using namespace BMSTU;


TEST(LinkedListTests, DefaultConstructor) {
    BMSTU::linked_list<int> ll;
    ASSERT_EQ(ll.size(), 0);
    ASSERT_TRUE(ll.empty());
    ASSERT_EQ(ll.begin(), ll.end());
}

TEST(LinkedListTests, InitializerConstructor) {
    BMSTU::linked_list<int> ll = {1, 2, 3, 4, 5, 6, 7};

    ASSERT_FALSE(ll.empty());

    ASSERT_EQ(ll.size(), 7);

    ASSERT_EQ(ll[0], 1);
    ASSERT_EQ(ll[2], 3);
    ASSERT_EQ(ll[6], 7);
}

TEST(LinkedListTests, OtherIterConstructor) {
    BMSTU::linked_list<int> ll1 = {1, 2, 3, 4, 5, 6, 7};
    BMSTU::linked_list<int> ll2(ll1.begin(), ll1.end());

    ASSERT_FALSE(ll2.empty());

    ASSERT_EQ(ll2.size(), 7);

    ASSERT_EQ(ll2[0], 1);
    ASSERT_EQ(ll2[2], 3);
    ASSERT_EQ(ll2[6], 7);
}

TEST(LinkedListTests, IsEmptyTest) {
    BMSTU::linked_list<int> ll;
    ASSERT_EQ(ll.size(), 0);
}

TEST(LinkedListTests, SizeTest) {
    BMSTU::linked_list<int> ll;
    ll.push_back(10);
    ASSERT_EQ(ll.size(), 1);
    ll.push_back(20);
    ASSERT_EQ(ll.size(), 2);
    ll.push_back(30);
    ASSERT_EQ(ll.size(), 3);

    ll.clear();
    ASSERT_EQ(ll.size(), 0);
}

TEST(LinkedListTests, PushBackTest) {
    BMSTU::linked_list<int> ll;
    ll.push_back(10);
    ASSERT_EQ(ll[ll.size() - 1], 10);
    ll.push_back(20);
    ASSERT_EQ(ll[ll.size() - 1], 20);
    ll.push_back(30);
    ASSERT_EQ(ll[ll.size() - 1], 30);
}

TEST(LinkedListTests, PushFrontTest) {
    BMSTU::linked_list<int> ll;
    ll.push_front(10);
    ASSERT_EQ(ll[0], 10);
    ll.push_front(20);
    ASSERT_EQ(ll[0], 20);
    ll.push_front(30);
    ASSERT_EQ(ll[0], 30);
}

TEST(LinkedListTests, IndexTest) {
    BMSTU::linked_list<int> ll;

    ll.push_front(10);
    ASSERT_EQ(ll[0], 10);

    ll[0] = 20;
    ASSERT_EQ(ll[0], 20);
}

TEST(LinkedListTests, CompareTest) {
    BMSTU::linked_list<int> ll1 = {1, 2, 3};

    BMSTU::linked_list<int> ll2 = {1, 2, 3};

    ASSERT_TRUE(ll1 == ll2);

    ll2.push_back(4);
    ASSERT_TRUE(ll1 != ll2);
}

TEST(LinkedListTests, SwapTest) {
    BMSTU::linked_list<int> ll1 = {1, 2, 3};
    BMSTU::linked_list<int> ll2 = {1, 2, 3, 4};
    BMSTU::linked_list<int> ll3 = {1, 2, 3};
    BMSTU::linked_list<int> ll4 = {1, 2, 3, 4};

    swap(ll1, ll2);
    ASSERT_TRUE(ll1 == ll4);
    ASSERT_TRUE(ll2 == ll3);
}

TEST(LinkedListTests, FromVector) {
    using namespace std;
    std::vector<std::string> my_vec = {"string1"s, "string3"s, "string4"s, "string5"s, "string6"s, "string7"s};
    BMSTU::linked_list<std::string> my_list(my_vec.begin(), my_vec.end());
    my_list.push_front("begin_string"s);
    my_list.push_back("end_string"s);
    my_vec.clear();
    my_vec.insert(my_vec.begin(), my_list.begin(), my_list.end());

    ASSERT_EQ((std::vector<std::string>{"begin_string"s, "string1"s, "string3"s, "string4"s, "string5"s, "string6"s, "string7"s, "end_string"s}), my_vec);
}


struct DeletionSpy {
    DeletionSpy() = default;

    explicit DeletionSpy(int &instance_counter)
            noexcept : instance_counter_ptr_(&instance_counter) {
        OnAddInstance();
    }

    DeletionSpy(const DeletionSpy &other)
            noexcept : instance_counter_ptr_(other.instance_counter_ptr_) {
        OnAddInstance();
    }

    DeletionSpy &operator=(const DeletionSpy &rhs) noexcept {
        if (this != &rhs) {
            auto rhs_copy(rhs);
            std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
        }
        return *this;
    }

    ~DeletionSpy() {
        OnDeleteInstance();
    }

private:
    void OnAddInstance() noexcept {
        if (instance_counter_ptr_) {
            ++(*instance_counter_ptr_);
        }
    }

    void OnDeleteInstance() noexcept {
        if (instance_counter_ptr_) {
            assert(*instance_counter_ptr_ != 0);
            --(*instance_counter_ptr_);
        }
    }

    int *instance_counter_ptr_ = nullptr;
};


TEST(LinkedListTests, Spy) {
    // Проверка фактического удаления элементов
    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            BMSTU::linked_list<DeletionSpy> list;
            list.push_front(DeletionSpy{item0_counter});
            list.push_front(DeletionSpy{item1_counter});
            list.push_front(DeletionSpy{item2_counter});

            ASSERT_EQ(item0_counter, 1);
            ASSERT_EQ(item1_counter, 1);
            ASSERT_EQ(item2_counter, 1);
            list.clear();
            ASSERT_EQ(item0_counter, 0);
            ASSERT_EQ(item1_counter, 0);
            ASSERT_EQ(item2_counter, 0);

            list.push_front(DeletionSpy{item0_counter});
            list.push_front(DeletionSpy{item1_counter});
            list.push_front(DeletionSpy{item2_counter});
            ASSERT_EQ(item0_counter, 1);
            ASSERT_EQ(item1_counter, 1);
            ASSERT_EQ(item2_counter, 1);
        }
        ASSERT_EQ(item0_counter, 0);
        ASSERT_EQ(item1_counter, 0);
        ASSERT_EQ(item2_counter, 0);
    }
}

struct ThrowOnCopy {
    ThrowOnCopy() = default;

    explicit ThrowOnCopy(int &copy_counter) noexcept : countdown_ptr(&copy_counter) {}

    ThrowOnCopy(const ThrowOnCopy &other) : countdown_ptr(other.countdown_ptr) {
        if (countdown_ptr) {
            if (*countdown_ptr == 0) {
                throw std::bad_alloc();
            } else {
                --(*countdown_ptr);
            }
        }
    }


    ThrowOnCopy &operator=(const ThrowOnCopy &rhs) = delete;

    int *countdown_ptr = nullptr;
};

TEST(LinkedListTests, Throw) {
    // Проверка фактического удаления элементов
    {
        bool exception_was_thrown = false;

        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            BMSTU::linked_list<ThrowOnCopy> list;
            list.push_front(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.push_front(ThrowOnCopy(copy_counter));
                ASSERT_EQ(list.size(), 2);
            } catch (const std::bad_alloc &) {
                exception_was_thrown = true;
                ASSERT_EQ(list.size(), 1);
                break;
            }
        }
        ASSERT_TRUE(exception_was_thrown);
    }
}