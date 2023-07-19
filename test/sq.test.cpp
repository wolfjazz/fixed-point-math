/* \file
 * Tests for sq.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm.hpp>
using namespace fpm;


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Construction ----------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Construct : public ::testing::Test {
protected:
    using i16sq4_2k = i16sq4<-2048., 2047.9>;
    using i32sq4_2k = i32sq4<-2048., 2048.>;
    using i32sqm2_2k = i32sqm2<-2048., 2048.>;
    using i32sq8_2k = i32sq8<-2048.1, 2048.1>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Construct, sq_relimit__some_sq_type__relimited_sq_type) {
    constexpr double restrictedLimit = 1024.;
    constexpr double extendedLimit = 4096.;
    using restricted_sq_t   = i32sq4_2k::relimit_t<-restrictedLimit,    +restrictedLimit>;
    using restricted_l_sq_t = i32sq4_2k::relimit_t<-restrictedLimit,    i32sq4_2k::realVMax>;
    using restricted_r_sq_t = i32sq4_2k::relimit_t<i32sq4_2k::realVMin, +restrictedLimit>;
    using extended_sq_t     = i32sq4_2k::relimit_t<-extendedLimit,      +extendedLimit>;
    using extended_l_sq_t   = i32sq4_2k::relimit_t<-extendedLimit,      i32sq4_2k::realVMax>;
    using extended_r_sq_t   = i32sq4_2k::relimit_t<i32sq4_2k::realVMin, +extendedLimit>;
    using shifted_sq_l_t    = i32sq4_2k::relimit_t<-extendedLimit,      +restrictedLimit>;
    using shifted_sq_r_t    = i32sq4_2k::relimit_t<-restrictedLimit,    +extendedLimit>;

    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -restrictedLimit,    +restrictedLimit >,   restricted_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -restrictedLimit,    i32sq4_2k::realVMax>, restricted_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, i32sq4_2k::realVMin, +restrictedLimit >,   restricted_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -extendedLimit,      +extendedLimit   >,   extended_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -extendedLimit,      i32sq4_2k::realVMax>, extended_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, i32sq4_2k::realVMin, +extendedLimit   >,   extended_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -extendedLimit,      +restrictedLimit >,   shifted_sq_l_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -restrictedLimit,    +extendedLimit   >,   shifted_sq_r_t >));
}

TEST_F(SQTest_Construct, sq_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double realValue = -2047.6;
    auto sqValue = i16sq4_2k::fromReal<realValue>;

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.toReal<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(sqValue.toReal())>));

    constexpr int16_t resultMemValue = -32761;
    ASSERT_EQ(resultMemValue, sqValue.reveal());
    ASSERT_EQ(-2047, sqValue.toReal<int>());
    ASSERT_NEAR(realValue, sqValue.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t memValue = 31686;
    auto a = i16sq4_2k::fromScaled<+memValue>;
    auto b = i16sq4_2k::fromScaled<-memValue>;

    constexpr double RESULT_REAL_VALUE = 1980.375;
    ASSERT_EQ(+memValue, a.reveal());
    ASSERT_EQ(-memValue, b.reveal());
    ASSERT_NEAR(+RESULT_REAL_VALUE, a.toReal(), i16sq4_2k::resolution);
    ASSERT_NEAR(-RESULT_REAL_VALUE, b.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_copy_constructor__int16_someF__int16_sameF) {
    constexpr double realValueA = -1024.2;
    auto a = i16sq4_2k::fromReal<realValueA>;
    auto b = i16sq4_2k::fromSq(a);
    i16sq4_2k c = a;

    ASSERT_NEAR(realValueA, b.toReal(), i16sq4_2k::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF__int16_sameF) {
    constexpr double realValueA = -1024.2;
    auto a = i16sq4_2k::fromReal<realValueA>;
    i16sq4_2k b = std::move(a);

    ASSERT_NEAR(realValueA, b.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double realValueA = -1024.2;
    auto a = i32sq4_2k::fromReal<realValueA>;
    auto b = i32sq8_2k::fromSq(a);
    i32sq8_2k c = a;

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another sq type
    ASSERT_NEAR(realValueA, b.toReal(), i32sq4_2k::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double realValueA = -1024.2;
    auto a = i32sq4_2k::fromReal<realValueA>;
    auto b = i32sqm2_2k::fromSq(a);
    i32sqm2_2k c = a;

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR(realValueA, b.toReal(), i32sq4_2k::resolution + i32sqm2_2k::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32sq4_2k::resolution + i32sqm2_2k::resolution);
}

TEST_F(SQTest_Construct, sq_copy_constructor__int16_someF_literal__int16_sameF) {
    auto a = i16sq4_2k::fromSq(1024_i16sq4);
    i16sq4_2k b = 1024_i16sq4;

    ASSERT_NEAR((1024_i16sq4).toReal(), a.toReal(), i16sq4_2k::resolution);
    ASSERT_NEAR((1024_i16sq4).toReal(), b.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF_literal__int16_sameF) {
    i16sq4_2k a = std::move(1024_i16sq4);

    ASSERT_NEAR((1024_i16sq4).toReal(), a.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_upscale_copy_constructor__int16_someF_literal__int16_largerF) {
    auto a = i32sq8_2k::fromSq(1024_i32sq4);
    i32sq8_2k b = 1024_i32sq4;

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another sq type
    ASSERT_NEAR((1024_i32sq4).toReal(), a.toReal(), i32sq4_2k::resolution);
    ASSERT_NEAR((1024_i32sq4).toReal(), b.toReal(), i32sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_downscale_copy_constructor__int16_someF_literal__int16_smallerF) {
    auto a = i32sqm2_2k::fromSq(1024_i32sq4);
    i32sqm2_2k b = 1024_i32sq4;

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR((1024_i32sq4).toReal(), a.toReal(), i32sq4_2k::resolution + i32sqm2_2k::resolution);
    ASSERT_NEAR((1024_i32sq4).toReal(), b.toReal(), i32sq4_2k::resolution + i32sqm2_2k::resolution);
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

    using i16sqm3_t = i16sqm3<10000., 100000.>;  // i16 -> u32, max delta f is 17
    using u32sq14_t = u32sq14<10000., 160000.>;
    auto a = i16sqm3_t::fromReal<i16sqm3_t::realVMin>;
    auto b = i16sqm3_t::fromReal<70000.>;
    auto c = i16sqm3_t::fromReal<i16sqm3_t::realVMax>;
    auto ac = static_cast<u32sq14_t>(a);
    auto ac2 = static_sq_cast<u32sq14_t>(a);
    auto ac3 = safe_sq_cast<u32sq14_t>(a);
    auto bc = static_cast<u32sq14_t>(b);
    auto bc2 = static_sq_cast<u32sq14_t>(b);
    auto bc3 = safe_sq_cast<u32sq14_t>(b);
    auto cc = static_cast<u32sq14_t>(c);
    auto cc2 = static_sq_cast<u32sq14_t>(c);
    auto cc3 = safe_sq_cast<u32sq14_t>(c);

    ASSERT_NEAR(i16sqm3_t::realVMin, ac.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(i16sqm3_t::realVMin, ac2.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(i16sqm3_t::realVMin, ac3.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(70000., bc.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(70000., bc2.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(70000., bc3.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(i16sqm3_t::realVMax, cc.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(i16sqm3_t::realVMax, cc2.toReal(), i16sqm3_t::resolution);
    ASSERT_NEAR(i16sqm3_t::realVMax, cc3.toReal(), i16sqm3_t::resolution);
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

    using u16sqm3_t = u16sqm3<10000., 400000.>;  // u16 -> i32, max delta f is 15
    using i32sq12_t = i32sq12<-80000., 500000.>;
    auto a = u16sqm3_t::fromReal<u16sqm3_t::realVMin>;
    auto b = u16sqm3_t::fromReal<50000.>;
    auto c = u16sqm3_t::fromReal<u16sqm3_t::realVMax>;
    auto ac = static_cast<i32sq12_t>(a);
    auto ac2 = static_sq_cast<i32sq12_t>(a);
    auto ac3 = safe_sq_cast<i32sq12_t>(a);
    auto bc = static_cast<i32sq12_t>(b);
    auto bc2 = static_sq_cast<i32sq12_t>(b);
    auto bc3 = safe_sq_cast<i32sq12_t>(b);
    auto cc = static_cast<i32sq12_t>(c);
    auto cc2 = static_sq_cast<i32sq12_t>(c);
    auto cc3 = safe_sq_cast<i32sq12_t>(c);

    ASSERT_NEAR(u16sqm3_t::realVMin, ac.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(u16sqm3_t::realVMin, ac2.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(u16sqm3_t::realVMin, ac3.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(50000., bc.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(50000., bc2.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(50000., bc3.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(u16sqm3_t::realVMax, cc.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(u16sqm3_t::realVMax, cc2.toReal(), u16sqm3_t::resolution);
    ASSERT_NEAR(u16sqm3_t::realVMax, cc3.toReal(), u16sqm3_t::resolution);
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

    using i16sqm4_t = i16sqm4<-100000., 400000.>;  // i16 -> i32, max delta f is 16
    using i32sq12_t = i32sq12<-120000., 500000.>;
    auto a = i16sqm4_t::fromReal<i16sqm4_t::realVMin>;
    auto b = i16sqm4_t::fromReal<-50000.>;
    auto c = i16sqm4_t::fromReal<i16sqm4_t::realVMax>;
    auto ac = static_cast<i32sq12_t>(a);
    auto ac2 = static_sq_cast<i32sq12_t>(a);
    auto ac3 = safe_sq_cast<i32sq12_t>(a);
    auto bc = static_cast<i32sq12_t>(b);
    auto bc2 = static_sq_cast<i32sq12_t>(b);
    auto bc3 = safe_sq_cast<i32sq12_t>(b);
    auto cc = static_cast<i32sq12_t>(c);
    auto cc2 = static_sq_cast<i32sq12_t>(c);
    auto cc3 = safe_sq_cast<i32sq12_t>(c);

    ASSERT_NEAR(i16sqm4_t::realVMin, ac.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(i16sqm4_t::realVMin, ac2.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(i16sqm4_t::realVMin, ac3.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(-50000., bc.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(-50000., bc2.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(-50000., bc3.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(i16sqm4_t::realVMax, cc.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(i16sqm4_t::realVMax, cc2.toReal(), i16sqm4_t::resolution);
    ASSERT_NEAR(i16sqm4_t::realVMax, cc3.toReal(), i16sqm4_t::resolution);
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

    using u16sqm2_t = u16sqm2<10000., 100000.>;  // u16 -> u32, max delta f is 16
    using u32sq14_t = u32sq14<0., 160000.>;
    auto a = u16sqm2_t::fromReal<u16sqm2_t::realVMin>;
    auto b = u16sqm2_t::fromReal<50000.>;
    auto c = u16sqm2_t::fromReal<u16sqm2_t::realVMax>;
    auto ac = static_cast<u32sq14_t>(a);
    auto ac2 = static_sq_cast<u32sq14_t>(a);
    auto ac3 = safe_sq_cast<u32sq14_t>(a);
    auto bc = static_cast<u32sq14_t>(b);
    auto bc2 = static_sq_cast<u32sq14_t>(b);
    auto bc3 = safe_sq_cast<u32sq14_t>(b);
    auto cc = static_cast<u32sq14_t>(c);
    auto cc2 = static_sq_cast<u32sq14_t>(c);
    auto cc3 = safe_sq_cast<u32sq14_t>(c);

    ASSERT_NEAR(u16sqm2_t::realVMin, ac.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(u16sqm2_t::realVMin, ac2.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(u16sqm2_t::realVMin, ac3.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(50000., bc.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(50000., bc2.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(50000., bc3.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(u16sqm2_t::realVMax, cc.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(u16sqm2_t::realVMax, cc2.toReal(), u16sqm2_t::resolution);
    ASSERT_NEAR(u16sqm2_t::realVMax, cc3.toReal(), u16sqm2_t::resolution);
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
    using i32sq16_t = i32sq16<-10000., 10000.>;
    auto a = i32sq16_t::fromReal<5000.>;
    auto b = i32sq16_t::fromReal<-3333.>;
    auto c = i32sq16_t::fromReal<1333.>;

    auto d = a + b + c;

    using expected_result_t = i32sq16_t::relimit_t< 3*i32sq16_t::realVMin, 3*i32sq16_t::realVMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(3000., d.toReal(), 3*i32sq16_t::resolution);
}

TEST_F(SQTest_Addition, sq_add__three_values_different_sq_type__values_added_largest_resolution) {
    using i32sq16_t = i32sq16<-500., 500.>;
    using i32sq20_t = i32sq20<-300., 300.>;
    auto a = i32sq16_t::fromReal<-455.>;
    auto b = i32sq20_t::fromReal<233.>;
    auto c = i32sq16_t::fromReal<167.>;

    auto d = a + b + c;

    using expected_result_t = i32sq20_t::relimit_t<-1300., 1300.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-55., d.toReal(), 2*i32sq16_t::resolution + i32sq20_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Subtraction ------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Subtraction : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Subtraction, sq_subtract__three_values_same_sq_type__values_subtracted) {
    using i32sq16_t = i32sq16<-500., 500.>;
    auto a = i32sq16_t::fromReal<-455.>;
    auto b = i32sq16_t::fromReal<233.>;
    auto c = i32sq16_t::fromReal<167.>;

    auto d = a - b - c;

    using expected_result_t = i32sq16_t::relimit_t<-1500., +1500.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-855., d.toReal(), 3*i32sq16_t::resolution);
}

TEST_F(SQTest_Subtraction, sq_subtract__three_values_different_sq_type__values_subtracted_largest_resolution) {
    using i32sq16_t = i32sq16<-500., 500.>;
    using i32sq20_t = i32sq20<-300., 300.>;
    auto a = i32sq16_t::fromReal<255.1111>;
    auto b = i32sq20_t::fromReal<233.2222>;
    auto c = i32sq16_t::fromReal<167.3333>;

    auto d = a - b - c;

    using expected_result_t = i32sq20_t::relimit_t<-1300., +1300.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-145.4444, d.toReal(), 2*i32sq16_t::resolution + i32sq20_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Multiplication --------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Multiplication : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Multiplication, sq_multiplicate__three_values_same_sq_type__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal< -4.5 >;
    auto b = i32sq16_t::fromReal<  7./3 >;
    auto c = i32sq16_t::fromReal<  5./3 >;

    auto d = a * b * c;

    using expected_result_t = i32sq16_t::relimit_t<-512., +512.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-17.5, d.toReal(), 12*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__three_values_different_sq_type__values_multiplied_largest_resolution) {
    using i32sq16_t = i32sq16<-8., -2.>;
    using i32sq20_t = i32sq20<-9., 10.>;
    auto a = i32sq16_t::fromReal< -7.888 >;
    auto b = i32sq20_t::fromReal< -2.666 >;
    auto c = i32sq20_t::fromReal<  8.123 >;

    auto d = a * b * c;

    using expected_result_t = i32sq20_t::relimit_t<-800., +720.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(170.8218812, d.toReal(), 26*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__three_values_same_type_and_int_constant__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal<  5.5 >;
    auto b = i32sq16_t::fromReal< -2.6 >;

    auto d = a * b * 15_i32sq16;
    auto e = a * 15_i32sq16 * b;
    auto f = 15_i32sq16 * a * b;

    using expected_result_t = i32sq16_t::relimit_t<-960., +960.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(f)>));
    ASSERT_NEAR(-214.5, d.toReal(), 60*i32sq16_t::resolution);
    ASSERT_NEAR(-214.5, e.toReal(), 60*i32sq16_t::resolution);
    ASSERT_NEAR(-214.5, f.toReal(), 60*i32sq16_t::resolution);
}


// EOF
