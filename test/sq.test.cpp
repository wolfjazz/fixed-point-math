/* \file
 * Tests for sq.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>

#include <fpm.hpp>
using namespace fpm::type;


/// Test concept to checks whether a value with the given Sq type can be constructed from the given real value.
template< class Sq, double realValue >
concept ConstructibleFromReal = requires {
    { Sq::template fromReal<realValue> } -> std::same_as<Sq const &>;  // false if expression cannot be compiled
};

/// Checks whether a value with the given Sq type can be constructed from the given scaled value.
template< class Sq, Sq::base_t scaledValue >
concept ConstructibleFromScaled = requires {
    { Sq::template fromScaled<scaledValue> } -> std::same_as<Sq const &>;  // false if expression cannot be compiled
};

/// Checks whether a value of the given sq type can be negated.
template< class Sq >
concept Negatable = requires (Sq sq) {
    -sq;  // false if expression cannot be compiled
};

/// Checks whether the absolute value of the given sq type can be taken.
template< class Sq >
concept Absolutizable = requires (Sq sq) {
    abs(sq);  // false if expression cannot be compiled
};

/// Checks whether Sq1 can be divided by Sq2.
template< class Sq1, class Sq2 >
concept Dividable = requires (Sq1 sq1, Sq2 sq2) {
    sq1 / sq2;  // false if expression cannot be compiled
};

/// Checks whether Sq1 can be remainder-divided by Sq2.
template< class Sq1, class Sq2 >
concept RemainderDividable = requires (Sq1 sq1, Sq2 sq2) {
    sq1 % sq2;  // false if expression cannot be compiled
};

/// Checks whether a lt comparison between Sq1 and Sq2 is possible.
template< class Sq1, class Sq2 >
concept LtComparable = requires (Sq1 sq1, Sq2 sq2) {
    sq1 < sq2;  // false if expression cannot be compiled
};

/// Checks whether a gt comparison between Sq1 and Sq2 is possible.
template< class Sq1, class Sq2 >
concept GtComparable = requires (Sq1 sq1, Sq2 sq2) {
    sq1 > sq2;  // false if expression cannot be compiled
};

/// Checks whether a comparison between Sq1 and Sq2 for equality is possible.
template< class Sq1, class Sq2 >
concept EqComparable = requires (Sq1 sq1, Sq2 sq2) {
    sq1 == sq2;  // false if expression cannot be compiled
};

/// Checks whether a comparison between Sq1 and Sq2 for non-equality is possible.
template< class Sq1, class Sq2 >
concept NotEqComparable = requires (Sq1 sq1, Sq2 sq2) {
    sq1 != sq2;  // false if expression cannot be compiled
};


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Construction ----------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Construct : public ::testing::Test {
protected:
    using i16sq4_2k  =  i16sq4<-2048., 2047.9>;
    using i32sq4_2k  =  i32sq4<-2048., 2048.>;
    using i32sqm2_2k = i32sqm2<-2048., 2048.>;
    using i32sq8_2k  =  i32sq8<-2048.1, 2048.1>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Construct, sq_relimit__some_sq_type__relimited_sq_type) {
    constexpr double restrictedLimit = i32sq4_2k::realVMax/2;
    constexpr double extendedLimit = i32sq4_2k::realVMax*2;
    using restricted_sq_t   = i32sq4_2k::relimit_t<-restrictedLimit,    +restrictedLimit>;
    using restricted_l_sq_t = i32sq4_2k::relimit_t<-restrictedLimit,    i32sq4_2k::realVMax>;
    using restricted_r_sq_t = i32sq4_2k::relimit_t<i32sq4_2k::realVMin, +restrictedLimit>;
    using extended_sq_t     = i32sq4_2k::relimit_t<-extendedLimit,      +extendedLimit>;
    using extended_l_sq_t   = i32sq4_2k::relimit_t<-extendedLimit,      i32sq4_2k::realVMax>;
    using extended_r_sq_t   = i32sq4_2k::relimit_t<i32sq4_2k::realVMin, +extendedLimit>;
    using shifted_sq_l_t    = i32sq4_2k::relimit_t<-extendedLimit,      +restrictedLimit>;
    using shifted_sq_r_t    = i32sq4_2k::relimit_t<-restrictedLimit,    +extendedLimit>;

    ASSERT_TRUE((std::is_same_v< i32sq4<-restrictedLimit,    +restrictedLimit >,   restricted_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-restrictedLimit,    i32sq4_2k::realVMax>, restricted_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<i32sq4_2k::realVMin, +restrictedLimit >,   restricted_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-extendedLimit,      +extendedLimit   >,   extended_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-extendedLimit,      i32sq4_2k::realVMax>, extended_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<i32sq4_2k::realVMin, +extendedLimit   >,   extended_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-extendedLimit,      +restrictedLimit >,   shifted_sq_l_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-restrictedLimit,    +extendedLimit   >,   shifted_sq_r_t >));
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

TEST_F(SQTest_Construct, sq_from_real__negative_value_out_of_range__construction_not_possible) {
    using i16sq4_t = i16sq4<-2000., 2000.>;
    EXPECT_TRUE(( ConstructibleFromReal< i16sq4_t, -2000. > ));
    ASSERT_FALSE(( ConstructibleFromReal< i16sq4_t, -2001. > ));
}

TEST_F(SQTest_Construct, sq_from_real__positive_value_out_of_range__construction_not_possible) {
    using i16sq4_t = i16sq4<-2000., 2000.>;
    EXPECT_TRUE(( ConstructibleFromReal< i16sq4_t, +2000. > ));
    ASSERT_FALSE(( ConstructibleFromReal< i16sq4_t, +2001. > ));
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

TEST_F(SQTest_Construct, sq_from_scaled__negative_value_out_of_range__construction_not_possible) {
    using i16sq4_t = i16sq4<-2000., 2000.>;  // scaled: -32000 .. +32000
    EXPECT_TRUE(( ConstructibleFromScaled< i16sq4_t, -32000 > ));
    ASSERT_FALSE(( ConstructibleFromScaled< i16sq4_t, -32001 > ));
}

TEST_F(SQTest_Construct, sq_from_scaled__positive_value_out_of_range__construction_not_possible) {
    using i16sq4_t = i16sq4<-2000., 2000.>;  // scaled: -32000 .. +32000
    EXPECT_TRUE(( ConstructibleFromScaled< i16sq4_t, +32000 > ));
    ASSERT_FALSE(( ConstructibleFromScaled< i16sq4_t, +32001 > ));
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
    auto a = i16sq4_2k::fromSq(1024.12_i16sq4);
    i16sq4_2k b = 1024.12_i16sq4;

    ASSERT_NEAR((1024.12_i16sq4).toReal(), a.toReal(), i16sq4_2k::resolution);
    ASSERT_NEAR((1024.12_i16sq4).toReal(), b.toReal(), i16sq4_2k::resolution);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF_literal__int16_sameF) {
    i16sq4_2k a = std::move(1024.12_i16sq4);

    ASSERT_NEAR((1024.12_i16sq4).toReal(), a.toReal(), i16sq4_2k::resolution);
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
// ------------------------------------ SQ Test: Unary ------------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Unary : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Unary, sq_unary_plus__some_signed_sq_value__same_value_and_limits) {
    using i16sq4_t = i16sq4<-1000., 2000.>;
    auto a = i16sq4_t::fromReal<1567.89>;
    auto b = +a;

    ASSERT_TRUE((std::is_same_v<decltype(a), decltype(b)>));
    ASSERT_NEAR(a.toReal(), b.toReal(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_plus__some_unsigned_sq_value__same_value_and_limits) {
    using u16sq4_t = u16sq4<0., 2000.>;
    auto a = u16sq4_t::fromReal<1567.89>;
    auto b = +a;

    ASSERT_TRUE((std::is_same_v<decltype(a), decltype(b)>));
    ASSERT_NEAR(a.toReal(), b.toReal(), (std::numeric_limits<double>::epsilon()));
}

TEST_F(SQTest_Unary, sq_unary_plus__some_signed_q_value__sq_with_same_value_and_limits) {
    using i16q4_t = i16q4<-1000., 2000.>;
    auto a = i16q4_t::fromReal<-567.89>;
    auto b = +a;

    ASSERT_TRUE(( std::is_same_v< i16q4_t::sq<>, decltype(b) > ));
    ASSERT_NEAR(a.toReal(), b.toReal(), (std::numeric_limits<double>::epsilon()));
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_positive_sq_value__negated_value_and_limits) {
    using i16sq4_t = i16sq4<-500., 1000.>;
    EXPECT_TRUE(( Negatable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<567.89>;
    auto b = -a;

    using expected_result_t = i16sq4<-1000., 500.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.toReal(), b.toReal(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_negative_sq_value__negated_value_and_limits) {
    using i16sq4_t = i16sq4<-500., 1000.>;
    EXPECT_TRUE(( Negatable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<-345.67>;
    auto b = -a;

    using expected_result_t = i16sq4<-1000., 500.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.toReal(), b.toReal(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_sq_value_with_full_range__negated_value_and_same_limits) {
    using i16sq4_t = i16sq4<>;  // use full symmetric range
    EXPECT_TRUE(( Negatable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<-2047.9375>;
    auto b = -a;

    ASSERT_TRUE((std::is_same_v<decltype(a), decltype(b)>));
    ASSERT_NEAR(-a.toReal(), b.toReal(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_sq_type_with_intmin__does_not_compile) {
    constexpr double absoluteMinValue = fpm::v2s<double, -4>( std::numeric_limits<int16_t>::min() );
    using i16sq4_t = i16sq4< absoluteMinValue, i16sq4<>::realVMax >;

    ASSERT_FALSE(( Negatable< i16sq4_t > ));
}

TEST_F(SQTest_Unary, sq_unary_minus__small_unsigned_sq_value__negated_value_and_limits) {
    using u16sq4_t = u16sq4<0., 500.>;
    EXPECT_TRUE(( Negatable< u16sq4_t > ));

    auto a = u16sq4_t::fromReal<234.56>;
    auto b = -a;

    using expected_result_t = i16sq4<-500., -0.>;  // (!) -0.0 is not equal to +0.0
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.toReal(), b.toReal(), u16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__large_unsigned_sq_value__negated_value_and_limits) {
    using u16sq4_t = u16sq4<>;  // use full range
    EXPECT_TRUE(( Negatable< u16sq4_t > ));

    auto a = u16sq4_t::fromReal<234.56>;
    auto b = -a;

    using expected_result_t = i32sq4< -u16sq4_t::realVMax, -0. >;  // (!) -0.0 is not equal to +0.0
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.toReal(), b.toReal(), u16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_q_value__sq_with_negated_value_and_limits) {
    using i16q4_t = i16q4<-1000., 2000.>;
    auto a = i16q4_t::fromReal<-567.89>;
    auto b = -a;

    ASSERT_TRUE(( std::is_same_v< i16sq4<-2000., 1000.>, decltype(b) > ));
    ASSERT_NEAR(-a.toReal(), b.toReal(), (std::numeric_limits<double>::epsilon()));
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_sq_literal__negated_value_and_limits) {
    auto a = -567_i32sq7;

    using expected_result_t = i32sq7<-567.,-567.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(a)>));
    ASSERT_NEAR(-567., a.toReal(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_unsigned_sq_literal__negated_value_and_limits) {
    auto a = -356.78_u16sq7;

    using expected_result_t = i32sq7<-356.78,-356.78>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(a)>));
    ASSERT_NEAR(-356.78, a.toReal(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_negative_sq_value__absolute_value_and_limits) {
    using i16sq4_t = i16sq4<>;  // use full symmetric range
    EXPECT_TRUE(( Absolutizable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<-1897.6>;
    auto b = fpm::abs(a);  // qualified lookup
    auto c = abs(a);  // unqualified lookup (argument-dependent lookup, ADL)

    using expected_result_t = u16sq4< 0., i16sq4<>::realVMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(std::abs(a.toReal()), b.toReal(), expected_result_t::resolution);
    ASSERT_NEAR(std::abs(a.toReal()), c.toReal(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_positive_sq_value__same_value_absolute_limits) {
    using i16sq4_t = i16sq4<>;  // use full symmetric range
    EXPECT_TRUE(( Absolutizable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<+1897.6>;
    auto b = fpm::abs(a);  // qualified lookup
    auto c = abs(a);  // unqualified lookup (argument-dependent lookup, ADL)

    using expected_result_t = u16sq4< 0., i16sq4<>::realVMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(a.toReal(), b.toReal(), expected_result_t::resolution);
    ASSERT_NEAR(a.toReal(), c.toReal(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_sq_type_with_intmin__does_not_compile) {
    constexpr double absoluteMinValue = fpm::v2s<double, -8>( std::numeric_limits<int32_t>::min() );
    using i32sq8_t = i32sq8< absoluteMinValue, i32sq8<>::realVMax >;

    ASSERT_FALSE(( Absolutizable< i32sq8_t > ));
}

TEST_F(SQTest_Unary, sq_unary_abs__some_unsigned_sq_value__same_value_and_limits) {
    using u16sq4_t = u16sq4<0., 2000.>;
    EXPECT_TRUE(( Absolutizable< u16sq4_t > ));

    auto a = u16sq4_t::fromReal<1563.77>;
    auto b = fpm::abs(a);  // qualified lookup
    auto c = abs(a);  // unqualified lookup (argument-dependent lookup, ADL)

    ASSERT_TRUE((std::is_same_v<u16sq4_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16sq4_t, decltype(c)>));
    ASSERT_NEAR(a.toReal(), b.toReal(), u16sq4_t::resolution);
    ASSERT_NEAR(a.toReal(), c.toReal(), u16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_q_value__absolute_sq_value_and_limits) {
    using i16q4_t = i16q4<>;  // use full symmetric range

    auto a = i16q4_t::fromReal<-1897.6>;
    auto b = fpm::abs(a);  // qualified lookup
    auto c = abs(a);  // unqualified lookup (argument-dependent lookup, ADL)

    using expected_result_t = u16sq4< 0., i16sq4<>::realVMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(std::abs(a.toReal()), b.toReal(), expected_result_t::resolution);
    ASSERT_NEAR(std::abs(a.toReal()), c.toReal(), expected_result_t::resolution);
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

TEST_F(SQTest_Addition, sq_add__three_sq_values_same_sq_type__values_added) {
    using i32sq16_t = i32sq16<-10000., 10000.>;
    auto a = i32sq16_t::fromReal<5000.>;
    auto b = i32sq16_t::fromReal<-3333.>;
    auto c = i32sq16_t::fromReal<1333.>;

    auto d = a + b + c;

    using expected_result_t = i32sq16_t::relimit_t< 3*i32sq16_t::realVMin, 3*i32sq16_t::realVMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(3000., d.toReal(), 3*i32sq16_t::resolution);
}

TEST_F(SQTest_Addition, sq_add__three_sq_values_different_sq_type__values_added_largest_resolution) {
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

TEST_F(SQTest_Addition, sq_add__three_q_values_different_q_type__values_added_to_sq_with_largest_resolution) {
    using i32q16_t = i32q16<-500., 500.>;
    using i32q20_t = i32q20<-300., 300.>;
    auto a = i32q16_t::fromReal<-455.>;
    auto b = i32q20_t::fromReal<233.>;
    auto c = i32q16_t::fromReal<167.>;

    auto d = a + b + c;

    using expected_result_t = i32sq20<-1300., 1300.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-55., d.toReal(), 2*i32q16_t::resolution + i32q20_t::resolution);
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

TEST_F(SQTest_Multiplication, sq_multiplicate__three_values_same_type_one_int_sq_constant__values_multiplied) {
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

TEST_F(SQTest_Multiplication, sq_multiplicate__two_values_same_type_and_int_q_constant__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal<  5.5 >;
    auto b = i32sq16_t::fromReal< -2.6 >;

    auto d = a * b * 20.1_i32q16;
    auto e = a * 20.1_i32q16 * b;
    auto f = 20.1_i32q16 * a * b;

    using expected_result_t = i32sq16_t::relimit_t<-1286.4, +1286.4>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(f)>));
    ASSERT_NEAR(-287.43, d.toReal(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(-287.43, e.toReal(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(-287.43, f.toReal(), 100*i32sq16_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Division --------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Division : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Division, sq_divide__three_values_similar_sq_type__values_divided) {
    using dividend_t = i32sq16<-80., 80.>;
    using divisor_t = i32sq16<-20., -1.>;
    auto a = dividend_t::fromReal< -45. >;
    auto b = divisor_t::fromReal< -7./3 >;
    auto c = divisor_t::fromReal< -5./3 >;

    auto d = a / b / c;

    using expected_result_t = dividend_t;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-11.57142857, d.toReal(), 3*2*dividend_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__three_values_different_sq_type__values_divided_largest_resolution) {
    using dividend_t = i32sq16<-8., -2.>;
    using divisor_t = u32sq20<1., 10.>;
    auto a = dividend_t::fromReal< -7.888 >;
    auto b = divisor_t::fromReal< 2.666 >;
    auto c = divisor_t::fromReal< 8.123 >;

    auto d = a / b / c;

    using expected_result_t = i32sq20<-8., -0.02>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-0.364242236, d.toReal(), 3*2*dividend_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__three_values_similar_type_one_int_sq_constant__values_divided) {
    using i32sq16_t = i32sq16<1., 8.>;
    auto a = i32sq16_t::fromReal< 5.5 >;
    auto b = i32sq16_t::fromReal< 2.6 >;

    auto d = a / b / -2.5_i32sq16;
    auto e = a / -2.5_i32sq16 / b;
    auto f = -2.5_i32sq16 / a / b;

    using expected_result_de_t = i32sq16_t::relimit_t<-3.2, -0.05>;
    using expected_result_f_t = i32sq16_t::relimit_t<-2.5, -0.0390625>;
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_f_t, decltype(f)>));
    ASSERT_NEAR(-0.846153846, d.toReal(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(-0.846153846, e.toReal(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(-0.174825175, f.toReal(), 3*2*i32sq16_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__two_values_similar_type_and_int_q_constant__values_divided) {
    using i32sq16_t = i32sq16<10., 80.>;
    auto a = i32sq16_t::fromReal< 55.5 >;
    auto b = i32sq16_t::fromReal< 11.1 >;

    auto d = a / b / 10_i32q8;
    auto e = a / 10_i32q8 / b;
    auto f = 10_i32q8 / a / b;

    using expected_result_de_t = i32sq16_t::relimit_t<0.0125, 0.8>;
    using expected_result_f_t = i32sq16_t::relimit_t<0.0015625, 0.1>;
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_f_t, decltype(f)>));
    ASSERT_NEAR(0.5, d.toReal(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(0.5, e.toReal(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(0.016232449, f.toReal(), 3*2*i32sq16_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__divisor_has_forbidden_range__does_not_compile) {
    ASSERT_FALSE(( Dividable< u32sq20<0., 200.>, i32sq16<-2., 20.> > ));
    ASSERT_FALSE(( Dividable< u32sq20<0., 200.>, u16sq6<0.5, 10.> > ));
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Modulus ---------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Modulus : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Modulus, sq_modulo__similar_type_negative_dividend_positive_divisor__modulo_works) {
    using dividend_t = i16sq8<-8., 3.>;
    using divisor_t = i16sq8<i16sq8<>::resolution, 6.>;
    auto a = dividend_t::fromReal<-4.56>;
    auto b = divisor_t::fromReal<+3.33>;

    auto c = a % b;

    using expected_result_t = i16sq8<-6., 3.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(-1.23, c.toReal(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__similar_type_negative_dividend__negative_divisor__modulo_works) {
    using dividend_t = i16sq8<-8., 3.>;
    using divisor_t = i16sq8<-6., -i16sq8<>::resolution>;
    auto a = dividend_t::fromReal<-4.56>;
    auto b = divisor_t::fromReal<-3.33>;

    auto c = a % b;

    using expected_result_t = i16sq8<-6., 3.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(-1.23, c.toReal(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__different_type_positive_dividend_positive_divisor__modulo_works) {
    using dividend_t = u16sq8<0., 8.>;
    using divisor_t = i16sq8<i16sq8<>::resolution, 6.>;
    auto a = dividend_t::fromReal<+4.56>;
    auto b = divisor_t::fromReal<+3.33>;

    auto c = a % b;

    using expected_result_t = i16sq8<0., 6.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(1.23, c.toReal(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__different_type_positive_dividend__negative_divisor__modulo_works) {
    using dividend_t = u16sq8<0., 8.>;
    using divisor_t = i16sq8<-6., -i16sq8<>::resolution>;
    auto a = dividend_t::fromReal<+4.56>;
    auto b = divisor_t::fromReal<-3.33>;

    auto c = a % b;

    using expected_result_t = i16sq8<0., 6.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(1.23, c.toReal(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__different_type_different_f_negative_dividend_positive_divisor__modulo_works) {
    using dividend_t = i16sq8<-8., 3.>;
    using divisor_t = u16sq12<i16sq8<>::resolution, 6.>;
    auto a = dividend_t::fromReal<-4.56>;
    auto b = divisor_t::fromReal<+3.33>;

    auto c = a % b;

    using expected_result_t = i16sq12<-6., 3.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(-1.23, c.toReal(), 3*dividend_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__divisor_has_forbidden_range__does_not_compile) {
    ASSERT_FALSE(( RemainderDividable< u32sq20<0., 200.>, i32sq16<-2., 20.> > ));
    ASSERT_FALSE(( RemainderDividable< u32sq20<0., 200.>, u16sq6<0., 10.> > ));
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Comparison ------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Comparison : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Comparison, sq_lt__same_type_some_value_and_larger_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-34456.78>;
    auto b = i32sq10_t::fromReal<-16789.25>;
    auto c = i32sq10_t::fromReal<+16789.25>;
    auto d = i32sq10_t::fromReal<+89999.99>;

    ASSERT_TRUE(a < b);
    ASSERT_TRUE(a < c);
    ASSERT_TRUE(a < d);
    ASSERT_TRUE(b < c);
    ASSERT_TRUE(b < d);
    ASSERT_TRUE(c < d);
}

TEST_F(SQTest_Comparison, sq_lt__different_types_some_value_and_larger_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<-34456.78>;
    auto b = i16sq5_t::fromReal<-789.25>;
    auto c = i16sq5_t::fromReal<+689.25>;
    auto d = u16sq6_t::fromReal<+889.99>;

    EXPECT_TRUE(( LtComparable< i32sq10_t, i16sq5_t > ));
    EXPECT_TRUE(( LtComparable< i32sq10_t, u16sq6_t > ));
    EXPECT_TRUE(( LtComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a < b);
    ASSERT_TRUE(a < c);
    ASSERT_TRUE(a < d);
    ASSERT_TRUE(b < c);
    // b < d does not work
    // c < d does not work
    ASSERT_FALSE(( LtComparable< i16sq5_t, u16sq6_t > ));
}

TEST_F(SQTest_Comparison, sq_lt__same_type_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<+38976.84>;
    auto b = i32sq10_t::fromReal<+23456.43>;
    auto c = i32sq10_t::fromReal<-12345.67>;
    auto d = i32sq10_t::fromReal<-65432.19>;

    ASSERT_FALSE(a < b);
    ASSERT_FALSE(a < c);
    ASSERT_FALSE(a < d);
    ASSERT_FALSE(b < c);
    ASSERT_FALSE(b < d);
    ASSERT_FALSE(c < d);
}

TEST_F(SQTest_Comparison, sq_lt__different_types_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+34456.78>;
    auto b = u16sq6_t::fromReal<+889.99>;
    auto c = i16sq5_t::fromReal<+678.25>;
    auto d = i16sq5_t::fromReal<-567.25>;

    ASSERT_FALSE(a < b);
    ASSERT_FALSE(a < c);
    ASSERT_FALSE(a < d);
    ASSERT_FALSE(c < d);
    // b < c does not work
    // b < d does not work
}

TEST_F(SQTest_Comparison, sq_lt__same_type_same_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>;
    auto b = i32sq10_t::fromReal<+56897.129>;
    auto c = i32sq10_t::fromReal<-0.0>;
    auto d = i32sq10_t::fromReal<+0.0>;

    EXPECT_TRUE(( LtComparable< i32sq10_t, i32sq10_t > ));

    ASSERT_FALSE(a < a);
    ASSERT_FALSE(b < b);

    // +/- zero
    ASSERT_FALSE(c < c);
    ASSERT_FALSE(d < d);
    ASSERT_FALSE(c < d);
    ASSERT_FALSE(d < c);
}

TEST_F(SQTest_Comparison, sq_lt__different_types_same_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+678.25>;
    auto b = u16sq6_t::fromReal<+678.25>;
    auto c = i16sq5_t::fromReal<+678.25>;
    auto d = i16sq5_t::fromReal<+678.25>;

    EXPECT_TRUE(( LtComparable< i16sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( LtComparable< u16sq6_t, u16sq6_t > ));

    ASSERT_FALSE(a < b);
    ASSERT_FALSE(a < c);
    ASSERT_FALSE(a < d);
    // b < c does not work
    // b < d does not work
    ASSERT_FALSE(c < d);
}

TEST_F(SQTest_Comparison, sq_gt__same_type_some_value_and_smaller_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<+89999.99>;
    auto b = i32sq10_t::fromReal<+16789.25>;
    auto c = i32sq10_t::fromReal<-16789.25>;
    auto d = i32sq10_t::fromReal<-34456.78>;

    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a > c);
    ASSERT_TRUE(a > d);
    ASSERT_TRUE(b > c);
    ASSERT_TRUE(b > d);
    ASSERT_TRUE(c > d);
}

TEST_F(SQTest_Comparison, sq_gt__different_types_some_value_and_smaller_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+34456.78>;
    auto b = u16sq6_t::fromReal<+889.99>;
    auto c = i16sq5_t::fromReal<+789.25>;
    auto d = i16sq5_t::fromReal<-689.25>;

    EXPECT_TRUE(( GtComparable< i32sq10_t, i16sq5_t > ));
    EXPECT_TRUE(( GtComparable< i32sq10_t, u16sq6_t > ));
    EXPECT_TRUE(( GtComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a > c);
    ASSERT_TRUE(a > d);
    ASSERT_TRUE(c > d);
    // b > c does not work
    // b > d does not work
    ASSERT_FALSE(( GtComparable< i16sq5_t, u16sq6_t > ));
}

TEST_F(SQTest_Comparison, sq_gt__same_type_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>;
    auto b = i32sq10_t::fromReal<-12345.67>;
    auto c = i32sq10_t::fromReal<+23456.43>;
    auto d = i32sq10_t::fromReal<+38976.84>;

    ASSERT_FALSE(a > b);
    ASSERT_FALSE(a > c);
    ASSERT_FALSE(a > d);
    ASSERT_FALSE(b > c);
    ASSERT_FALSE(b > d);
    ASSERT_FALSE(c > d);
}

TEST_F(SQTest_Comparison, sq_gt__different_types_some_value_and_larger_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<-34456.78>;
    auto b = i16sq5_t::fromReal<-689.25>;
    auto c = i16sq5_t::fromReal<+789.25>;
    auto d = u16sq6_t::fromReal<+889.99>;

    ASSERT_FALSE(a > b);
    ASSERT_FALSE(a > c);
    ASSERT_FALSE(a > d);
    ASSERT_FALSE(b > c);
    // b > d does not work
    // c > d does not work
}

TEST_F(SQTest_Comparison, sq_gt__same_type_same_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>;
    auto b = i32sq10_t::fromReal<+56897.129>;
    auto c = i32sq10_t::fromReal<-0.0>;
    auto d = i32sq10_t::fromReal<+0.0>;

    EXPECT_TRUE(( GtComparable< i32sq10_t, i32sq10_t > ));

    ASSERT_FALSE(a > a);
    ASSERT_FALSE(b > b);

    // +/- zero
    ASSERT_FALSE(c > c);
    ASSERT_FALSE(d > d);
    ASSERT_FALSE(c > d);
    ASSERT_FALSE(d > c);
}

TEST_F(SQTest_Comparison, sq_gt__different_types_same_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+678.25>;
    auto b = u16sq6_t::fromReal<+678.25>;
    auto c = i16sq5_t::fromReal<+678.25>;
    auto d = i16sq5_t::fromReal<+678.25>;

    EXPECT_TRUE(( GtComparable< i16sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( GtComparable< u16sq6_t, u16sq6_t > ));

    ASSERT_FALSE(a > b);
    ASSERT_FALSE(a > c);
    ASSERT_FALSE(a > d);
    // b > c does not work
    // b > d does not work
    ASSERT_FALSE(c > d);
}

TEST_F(SQTest_Comparison, sq_equal__various_types_same_value__returns_true) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+488.7>;
    auto b = u16sq5_t::fromReal<+488.7>;
    auto c = i16sq5_t::fromReal<+488.7>;

    EXPECT_TRUE(( EqComparable< i32sq5_t, i32sq5_t > ));
    EXPECT_TRUE(( EqComparable< i32sq5_t, u16sq5_t > ));
    EXPECT_TRUE(( EqComparable< i32sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( EqComparable< u16sq5_t, u16sq5_t > ));
    EXPECT_TRUE(( EqComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a == a);
    ASSERT_TRUE(a == b);
    ASSERT_TRUE(a == c);
    ASSERT_TRUE(b == b);
    ASSERT_TRUE(c == c);
    // b == c does not work
    ASSERT_FALSE(( EqComparable< u16sq5_t, i16sq5_t > ));
}

TEST_F(SQTest_Comparison, sq_equal__various_types_different_value__returns_false) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+1488.7>;
    auto b = u16sq5_t::fromReal<+488.7>;
    auto c = i16sq5_t::fromReal<-288.4>;

    ASSERT_FALSE(a == b);
    ASSERT_FALSE(a == c);
    // b == c does not work
}

TEST_F(SQTest_Comparison, sq_not_equal__various_types_same_value__returns_false) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+488.7>;
    auto b = u16sq5_t::fromReal<+488.7>;
    auto c = i16sq5_t::fromReal<+488.7>;

    EXPECT_TRUE(( NotEqComparable< i32sq5_t, i32sq5_t > ));
    EXPECT_TRUE(( NotEqComparable< i32sq5_t, u16sq5_t > ));
    EXPECT_TRUE(( NotEqComparable< i32sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( NotEqComparable< u16sq5_t, u16sq5_t > ));
    EXPECT_TRUE(( NotEqComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_FALSE(a != a);
    ASSERT_FALSE(a != b);
    ASSERT_FALSE(a != c);
    ASSERT_FALSE(b != b);
    // b != c does not work
    ASSERT_FALSE(c != c);
    ASSERT_FALSE(( NotEqComparable< u16sq5_t, i16sq5_t > ));
}

TEST_F(SQTest_Comparison, sq_not_equal__various_types_different_value__returns_true) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+1488.7>;
    auto b = u16sq5_t::fromReal<+488.7>;
    auto c = i16sq5_t::fromReal<-288.4>;

    ASSERT_TRUE(a != b);
    ASSERT_TRUE(a != c);
    // b != c does not work
}


// EOF
