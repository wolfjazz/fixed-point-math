/* \file
 * Tests for sq.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/sq.hpp>
#include <fpm/q.hpp>
using namespace fpm;


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test ------------------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest : public ::testing::Test {
protected:
    using i16q4 = q<int16_t, 4, -2048., 2047.9>;
    using i32q4 = q<int32_t, 4, -2048., 2048.>;
    using i32q4_sat = q<int32_t, 4, -2048., 2048., overflow::SATURATE>;
    using i32q4_ovf = q<int32_t, 4, -2048., 2048., overflow::ALLOWED>;
    using i32qm2 = q<int32_t, -2, -2048., 2048.>;
    using i32qm2_sat = q<int32_t, -2, -2048., 2048., overflow::SATURATE>;
    using i32qm2_ovf = q<int32_t, -2, -2048., 2048., overflow::ALLOWED>;
    using i32q8 = q<int32_t, 8, -2048.1, 2048.1>;
    using i32q8_sat = q<int32_t, 8, -2048.1, 2048.1, overflow::SATURATE>;
    using i32q8_ovf = q<int32_t, 8, -2048.1, 2048.1, overflow::ALLOWED>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest, sq__constexpr_int16_positiveF__expected_value) {
    constexpr double REAL_VALUE = -2047.6;
    auto sqValue = i16q4::from_real<REAL_VALUE>().to_sq();

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.to_real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(sqValue.to_real())>));

    constexpr int16_t RESULT_MEM_VALUE = -32761;
    ASSERT_EQ(RESULT_MEM_VALUE, sqValue.reveal());
    ASSERT_EQ(-2047, sqValue.to_real<int>());
    ASSERT_NEAR(REAL_VALUE, sqValue.to_real(), i16q4::sq<>::RESOLUTION);
}

TEST_F(SQTest, sq_copy_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16q4::from_real<REAL_VALUE_A>().to_sq();
    auto b = i16q4::sq<>::from_sq(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16q4::sq<>::RESOLUTION);
}

TEST_F(SQTest, sq_move_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16q4::from_real<REAL_VALUE_A>().to_sq();
    i16q4::sq<> b = std::move(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16q4::sq<>::RESOLUTION);
}

TEST_F(SQTest, sq_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>().to_sq();
    auto b = i32q8::sq<>::from_sq(a);

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another sq type
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::sq<>::RESOLUTION);
}

TEST_F(SQTest, sq_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>().to_sq();
    auto b = i32qm2::sq<>::from_sq(a);

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::sq<>::RESOLUTION + i32qm2::sq<>::RESOLUTION);
}

// EOF
