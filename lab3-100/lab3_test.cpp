#include "unique_ptr.h"

#include <gtest/gtest.h>
#include <utility>

TEST(UniquePtrTest, moveConstructor) {
    auto p1 = MakeUnique<int>(1);
    const auto p2 = std::move(p1);
    EXPECT_EQ(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_EQ(*p2, 1);
}

TEST(UniquePtrTest, moveAssign) {
    auto p1 = MakeUnique<int>(1);
    UniquePtr<int> p2 {};
    p2 = std::move(p1);
    EXPECT_EQ(p1, nullptr);
    EXPECT_NE(p2, nullptr);
    EXPECT_EQ(*p2, 1);
}

TEST(UniquePtrTest, selfAssign) {
    auto p1 = MakeUnique<int>(1);
    p1 = std::move(p1);
    EXPECT_NE(p1, nullptr);
    EXPECT_EQ(*p1, 1);
}

TEST(UniquePtrTest, nullAssign) {
    auto p1 = MakeUnique<int>(1);
    p1 = nullptr;
    EXPECT_EQ(p1, nullptr);
}

TEST(UniquePtrTest, get) {
    auto p = MakeUnique<int>(42);
    int* raw = p.get();
    EXPECT_NE(raw, nullptr);
    EXPECT_EQ(*raw, 42);
    EXPECT_EQ(*p, 42);
}

TEST(UniquePtrTest, resetToNullptr) {
    auto p = MakeUnique<int>(99);
    p.reset(); // 等价于 p.reset(nullptr)
    EXPECT_EQ(p, nullptr);
}

TEST(UniquePtrTest, resetToNewPointer) {
    auto p = MakeUnique<int>(5);
    p.reset(new int(10));
    EXPECT_NE(p, nullptr);
    EXPECT_EQ(*p, 10);
}

TEST(UniquePtrTest, release) {
    auto p = MakeUnique<int>(77);
    int* raw = p.release();  // 释放所有权，p 不再管理资源
    EXPECT_EQ(p, nullptr);
    EXPECT_NE(raw, nullptr);
    EXPECT_EQ(*raw, 77);
    delete raw;  // 手动释放
}

TEST(UniquePtrTest, dereferenceOperator) {
    struct TestStruct {
        int x;
        int getDouble() const { return 2 * x; }
    };

    auto p = MakeUnique<TestStruct>(TestStruct{3});
    EXPECT_EQ(p->x, 3);
    EXPECT_EQ(p->getDouble(), 6);
    EXPECT_EQ((*p).x, 3);
}

TEST(UniquePtrTest, comparisonWithNullptr) {
    UniquePtr<int> p1;
    EXPECT_EQ(p1, nullptr);

    auto p2 = MakeUnique<int>(100);
    EXPECT_NE(p2, nullptr);
}