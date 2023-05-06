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
// ------------------------------------ SQ Test: Construction ----------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Construct : public ::testing::Test {
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

TEST_F(SQTest_Construct, sq_delimit__some_sq_type__delimited_sq_type) {
    constexpr double RESTRICTED_LIMIT = 1024.;
    constexpr double EXTENDED_LIMIT = 4096.;
    using restricted_sq_t   = i32sq4::delimit<-RESTRICTED_LIMIT,  +RESTRICTED_LIMIT>;
    using restricted_l_sq_t = i32sq4::delimit<-RESTRICTED_LIMIT,  i32sq4::REAL_V_MAX>;
    using restricted_r_sq_t = i32sq4::delimit<i32sq4::REAL_V_MIN, +RESTRICTED_LIMIT>;
    using extended_sq_t     = i32sq4::delimit<-EXTENDED_LIMIT,    +EXTENDED_LIMIT>;
    using extended_l_sq_t   = i32sq4::delimit<-EXTENDED_LIMIT,    i32sq4::REAL_V_MAX>;
    using extended_r_sq_t   = i32sq4::delimit<i32sq4::REAL_V_MIN, +EXTENDED_LIMIT>;
    using shifted_sq_l_t    = i32sq4::delimit<-EXTENDED_LIMIT,    +RESTRICTED_LIMIT>;
    using shifted_sq_r_t    = i32sq4::delimit<-RESTRICTED_LIMIT,  +EXTENDED_LIMIT>;

    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -RESTRICTED_LIMIT,  +RESTRICTED_LIMIT >, restricted_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -RESTRICTED_LIMIT,  i32sq4::REAL_V_MAX>, restricted_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, i32sq4::REAL_V_MIN, +RESTRICTED_LIMIT >, restricted_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -EXTENDED_LIMIT,    +EXTENDED_LIMIT   >, extended_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -EXTENDED_LIMIT,    i32sq4::REAL_V_MAX>, extended_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, i32sq4::REAL_V_MIN, +EXTENDED_LIMIT   >, extended_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -EXTENDED_LIMIT,    +RESTRICTED_LIMIT >, shifted_sq_l_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -RESTRICTED_LIMIT,  +EXTENDED_LIMIT   >, shifted_sq_r_t >));
}

TEST_F(SQTest_Construct, sq_from_real__constexpr_int16_positiveF__expected_value) {
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

TEST_F(SQTest_Construct, sq_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t MEM_VALUE = 31686;
    auto a = i16sq4::from_scaled<+MEM_VALUE>();
    auto b = i16sq4::from_scaled<-MEM_VALUE>();

    constexpr double RESULT_REAL_VALUE = 1980.375;
    ASSERT_EQ(+MEM_VALUE, a.reveal());
    ASSERT_EQ(-MEM_VALUE, b.reveal());
    ASSERT_NEAR(+RESULT_REAL_VALUE, a.to_real(), i16sq4::RESOLUTION);
    ASSERT_NEAR(-RESULT_REAL_VALUE, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_copy_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16sq4::from_real<REAL_VALUE_A>();
    auto b = i16sq4::from_sq(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16sq4::from_real<REAL_VALUE_A>();
    i16sq4 b = std::move(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32sq4::from_real<REAL_VALUE_A>();
    auto b = i32sq8::from_sq(a);

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another sq type
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32sq4::from_real<REAL_VALUE_A>();
    auto b = i32sqm2::from_sq(a);

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32sq4::RESOLUTION + i32sqm2::RESOLUTION);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Casting ---------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Casting : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Casting, sq_static_cast__signed_user_range__unsigned_larger_range__same_real_value) {
    // i16[min,max]:
    // i16::min               0 min         max  i16::max
    // |----------------------|--|-----------|-------|
    //                           ^^^^^^^^^^^^^  <= user value range i16
    //
    // static_cast< u32[MIN,MAX] >( i16_scaled[min,max] ):
    // 0  MIN                                          MAX    u32::max
    // |---|---|--------------|-----||------------------|---------|
    //        min            max
    //         ^^^^^^^^^^^^^^^^  <= scaled i16 user value range in u32

    using i16sqm3 = sq<int16_t, -3, 10000., 100000.>;  // i16 -> u32, max delta F is 17
    using u32sq14 = sq<uint32_t, 14, 10000., 160000.>;
    auto a = i16sqm3::from_real<i16sqm3::REAL_V_MIN>();
    auto b = i16sqm3::from_real<70000.>();
    auto c = i16sqm3::from_real<i16sqm3::REAL_V_MAX>();
    auto ac = static_cast<u32sq14>(a);
    auto ac2 = static_sq_cast<u32sq14>(a);
    auto ac3 = safe_sq_cast<u32sq14>(a);
    auto bc = static_cast<u32sq14>(b);
    auto bc2 = static_sq_cast<u32sq14>(b);
    auto bc3 = safe_sq_cast<u32sq14>(b);
    auto cc = static_cast<u32sq14>(c);
    auto cc2 = static_sq_cast<u32sq14>(c);
    auto cc3 = safe_sq_cast<u32sq14>(c);

    ASSERT_NEAR(i16sqm3::REAL_V_MIN, ac.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(i16sqm3::REAL_V_MIN, ac2.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(i16sqm3::REAL_V_MIN, ac3.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(70000., bc.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(70000., bc2.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(70000., bc3.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(i16sqm3::REAL_V_MAX, cc.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(i16sqm3::REAL_V_MAX, cc2.to_real(), i16sqm3::RESOLUTION);
    ASSERT_NEAR(i16sqm3::REAL_V_MAX, cc3.to_real(), i16sqm3::RESOLUTION);
}

TEST_F(SQTest_Casting, sq_static_cast__unsigned_user_range__signed_larger_range__same_real_value) {
    // u16[min,max]:
    // 0     min          max                    u16::max
    // |------|------------|--|----------------------|
    //        ^^^^^^^^^^^^^^  <= user value range u16
    //
    // static_cast< i32[MIN,MAX] >( u16_scaled[min,max] ):
    // i32::min  MIN                0                   MAX  i32::max
    // |----------|-----------------|---|------------|---|-------|
    //                                 min          max
    //                                  ^^^^^^^^^^^^^^  <= scaled u16 user value range in i32

    using u16sqm3 = sq<uint16_t, -3, 10000., 400000.>;  // u16 -> i32, max delta F is 15
    using i32sq12 = sq<int32_t, 12, -80000., 500000.>;
    auto a = u16sqm3::from_real<u16sqm3::REAL_V_MIN>();
    auto b = u16sqm3::from_real<50000.>();
    auto c = u16sqm3::from_real<u16sqm3::REAL_V_MAX>();
    auto ac = static_cast<i32sq12>(a);
    auto ac2 = static_sq_cast<i32sq12>(a);
    auto ac3 = safe_sq_cast<i32sq12>(a);
    auto bc = static_cast<i32sq12>(b);
    auto bc2 = static_sq_cast<i32sq12>(b);
    auto bc3 = safe_sq_cast<i32sq12>(b);
    auto cc = static_cast<i32sq12>(c);
    auto cc2 = static_sq_cast<i32sq12>(c);
    auto cc3 = safe_sq_cast<i32sq12>(c);

    ASSERT_NEAR(u16sqm3::REAL_V_MIN, ac.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(u16sqm3::REAL_V_MIN, ac2.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(u16sqm3::REAL_V_MIN, ac3.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(50000., bc.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(50000., bc2.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(50000., bc3.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(u16sqm3::REAL_V_MAX, cc.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(u16sqm3::REAL_V_MAX, cc2.to_real(), u16sqm3::RESOLUTION);
    ASSERT_NEAR(u16sqm3::REAL_V_MAX, cc3.to_real(), u16sqm3::RESOLUTION);
}

TEST_F(SQTest_Casting, sq_static_cast__signed_user_range__signed_larger_range__same_real_value) {
    // i16[min,max]:
    // i16::min     min       0       max        i16::max
    // |-------------|--------|--------|-------------|
    //               ^^^^^^^^^^^^^^^^^^^  <= i16 user value range
    //
    // static_cast< i32[MIN,MAX] >( i16_scaled[min,max] ):
    // i32::min  MIN                0                 MAX    i32::max
    // |----------|--|--------------|---------------|--|---------|
    //              min                            max
    //               ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled i16 user value range in i32

    using i16sqm4 = sq<int16_t, -4, -100000., 400000.>;  // i16 -> i32, max delta F is 16
    using i32sq12 = sq<int32_t, 12, -120000., 500000.>;
    auto a = i16sqm4::from_real<i16sqm4::REAL_V_MIN>();
    auto b = i16sqm4::from_real<-50000.>();
    auto c = i16sqm4::from_real<i16sqm4::REAL_V_MAX>();
    auto ac = static_cast<i32sq12>(a);
    auto ac2 = static_sq_cast<i32sq12>(a);
    auto ac3 = safe_sq_cast<i32sq12>(a);
    auto bc = static_cast<i32sq12>(b);
    auto bc2 = static_sq_cast<i32sq12>(b);
    auto bc3 = safe_sq_cast<i32sq12>(b);
    auto cc = static_cast<i32sq12>(c);
    auto cc2 = static_sq_cast<i32sq12>(c);
    auto cc3 = safe_sq_cast<i32sq12>(c);

    ASSERT_NEAR(i16sqm4::REAL_V_MIN, ac.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(i16sqm4::REAL_V_MIN, ac2.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(i16sqm4::REAL_V_MIN, ac3.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(-50000., bc.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(-50000., bc2.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(-50000., bc3.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(i16sqm4::REAL_V_MAX, cc.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(i16sqm4::REAL_V_MAX, cc2.to_real(), i16sqm4::RESOLUTION);
    ASSERT_NEAR(i16sqm4::REAL_V_MAX, cc3.to_real(), i16sqm4::RESOLUTION);
}

TEST_F(SQTest_Casting, q_static_cast__unsigned_user_range__unsigned_larger_range__same_real_value) {
    // u16[min,max]:
    // 0            min               max        u16::max
    // |-------------|--------|--------|-------------|
    //               ^^^^^^^^^^^^^^^^^^^  <= u16 user value range
    //
    // static_cast< u32[MIN,MAX] >( u16_scaled[min,max] ):
    // 0         MIN                                  MAX    u32::max
    // |----------|--|--------------|---------------|--|---------|
    //              min                            max
    //               ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled u16 user value range in u32

    using u16sqm2 = sq<uint16_t, -2, 10000., 100000.>;  // u16 -> u32, max delta F is 16
    using u32sq14 = sq<uint32_t, 14, 0., 160000.>;
    auto a = u16sqm2::from_real<u16sqm2::REAL_V_MIN>();
    auto b = u16sqm2::from_real<50000.>();
    auto c = u16sqm2::from_real<u16sqm2::REAL_V_MAX>();
    auto ac = static_cast<u32sq14>(a);
    auto ac2 = static_sq_cast<u32sq14>(a);
    auto ac3 = safe_sq_cast<u32sq14>(a);
    auto bc = static_cast<u32sq14>(b);
    auto bc2 = static_sq_cast<u32sq14>(b);
    auto bc3 = safe_sq_cast<u32sq14>(b);
    auto cc = static_cast<u32sq14>(c);
    auto cc2 = static_sq_cast<u32sq14>(c);
    auto cc3 = safe_sq_cast<u32sq14>(c);

    ASSERT_NEAR(u16sqm2::REAL_V_MIN, ac.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(u16sqm2::REAL_V_MIN, ac2.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(u16sqm2::REAL_V_MIN, ac3.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(50000., bc.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(50000., bc2.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(50000., bc3.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(u16sqm2::REAL_V_MAX, cc.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(u16sqm2::REAL_V_MAX, cc2.to_real(), u16sqm2::RESOLUTION);
    ASSERT_NEAR(u16sqm2::REAL_V_MAX, cc3.to_real(), u16sqm2::RESOLUTION);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Addition --------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Addition : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Addition, sq_add__three_values_same_sq_type__values_added) {
    using i32sq16 = sq<int32_t, 16, -10000., 10000.>;
    auto a = i32sq16::from_real<5000.>();
    auto b = i32sq16::from_real<-3333.>();
    auto c = i32sq16::from_real<1333.>();

    auto d = a + b + c;

    using result_t = i32sq16::delimit< 3*i32sq16::REAL_V_MIN, 3*i32sq16::REAL_V_MAX >;
    ASSERT_TRUE((std::is_same_v<result_t, decltype(d)>));
    ASSERT_NEAR(3000., d.to_real(), 3*i32sq16::RESOLUTION);
}


// EOF
