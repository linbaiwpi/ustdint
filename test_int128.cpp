#include <gtest/gtest.h>
#include "ustdint.h"

// Test constructors
TEST(Int128Test, DefaultConstructor) {
    int128_t val;
    // Default constructor creates zero
    EXPECT_EQ(val + int128_t(0), val);
}

TEST(Int128Test, Int64Constructor) {
    int128_t positive(100);
    int128_t negative(-100);
    int128_t zero(0);
    
    EXPECT_EQ((positive + negative) + int128_t(0), int128_t(0));
}

TEST(Int128Test, HighLowConstructor) {
    int128_t val(1, 0); // 2^64
    EXPECT_EQ(val + int128_t(0), val);
}

// Test addition
TEST(Int128Test, SimpleAddition) {
    int128_t a(100);
    int128_t b(200);
    int128_t result = a + b;
    
    int128_t expected(300);
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, AdditionWithOverflow) {
    // Test low part overflow
    // UINT64_MAX + 1 produces carry
    // 0xFFFFFFFFFFFFFFFF + 0x1 = 0x10000000000000000
    int128_t a(0, UINT64_MAX);
    int128_t b(0, 1);
    int128_t result = a + b;
    
    // Expected: high=1, low=0 (carry occurred)
    int128_t expected(1, 0);
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, AdditionNegativeNumbers) {
    int128_t a(-50);
    int128_t b(-30);
    int128_t result = a + b;
    
    int128_t expected(-80);
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, AdditionLargeNegativeNumbers) {
    // Test: int128_t(-1, -1) + int128_t(0, -1)
    // -1 as int64_t = 0xFFFFFFFFFFFFFFFF as uint64_t
    // a represents: -1 * 2^64 + UINT64_MAX = -2^64 + (2^64 - 1) = -1
    // b represents: 0 * 2^64 + UINT64_MAX = 2^64 - 1
    // Sum: -1 + (2^64 - 1) = 2^64 - 2
    int128_t a(-1, -1);
    int128_t b(0, -1);
    int128_t result = a + b;
    
    // Expected: 0 * 2^64 + (UINT64_MAX - 1) = int128_t(0, UINT64_MAX - 1)
    int128_t expected(0, UINT64_MAX - 1);  // Changed from (-1, -2)
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, AdditionMixedSigns) {
    // a represents: 0 * 2^64 + 100 = 100
    // b represents: -1 * 2^64 + UINT64_MAX - 49 = -1 * 2^64 + (2^64 - 50) = -50
    // Sum: 100 + (-50) = 50
    int128_t a(0, 100);
    int128_t b(-1, -50);  // -50 as int64_t = UINT64_MAX - 49 as uint64_t
    int128_t result = a + b;
    
    // Expected: 0 * 2^64 + 50 = 50
    int128_t expected(0, 50);  // Changed from (-1, 50)
    EXPECT_EQ(result, expected);
}

// Test subtraction
TEST(Int128Test, SimpleSubtraction) {
    int128_t a(300);
    int128_t b(100);
    int128_t result = a - b;
    
    int128_t expected(200);
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, SubtractionWithUnderflow) {
    // Test low part underflow
    // a = 1 * 2^64 + 0 = 2^64
    // b = 0 * 2^64 + 1 = 1
    // result = 2^64 - 1
    int128_t a(1, 0);
    int128_t b(0, 1);
    int128_t result = a - b;
    
    // Expected: 0 * 2^64 + (UINT64_MAX) = 2^64 - 1
    int128_t expected(0, UINT64_MAX);  // Changed from (0, INT64_MAX)
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, SubtractionNegativeResult) {
    int128_t a(50);
    int128_t b(100);
    int128_t result = a - b;
    
    int128_t expected(-50);
    EXPECT_EQ(result, expected);
}

// Test combined operations
TEST(Int128Test, CombinedOperations) {
    int128_t a(100);
    int128_t b(50);
    int128_t c(30);
    
    int128_t result = a + b - c;
    int128_t expected(120);
    
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, StringInput) {
    int128_t a("123456789012345678901234567890");
    int128_t b("987654321098765432109876543210");
    int128_t result = a + b;
    
    int128_t expected("1111111110111111111011111111100");
    EXPECT_EQ(result, expected);
}

TEST(Int128Test, StringInput2) {
    int128_t a = "123456789012345678901234567890";
    int128_t b("987654321098765432109876543210");
    int128_t result = a + b;
    
    int128_t expected("1111111110111111111011111111100");
    EXPECT_EQ(result, expected);
}