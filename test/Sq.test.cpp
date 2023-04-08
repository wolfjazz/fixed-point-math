/* \file
 * Tests for sq.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/sq.hpp>
using namespace fpm;


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test ------------------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest : public ::testing::Test {
protected:
    using i16sq4 = sq<int16_t, 4, -2048., 2047.9>;
    using i32sq4 = sq<int32_t, 4, -2048., 2048.>;
    using i32sqm2 = sq<int32_t, -2, -2048., 2048.>;
    using i32sq8 = sq<int32_t, 8, -2048.1, 2048.1>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest, sq__constexpr_int16_positiveF__expected_value) {
    constexpr double REAL_VALUE = -2047.6;
    auto sqValue = i16sq4::from_real<REAL_VALUE>();

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.to_real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(sqValue.to_real())>));

    constexpr int16_t RESULT_MEM_VALUE = -32761;
    ASSERT_EQ(RESULT_MEM_VALUE, sqValue.reveal());
    ASSERT_EQ(-2047, sqValue.to_real<int>());
    ASSERT_NEAR(REAL_VALUE, sqValue.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest, sq_copy_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16sq4::from_real<REAL_VALUE_A>();
    auto b = i16sq4(a);  // explicit copy-construction
    i16sq4 c = b;  // implicit copy-construction

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16sq4::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest, sq_move_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16sq4::from_real<REAL_VALUE_A>();
    i16sq4 b = std::move(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest, sq_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32sq4::from_real<REAL_VALUE_A>();
    auto b = i32sq8(a);  // explicit upscale-copy construction
    i32sq8 c = a;  // implicit upscale-copy construction

    // note: the error of the real value (due to rounding) is determined by the initial q4 type
    //       and does not change if the value is scaled to another q type
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32sq4::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32sq4::RESOLUTION);
}

TEST_F(SQTest, sq_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32sq4::from_real<REAL_VALUE_A>();
    auto b = i32sqm2(a);  // explicit downscale-copy construction
    i32sqm2 c = a;  // implicit downscale-copy construction

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32sq4::RESOLUTION + i32sqm2::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32sq4::RESOLUTION + i32sqm2::RESOLUTION);
}

// EOF
