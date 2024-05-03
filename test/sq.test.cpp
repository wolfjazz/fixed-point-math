/* \file
 * Tests for sq.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>

#include <fpm.hpp>
using namespace fpm::types;


template< class SqT, double real >
concept ConstructibleFromReal = requires {
    { SqT::template fromReal<real>() } -> std::same_as<SqT>;
};

template< class SqT, typename SqT::base_t scaled >
concept ConstructibleFromScaled = requires {
    { SqT::template fromScaled<scaled>() } -> std::same_as<SqT>;
};

template< class SqT >
concept Negatable = requires(SqT &sq) {
    { -sq } -> fpm::detail::SqType;
};

template< class SqL, class SqR >
concept Dividable = requires(SqL &lhs, SqR &rhs) {
    { lhs / rhs } -> fpm::detail::SqType;
};

template< class SqL, class SqR >
concept ModDividable = requires(SqL &lhs, SqR &rhs) {
    { lhs % rhs } -> fpm::detail::SqType;
};

template< class SqL, class SqR >
concept EqComparable = requires(SqL &lhs, SqR &rhs) {
    requires fpm::detail::Comparable<typename SqL::base_t, typename SqR::base_t>;  // avoid fallback to standard library
    { lhs == rhs } -> std::convertible_to<bool>;
};

template< class SqL, class SqR >
concept ThreewayComparable = requires(SqL &lhs, SqR &rhs) {
    requires fpm::detail::Comparable<typename SqL::base_t, typename SqR::base_t>;  // avoid fallback to standard library
    { lhs <=> rhs } -> std::convertible_to<std::strong_ordering>;
};

template< class SqT, class Ic >
concept LeftShiftable = requires(SqT &sq, Ic ic) {
    { sq << ic } -> fpm::detail::SqType;
};

template< class SqT, class Ic >
concept RightShiftable = requires(SqT &sq, Ic ic) {
    { sq >> ic } -> fpm::detail::SqType;
};

template< class SqT >
concept Absolutizable = requires(SqT &sq) {
    { abs(sq) } -> fpm::detail::SqType;
};

template< class SqT >
concept Squarable = requires(SqT &sq) {
    { sqr(sq) } -> fpm::detail::SqType;
};

template< class SqT >
concept SquareRootable = requires(SqT &sq) {
    { sqrt(sq) } -> fpm::detail::SqType;
};

template< class SqT >
concept RSquareRootable = requires(SqT &sq) {
    { rsqrt(sq) } -> fpm::detail::SqType;
};

template< class SqT >
concept Cubeable = requires(SqT &sq) {
    { cube(sq) } -> fpm::detail::SqType;
};

template< class SqT >
concept CubeRootable = requires(SqT &sq) {
    { cbrt(sq) } -> fpm::detail::SqType;
};

template< class SqT, class SqL, class SqH >
concept Clampable = requires(SqT &sq, SqL &lo, SqH &hi) {
    requires fpm::detail::Clampable<SqT, SqL, SqH>;  // avoid fallback to standard library
    { clamp(sq, lo, hi) } -> fpm::detail::SqType;
};

template< class SqT, class SqL >
concept ClampableLower = requires(SqT &sq, SqL &lo) {
    requires fpm::detail::ImplicitlyConvertible<SqL, SqT>;  // avoid fallback to standard library
    { clampLower(sq, lo) } -> fpm::detail::SqType;
};

template< class SqT, class SqH >
concept ClampableUpper = requires(SqT &sq, SqH &hi) {
    requires fpm::detail::ImplicitlyConvertible<SqH, SqT>;  // avoid fallback to standard library
    { clampUpper(sq, hi) } -> fpm::detail::SqType;
};

template< class SqT, double lo, double hi >
concept CTClampable = requires(SqT &sq) {
    { clamp<lo, hi>(sq) } -> fpm::detail::SqType;
};

template< class SqT, double lo >
concept CTClampableLower = requires(SqT &sq) {
    { clampLower<lo>(sq) } -> fpm::detail::SqType;
};

template< class SqT, double hi >
concept CTClampableUpper = requires(SqT &sq) {
    { clampUpper<hi>(sq) } -> fpm::detail::SqType;
};


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Construction ----------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Construct : public ::testing::Test {
protected:
    using i16sq4_2k_t  = i16sq4<-2048., 2047.9>;
    using i32sq4_2k_t  = i32sq4<-2048., 2048.>;
    using i32sqm2_2k_t = i32sqm2<-2048., 2048.>;
    using i32sq8_2k_t  = i32sq8<-2048.1, 2048.1>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Construct, sq_relimit__some_sq_type__relimited_sq_type) {
    constexpr double restrictedLimit = i32sq4_2k_t::realMax/2;
    constexpr double extendedLimit = i32sq4_2k_t::realMax*2;
    using restricted_sq_t   = i32sq4_2k_t::clamp_t<-restrictedLimit,      +restrictedLimit>;
    using restricted_l_sq_t = i32sq4_2k_t::clamp_t<-restrictedLimit,       i32sq4_2k_t::realMax>;
    using restricted_r_sq_t = i32sq4_2k_t::clamp_t< i32sq4_2k_t::realMin, +restrictedLimit>;
    using extended_sq_t     = i32sq4_2k_t::clamp_t<-extendedLimit,        +extendedLimit>;
    using extended_l_sq_t   = i32sq4_2k_t::clamp_t<-extendedLimit,         i32sq4_2k_t::realMax>;
    using extended_r_sq_t   = i32sq4_2k_t::clamp_t< i32sq4_2k_t::realMin, +extendedLimit>;
    using shifted_sq_l_t    = i32sq4_2k_t::clamp_t<-extendedLimit,        +restrictedLimit>;
    using shifted_sq_r_t    = i32sq4_2k_t::clamp_t<-restrictedLimit,      +extendedLimit>;

    ASSERT_TRUE((std::is_same_v< i32sq4<-restrictedLimit,      +restrictedLimit >,     restricted_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-restrictedLimit,       i32sq4_2k_t::realMax>, restricted_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4< i32sq4_2k_t::realMin, +restrictedLimit >,     restricted_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-extendedLimit,        +extendedLimit   >,     extended_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-extendedLimit,         i32sq4_2k_t::realMax>, extended_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4< i32sq4_2k_t::realMin, +extendedLimit   >,     extended_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-extendedLimit,        +restrictedLimit >,     shifted_sq_l_t >));
    ASSERT_TRUE((std::is_same_v< i32sq4<-restrictedLimit,      +extendedLimit   >,     shifted_sq_r_t >));
}

TEST_F(SQTest_Construct, sq_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double real = -2047.6;
    auto sqValue = i16sq4_2k_t::fromReal<real>();

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.scaled())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(sqValue.real())>));

    constexpr int16_t resultMemValue = -32761;
    ASSERT_EQ(resultMemValue, sqValue.scaled());
    ASSERT_EQ(-2047, sqValue.real<int>());
    ASSERT_NEAR(real, sqValue.real(), i16sq4_2k_t::resolution);
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
    auto a = i16sq4_2k_t::fromScaled<+memValue>();
    auto b = i16sq4_2k_t::fromScaled<-memValue>();

    constexpr double RESULT_REAL_VALUE = 1980.375;
    ASSERT_EQ(+memValue, a.scaled());
    ASSERT_EQ(-memValue, b.scaled());
    ASSERT_NEAR(+RESULT_REAL_VALUE, a.real(), i16sq4_2k_t::resolution);
    ASSERT_NEAR(-RESULT_REAL_VALUE, b.real(), i16sq4_2k_t::resolution);
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
    constexpr double realA = -1024.2;
    auto a = i16sq4_2k_t::fromReal<realA>();
    auto b = i16sq4_2k_t::fromSq(a);
    i16sq4_2k_t c = a;

    ASSERT_NEAR(realA, b.real(), i16sq4_2k_t::resolution);
    ASSERT_NEAR(realA, c.real(), i16sq4_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF__int16_sameF) {
    constexpr double realA = -1024.2;
    auto a = i16sq4_2k_t::fromReal<realA>();
    i16sq4_2k_t b = std::move(a);

    ASSERT_NEAR(realA, b.real(), i16sq4_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double realA = -1024.2;
    auto a = i32sq4_2k_t::fromReal<realA>();
    auto b = i32sq8_2k_t::fromSq(a);
    i32sq8_2k_t c = a;

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another Sq type
    ASSERT_NEAR(realA, b.real(), i32sq4_2k_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32sq4_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double realA = -1024.2;
    auto a = i32sq4_2k_t::fromReal<realA>();
    auto b = i32sqm2_2k_t::fromSq(a);
    i32sqm2_2k_t c = a;

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR(realA, b.real(), i32sq4_2k_t::resolution + i32sqm2_2k_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32sq4_2k_t::resolution + i32sqm2_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_copy_constructor__int16_someF_literal__int16_sameF) {
    auto a = i16sq4_2k_t::fromSq(1024.12_i16sq4);
    i16sq4_2k_t b = 1024.12_i16sq4;

    ASSERT_NEAR((1024.12_i16sq4).real(), a.real(), i16sq4_2k_t::resolution);
    ASSERT_NEAR((1024.12_i16sq4).real(), b.real(), i16sq4_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF_literal__int16_sameF) {
    i16sq4_2k_t a = std::move(1024.12_i16sq4);

    ASSERT_NEAR((1024.12_i16sq4).real(), a.real(), i16sq4_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_upscale_copy_constructor__int16_someF_literal__int16_largerF) {
    auto a = i32sq8_2k_t::fromSq(1024_i32sq4);
    i32sq8_2k_t b = 1024_i32sq4;

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another Sq type
    ASSERT_NEAR((1024_i32sq4).real(), a.real(), i32sq4_2k_t::resolution);
    ASSERT_NEAR((1024_i32sq4).real(), b.real(), i32sq4_2k_t::resolution);
}

TEST_F(SQTest_Construct, sq_downscale_copy_constructor__int16_someF_literal__int16_smallerF) {
    auto a = i32sqm2_2k_t::fromSq(1024_i32sq4);
    i32sqm2_2k_t b = 1024_i32sq4;

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR((1024_i32sq4).real(), a.real(), i32sq4_2k_t::resolution + i32sqm2_2k_t::resolution);
    ASSERT_NEAR((1024_i32sq4).real(), b.real(), i32sq4_2k_t::resolution + i32sqm2_2k_t::resolution);
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
    auto a = i16sqm3_t::fromReal<i16sqm3_t::realMin>();
    auto b = i16sqm3_t::fromReal<70000.>();
    auto c = i16sqm3_t::fromReal<i16sqm3_t::realMax>();
    auto ac = static_cast<u32sq14_t>(a);
    auto bc = static_cast<u32sq14_t>(b);
    auto cc = static_cast<u32sq14_t>(c);

    ASSERT_NEAR(i16sqm3_t::realMin, ac.real(), i16sqm3_t::resolution);
    ASSERT_NEAR(70000., bc.real(), i16sqm3_t::resolution);
    ASSERT_NEAR(i16sqm3_t::realMax, cc.real(), i16sqm3_t::resolution);
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
    auto a = u16sqm3_t::fromReal<u16sqm3_t::realMin>();
    auto b = u16sqm3_t::fromReal<50000.>();
    auto c = u16sqm3_t::fromReal<u16sqm3_t::realMax>();
    auto ac = static_cast<i32sq12_t>(a);
    auto bc = static_cast<i32sq12_t>(b);
    auto cc = static_cast<i32sq12_t>(c);

    ASSERT_NEAR(u16sqm3_t::realMin, ac.real(), u16sqm3_t::resolution);
    ASSERT_NEAR(50000., bc.real(), u16sqm3_t::resolution);
    ASSERT_NEAR(u16sqm3_t::realMax, cc.real(), u16sqm3_t::resolution);
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
    auto a = i16sqm4_t::fromReal<i16sqm4_t::realMin>();
    auto b = i16sqm4_t::fromReal<-50000.>();
    auto c = i16sqm4_t::fromReal<i16sqm4_t::realMax>();
    auto ac = static_cast<i32sq12_t>(a);
    auto bc = static_cast<i32sq12_t>(b);
    auto cc = static_cast<i32sq12_t>(c);

    ASSERT_NEAR(i16sqm4_t::realMin, ac.real(), i16sqm4_t::resolution);
    ASSERT_NEAR(-50000., bc.real(), i16sqm4_t::resolution);
    ASSERT_NEAR(i16sqm4_t::realMax, cc.real(), i16sqm4_t::resolution);
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
    auto a = u16sqm2_t::fromReal<u16sqm2_t::realMin>();
    auto b = u16sqm2_t::fromReal<50000.>();
    auto c = u16sqm2_t::fromReal<u16sqm2_t::realMax>();
    auto ac = static_cast<u32sq14_t>(a);
    auto bc = static_cast<u32sq14_t>(b);
    auto cc = static_cast<u32sq14_t>(c);

    ASSERT_NEAR(u16sqm2_t::realMin, ac.real(), u16sqm2_t::resolution);
    ASSERT_NEAR(50000., bc.real(), u16sqm2_t::resolution);
    ASSERT_NEAR(u16sqm2_t::realMax, cc.real(), u16sqm2_t::resolution);
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
    auto a = i16sq4_t::fromReal<1567.89>();
    auto b = +a;

    ASSERT_TRUE((std::is_same_v<decltype(a), decltype(b)>));
    ASSERT_NEAR(a.real(), b.real(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_plus__some_unsigned_sq_value__same_value_and_limits) {
    using u16sq4_t = u16sq4<0., 2000.>;
    auto a = u16sq4_t::fromReal<1567.89>();
    auto b = +a;

    ASSERT_TRUE((std::is_same_v<decltype(a), decltype(b)>));
    ASSERT_NEAR(a.real(), b.real(), (std::numeric_limits<double>::epsilon()));
}

TEST_F(SQTest_Unary, sq_unary_plus__some_signed_q_value__sq_with_same_value_and_limits) {
    using i16q4_t = i16q4<-1000., 2000.>;
    auto a = i16q4_t::fromReal<-567.89>();
    auto b = +a;  // note: quick way to convert q value to its corresponding sq value

    ASSERT_TRUE(( std::is_same_v< i16q4_t::Sq<>, decltype(b) > ));
    ASSERT_NEAR(a.real(), b.real(), (std::numeric_limits<double>::epsilon()));
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_positive_sq_value__negated_value_and_limits) {
    using i16sq4_t = i16sq4<-500., 1000.>;
    EXPECT_TRUE(( Negatable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<567.89>();
    auto b = -a;

    using expected_result_t = i16sq4<-1000., 500.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.real(), b.real(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_negative_sq_value__negated_value_and_limits) {
    using i16sq4_t = i16sq4<-500., 1000.>;
    EXPECT_TRUE(( Negatable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<-345.67>();
    auto b = -a;

    using expected_result_t = i16sq4<-1000., 500.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.real(), b.real(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_sq_value_with_full_range__negated_value_and_same_limits) {
    using i16sq4_t = i16sq4<>;  // use full symmetric range
    EXPECT_TRUE(( Negatable< i16sq4_t > ));

    auto a = i16sq4_t::fromReal<-2047.9375>();
    auto b = -a;

    ASSERT_TRUE((std::is_same_v<decltype(a), decltype(b)>));
    ASSERT_NEAR(-a.real(), b.real(), i16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_sq_type_with_intmin__does_not_compile) {
    constexpr double typeMinimum = fpm::v2s<-4, double>( std::numeric_limits<int16_t>::min() );
    using i16sq4_t = i16sq4< typeMinimum, i16sq4<>::realMax >;

    ASSERT_FALSE(( Negatable< i16sq4_t > ));
}

TEST_F(SQTest_Unary, sq_unary_minus__small_unsigned_sq_value__negated_value_and_limits) {
    using u16sq4_t = u16sq4<0., 500.>;
    EXPECT_TRUE(( Negatable< u16sq4_t > ));

    auto a = u16sq4_t::fromReal<234.56>();
    auto b = -a;

    using expected_result_t = i16sq4<-500., -0.>;  // (!) -0.0 is not equal to +0.0
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.real(), b.real(), u16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__large_unsigned_sq_value__negated_value_and_limits) {
    using u16sq4_t = u16sq4<>;  // use full range
    EXPECT_TRUE(( Negatable< u16sq4_t > ));

    auto a = u16sq4_t::fromReal<234.56>();
    auto b = -a;

    using expected_result_t = i32sq4< -u16sq4_t::realMax, -0. >;  // (!) -0.0 is not equal to +0.0
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(b)>));
    ASSERT_NEAR(-a.real(), b.real(), u16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_q_value__sq_with_negated_value_and_limits) {
    using i16q4_t = i16q4<-1000., 2000.>;
    auto a = i16q4_t::fromReal<-567.89>();
    auto b = -a;

    ASSERT_TRUE(( std::is_same_v< i16sq4<-2000., 1000.>, decltype(b) > ));
    ASSERT_NEAR(-a.real(), b.real(), (std::numeric_limits<double>::epsilon()));
}

TEST_F(SQTest_Unary, sq_unary_minus__some_signed_sq_literal__negated_value_and_limits) {
    auto a = -567_i32sq7;

    using expected_result_t = i32sq7<-567.,-567.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(a)>));
    ASSERT_NEAR(-567., a.real(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_minus__some_unsigned_sq_literal__negated_value_and_limits) {
    auto a = -356.78_u16sq7;

    using expected_result_t = i32sq7<-356.78,-356.78>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(a)>));
    ASSERT_NEAR(-356.78, a.real(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_negative_sq_value__absolute_value_and_limits) {
    using i16sq4_t = i16sq4<>;  // use full symmetric range
    EXPECT_TRUE(( Absolutizable< i16sq4_t > ));

    auto value = i16sq4_t::fromReal<-1897.6>();
    auto absValue = abs(value);  // unqualified lookup (argument-dependent lookup, ADL)

    using expected_result_t = u16sq4< 0., i16sq4<>::realMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(absValue)>));
    ASSERT_NEAR(std::abs(value.real()), absValue.real(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_positive_sq_value__same_value_absolute_limits) {
    using i16sq4_t = i16sq4<>;  // use full symmetric range
    EXPECT_TRUE(( Absolutizable< i16sq4_t > ));

    auto value = i16sq4_t::fromReal<+1897.6>();
    auto absValue = abs(value);

    using expected_result_t = u16sq4< 0., i16sq4<>::realMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(absValue)>));
    ASSERT_NEAR(value.real(), absValue.real(), expected_result_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_sq_type_with_intmin__does_not_compile) {
    constexpr double typeMinimum = fpm::v2s<-8, double>( std::numeric_limits<int32_t>::min() );
    using i32sq8_t = i32sq8< typeMinimum, i32sq8<>::realMax >;

    ASSERT_FALSE(( Absolutizable< i32sq8_t > ));
}

TEST_F(SQTest_Unary, sq_unary_abs__some_unsigned_sq_value__same_value_and_limits) {
    using u16sq4_t = u16sq4<0., 2000.>;
    EXPECT_TRUE(( Absolutizable< u16sq4_t > ));

    auto value = u16sq4_t::fromReal<1563.77>();
    auto absValue = abs(value);

    ASSERT_TRUE((std::is_same_v<u16sq4_t, decltype(absValue)>));
    ASSERT_NEAR(value.real(), absValue.real(), u16sq4_t::resolution);
}

TEST_F(SQTest_Unary, sq_unary_abs__some_signed_q_value__absolute_sq_value_and_limits) {
    using i16q4_t = i16q4<>;  // use full symmetric range

    auto value = i16q4_t::fromReal<-1897.6>();
    auto absValue = abs(value);

    using expected_result_t = u16sq4< 0., i16sq4<>::realMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(absValue)>));
    ASSERT_NEAR(std::abs(value.real()), absValue.real(), expected_result_t::resolution);
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
    auto a = i32sq16_t::fromReal<5000.>();
    auto b = i32sq16_t::fromReal<-3333.>();
    auto c = i32sq16_t::fromReal<1333.>();

    auto d = a + b + c;

    using expected_result_t = i32sq16_t::clamp_t< 3*i32sq16_t::realMin, 3*i32sq16_t::realMax >;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(3000., d.real(), 3*i32sq16_t::resolution);
}

TEST_F(SQTest_Addition, sq_add__three_sq_values_different_sq_type__values_added_largest_resolution) {
    using i32sq16_t = i32sq16<-500., 500.>;
    using i32sq20_t = i32sq20<-300., 300.>;
    auto a = i32sq16_t::fromReal<-455.>();
    auto b = i32sq20_t::fromReal<233.>();
    auto c = i32sq16_t::fromReal<167.>();

    auto d = a + b + c;

    using expected_result_t = i32sq20_t::clamp_t<-1300., 1300.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-55., d.real(), 2*i32sq16_t::resolution + i32sq20_t::resolution);
}

TEST_F(SQTest_Addition, sq_add__three_q_values_different_q_type__values_added_to_sq_with_largest_resolution) {
    using i32q16_t = i32q16<-500., 500.>;
    using i32q20_t = i32q20<-300., 300.>;
    auto a = i32q16_t::fromReal<-455.>();
    auto b = i32q20_t::fromReal<233.>();
    auto c = i32q16_t::fromReal<167.>();

    auto d = a + b + c;

    using expected_result_t = i32sq20<-1300., 1300.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-55., d.real(), 2*i32q16_t::resolution + i32q20_t::resolution);
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
    auto a = i32sq16_t::fromReal<-455.>();
    auto b = i32sq16_t::fromReal<233.>();
    auto c = i32sq16_t::fromReal<167.>();

    auto d = a - b - c;

    using expected_result_t = i32sq16_t::clamp_t<-1500., +1500.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-855., d.real(), 3*i32sq16_t::resolution);
}

TEST_F(SQTest_Subtraction, sq_subtract__three_values_different_sq_type__values_subtracted_largest_resolution) {
    using i32sq16_t = i32sq16<-500., 500.>;
    using i32sq20_t = i32sq20<-300., 300.>;
    auto a = i32sq16_t::fromReal<255.1111>();
    auto b = i32sq20_t::fromReal<233.2222>();
    auto c = i32sq16_t::fromReal<167.3333>();

    auto d = a - b - c;

    using expected_result_t = i32sq20_t::clamp_t<-1300., +1300.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-145.4444, d.real(), 2*i32sq16_t::resolution + i32sq20_t::resolution);
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
    auto a = i32sq16_t::fromReal< -4.5 >();
    auto b = i32sq16_t::fromReal<  7./3 >();
    auto c = i32sq16_t::fromReal<  5./3 >();

    auto d = a * b * c;

    using expected_result_t = i32sq16_t::clamp_t<-512., +512.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-17.5, d.real(), 12*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__three_values_different_sq_type__values_multiplied_largest_resolution) {
    using i32sq16_t = i32sq16<-8., -2.>;
    using i32sq20_t = i32sq20<-9., 10.>;
    auto a = i32sq16_t::fromReal< -7.888 >();
    auto b = i32sq20_t::fromReal< -2.666 >();
    auto c = i32sq20_t::fromReal<  8.123 >();

    auto d = a * b * c;

    using expected_result_t = i32sq20_t::clamp_t<-800., +720.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(170.8218812, d.real(), 26*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__three_values_same_type_one_int_sq_constant__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal<  5.5 >();
    auto b = i32sq16_t::fromReal< -2.6 >();

    auto d = a * b * 15_i32sq16;
    auto e = a * 15_i32sq16 * b;
    auto f = 15_i32sq16 * a * b;

    using expected_result_t = i32sq16_t::clamp_t<-960., +960.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(f)>));
    ASSERT_NEAR(-214.5, d.real(), 60*i32sq16_t::resolution);
    ASSERT_NEAR(-214.5, e.real(), 60*i32sq16_t::resolution);
    ASSERT_NEAR(-214.5, f.real(), 60*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__two_values_same_type_and_int_q_constant__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal<  5.5 >();
    auto b = i32sq16_t::fromReal< -2.6 >();

    auto d = a * b * 20.1_i32q16;
    auto e = a * 20.1_i32q16 * b;
    auto f = 20.1_i32q16 * a * b;

    using expected_result_t = i32sq16_t::clamp_t<-1286.4, +1286.4>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(f)>));
    ASSERT_NEAR(-287.43, d.real(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(-287.43, e.real(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(-287.43, f.real(), 100*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__sq_values_with_positive_integral_constant__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal<  5.5 >();
    auto b = i32sq16_t::fromReal< -2.6 >();

    auto d = a * b * 20_ic;
    auto e = a * 20_ic * b;
    auto f = 20_ic * a * b;

    using expected_result_t = i32sq16_t::clamp_t<-1280., +1280.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(f)>));
    ASSERT_NEAR(-286., d.real(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(-286., e.real(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(-286., f.real(), 100*i32sq16_t::resolution);
}

TEST_F(SQTest_Multiplication, sq_multiplicate__sq_values_with_negative_integral_constant__values_multiplied) {
    using i32sq16_t = i32sq16<-8., 8.>;
    auto a = i32sq16_t::fromReal<  5.5 >();
    auto b = i32sq16_t::fromReal< -2.6 >();

    auto d = a * b * -20_ic;
    auto e = a * -20_ic * b;
    auto f = -20_ic * a * b;

    using expected_result_t = i32sq16_t::clamp_t<-1280., +1280.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(f)>));
    ASSERT_NEAR(286., d.real(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(286., e.real(), 100*i32sq16_t::resolution);
    ASSERT_NEAR(286., f.real(), 100*i32sq16_t::resolution);
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
    auto a = dividend_t::fromReal< -45. >();
    auto b = divisor_t::fromReal< -7./3 >();
    auto c = divisor_t::fromReal< -5./3 >();

    auto d = a / b / c;

    using expected_result_t = dividend_t;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-11.57142857, d.real(), 3*2*dividend_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__three_values_different_sq_type__values_divided_largest_resolution) {
    using dividend_t = i32sq16<-8., -2.>;
    using divisor_t = u32sq20<1., 10.>;
    auto a = dividend_t::fromReal< -7.888 >();
    auto b = divisor_t::fromReal< 2.666 >();
    auto c = divisor_t::fromReal< 8.123 >();

    auto d = a / b / c;

    using expected_result_t = i32sq20<-8., -0.02>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(d)>));
    ASSERT_NEAR(-0.364242236, d.real(), 3*2*dividend_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__three_values_similar_type_one_int_sq_constant__values_divided) {
    using i32sq16_t = i32sq16<1., 8.>;
    auto a = i32sq16_t::fromReal< 5.5 >();
    auto b = i32sq16_t::fromReal< 2.6 >();

    auto d = a / b / -2.5_i32sq16;
    auto e = a / -2.5_i32sq16 / b;
    auto f = -2.5_i32sq16 / a / b;

    using expected_result_de_t = i32sq16_t::clamp_t<-3.2, -0.05>;
    using expected_result_f_t = i32sq16_t::clamp_t<-2.5, -0.0390625>;
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_f_t, decltype(f)>));
    ASSERT_NEAR(-0.846153846, d.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(-0.846153846, e.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(-0.174825175, f.real(), 3*2*i32sq16_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__two_values_similar_type_and_int_q_constant__values_divided) {
    using i32sq16_t = i32sq16<10., 80.>;
    auto a = i32sq16_t::fromReal< 55.5 >();
    auto b = i32sq16_t::fromReal< 11.1 >();

    auto d = a / b / 10_i32q8;
    auto e = a / 10_i32q8 / b;
    auto f = 10_i32q8 / a / b;

    using expected_result_de_t = i32sq16_t::clamp_t<0.0125, 0.8>;
    using expected_result_f_t = i32sq16_t::clamp_t<0.0015625, 0.1>;
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_f_t, decltype(f)>));
    ASSERT_NEAR(0.5, d.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(0.5, e.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(0.016232449, f.real(), 3*2*i32sq16_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__divisor_has_forbidden_range__does_not_compile) {
    EXPECT_TRUE(( Dividable< u32sq20<0., 200.>, i32sq16<2., 20.> > ));  // concept sanity check
    ASSERT_FALSE(( Dividable< u32sq20<0., 200.>, i32sq16<-2., 20.> > ));
    ASSERT_FALSE(( Dividable< u32sq20<0., 200.>, u16sq6<0.5, 10.> > ));
}

TEST_F(SQTest_Division, sq_divide__two_values_similar_type_and_positive_integral_constant__values_divided) {
    using i32sq16_t = i32sq16<10., 80.>;
    auto a = i32sq16_t::fromReal< 55.5 >();
    auto b = i32sq16_t::fromReal< 11.1 >();

    auto d = a / b / 10_ic;
    auto e = a / 10_ic / b;
    auto f = 10_ic / a / b;

    using expected_result_de_t = i32sq16_t::clamp_t<0.0125, 0.8>;
    using expected_result_f_t = i32sq16_t::clamp_t<0.0015625, 0.1>;
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_f_t, decltype(f)>));
    ASSERT_NEAR(0.5, d.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(0.5, e.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(0.016232449, f.real(), 3*2*i32sq16_t::resolution);
}

TEST_F(SQTest_Division, sq_divide__two_values_similar_type_and_negative_integral_constant__values_divided) {
    using i32sq16_t = i32sq16<10., 80.>;
    auto a = i32sq16_t::fromReal< 55.5 >();
    auto b = i32sq16_t::fromReal< 11.1 >();

    auto d = a / b / -10_ic;
    auto e = a / -10_ic / b;
    auto f = -10_ic / a / b;

    using expected_result_de_t = i32sq16_t::clamp_t<-0.8, -0.0125>;
    using expected_result_f_t = i32sq16_t::clamp_t<-0.1, -0.0015625>;
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(d)>));
    ASSERT_TRUE((std::is_same_v<expected_result_de_t, decltype(e)>));
    ASSERT_TRUE((std::is_same_v<expected_result_f_t, decltype(f)>));
    ASSERT_NEAR(-0.5, d.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(-0.5, e.real(), 3*2*i32sq16_t::resolution);
    ASSERT_NEAR(-0.016232449, f.real(), 3*2*i32sq16_t::resolution);
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
    auto a = dividend_t::fromReal<-4.56>();
    auto b = divisor_t::fromReal<+3.33>();

    auto c = a % b;

    using expected_result_t = i16sq8<-6., 3.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(-1.23, c.real(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__similar_type_negative_dividend__negative_divisor__modulo_works) {
    using dividend_t = i16sq8<-8., 3.>;
    using divisor_t = i16sq8<-6., -i16sq8<>::resolution>;
    auto a = dividend_t::fromReal<-4.56>();
    auto b = divisor_t::fromReal<-3.33>();

    auto c = a % b;

    using expected_result_t = i16sq8<-6., 3.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(-1.23, c.real(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__different_type_positive_dividend_positive_divisor__modulo_works) {
    using dividend_t = u16sq8<0., 8.>;
    using divisor_t = i16sq8<i16sq8<>::resolution, 6.>;
    auto a = dividend_t::fromReal<+4.56>();
    auto b = divisor_t::fromReal<+3.33>();

    auto c = a % b;

    using expected_result_t = i16sq8<0., 6.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(1.23, c.real(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__different_type_positive_dividend__negative_divisor__modulo_works) {
    using dividend_t = u16sq8<0., 8.>;
    using divisor_t = i16sq8<-6., -i16sq8<>::resolution>;
    auto a = dividend_t::fromReal<+4.56>();
    auto b = divisor_t::fromReal<-3.33>();

    auto c = a % b;

    using expected_result_t = i16sq8<0., 6.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(1.23, c.real(), 3*expected_result_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__different_type_different_f_negative_dividend_positive_divisor__modulo_works) {
    using dividend_t = i16sq8<-8., 3.>;
    using divisor_t = u16sq12<i16sq8<>::resolution, 6.>;
    auto a = dividend_t::fromReal<-4.56>();
    auto b = divisor_t::fromReal<+3.33>();

    auto c = a % b;

    using expected_result_t = i16sq12<-6., 3.>;
    ASSERT_TRUE((std::is_same_v<expected_result_t, decltype(c)>));
    ASSERT_NEAR(-1.23, c.real(), 3*dividend_t::resolution);
}

TEST_F(SQTest_Modulus, sq_modulo__divisor_has_forbidden_range__does_not_compile) {
    EXPECT_TRUE(( ModDividable< u32sq20<0., 200.>, i32sq16<2., 20.> > ));  // concept sanity check
    ASSERT_FALSE(( ModDividable< u32sq20<0., 200.>, i32sq16<-2., 20.> > ));
    ASSERT_FALSE(( ModDividable< u32sq20<0., 200.>, u16sq6<0., 10.> > ));
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
    auto a = i32sq10_t::fromReal<-34456.78>();
    auto b = i32sq10_t::fromReal<-16789.25>();
    auto c = i32sq10_t::fromReal<+16789.25>();
    auto d = i32sq10_t::fromReal<+89999.99>();

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
    auto a = i32sq10_t::fromReal<-34456.78>();
    auto b = i16sq5_t::fromReal<-789.25>();
    auto c = i16sq5_t::fromReal<+689.25>();
    auto d = u16sq6_t::fromReal<+889.99>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, u16sq6_t > ));
    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a < b);
    ASSERT_TRUE(a < c);
    ASSERT_TRUE(a < d);
    ASSERT_TRUE(b < c);
    // b < d does not work
    // c < d does not work
    ASSERT_FALSE(( ThreewayComparable< i16sq5_t, u16sq6_t > ));
}

TEST_F(SQTest_Comparison, sq_lt__same_type_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<+38976.84>();
    auto b = i32sq10_t::fromReal<+23456.43>();
    auto c = i32sq10_t::fromReal<-12345.67>();
    auto d = i32sq10_t::fromReal<-65432.19>();

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
    auto a = i32sq10_t::fromReal<+34456.78>();
    auto b = u16sq6_t::fromReal<+889.99>();
    auto c = i16sq5_t::fromReal<+678.25>();
    auto d = i16sq5_t::fromReal<-567.25>();

    ASSERT_FALSE(a < b);
    ASSERT_FALSE(a < c);
    ASSERT_FALSE(a < d);
    ASSERT_FALSE(c < d);
    // b < c does not work
    // b < d does not work
}

TEST_F(SQTest_Comparison, sq_lt__same_type_same_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>();
    auto b = i32sq10_t::fromReal<+56897.129>();
    auto c = i32sq10_t::fromReal<-0.0>();
    auto d = i32sq10_t::fromReal<+0.0>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i32sq10_t > ));

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
    auto a = i32sq10_t::fromReal<+678.25>();
    auto b = u16sq6_t::fromReal<+678.25>();
    auto c = i16sq5_t::fromReal<+678.25>();
    auto d = i16sq5_t::fromReal<+678.25>();

    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< u16sq6_t, u16sq6_t > ));

    ASSERT_FALSE(a < b);
    ASSERT_FALSE(a < c);
    ASSERT_FALSE(a < d);
    // b < c does not work
    // b < d does not work
    ASSERT_FALSE(c < d);
}

TEST_F(SQTest_Comparison, sq_lteq__same_type_some_value_and_larger_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-34456.78>();
    auto b = i32sq10_t::fromReal<-16789.25>();
    auto c = i32sq10_t::fromReal<+16789.25>();
    auto d = i32sq10_t::fromReal<+89999.99>();

    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a <= c);
    ASSERT_TRUE(a <= d);
    ASSERT_TRUE(b <= c);
    ASSERT_TRUE(b <= d);
    ASSERT_TRUE(c <= d);
}

TEST_F(SQTest_Comparison, sq_lteq__different_types_some_value_and_larger_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<-34456.78>();
    auto b = i16sq5_t::fromReal<-789.25>();
    auto c = i16sq5_t::fromReal<+689.25>();
    auto d = u16sq6_t::fromReal<+889.99>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, u16sq6_t > ));
    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a <= c);
    ASSERT_TRUE(a <= d);
    ASSERT_TRUE(b <= c);
    // b <= d does not work
    // c <= d does not work
    ASSERT_FALSE(( ThreewayComparable< i16sq5_t, u16sq6_t > ));
}

TEST_F(SQTest_Comparison, sq_lteq__same_type_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<+38976.84>();
    auto b = i32sq10_t::fromReal<+23456.43>();
    auto c = i32sq10_t::fromReal<-12345.67>();
    auto d = i32sq10_t::fromReal<-65432.19>();

    ASSERT_FALSE(a <= b);
    ASSERT_FALSE(a <= c);
    ASSERT_FALSE(a <= d);
    ASSERT_FALSE(b <= c);
    ASSERT_FALSE(b <= d);
    ASSERT_FALSE(c <= d);
}

TEST_F(SQTest_Comparison, sq_lteq__different_types_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+34456.78>();
    auto b = u16sq6_t::fromReal<+889.99>();
    auto c = i16sq5_t::fromReal<+678.25>();
    auto d = i16sq5_t::fromReal<-567.25>();

    ASSERT_FALSE(a <= b);
    ASSERT_FALSE(a <= c);
    ASSERT_FALSE(a <= d);
    ASSERT_FALSE(c <= d);
    // b <= c does not work
    // b <= d does not work
}

TEST_F(SQTest_Comparison, sq_lteq__same_type_same_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>();
    auto b = i32sq10_t::fromReal<+56897.129>();
    auto c = i32sq10_t::fromReal<-0.0>();
    auto d = i32sq10_t::fromReal<+0.0>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i32sq10_t > ));

    ASSERT_TRUE(a <= a);
    ASSERT_TRUE(b <= b);

    // +/- zero
    ASSERT_TRUE(c <= c);
    ASSERT_TRUE(d <= d);
    ASSERT_TRUE(c <= d);
    ASSERT_TRUE(d <= c);
}

TEST_F(SQTest_Comparison, sq_lteq__different_types_same_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+678.25>();
    auto b = u16sq6_t::fromReal<+678.25>();
    auto c = i16sq5_t::fromReal<+678.25>();
    auto d = i16sq5_t::fromReal<+678.25>();

    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< u16sq6_t, u16sq6_t > ));

    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a <= c);
    ASSERT_TRUE(a <= d);
    // b <= c does not work
    // b <= d does not work
    ASSERT_TRUE(c <= d);
}

TEST_F(SQTest_Comparison, sq_gt__same_type_some_value_and_smaller_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<+89999.99>();
    auto b = i32sq10_t::fromReal<+16789.25>();
    auto c = i32sq10_t::fromReal<-16789.25>();
    auto d = i32sq10_t::fromReal<-34456.78>();

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
    auto a = i32sq10_t::fromReal<+34456.78>();
    auto b = u16sq6_t::fromReal<+889.99>();
    auto c = i16sq5_t::fromReal<+789.25>();
    auto d = i16sq5_t::fromReal<-689.25>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, u16sq6_t > ));
    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a > c);
    ASSERT_TRUE(a > d);
    ASSERT_TRUE(c > d);
    // b > c does not work
    // b > d does not work
    ASSERT_FALSE(( ThreewayComparable< i16sq5_t, u16sq6_t > ));
}

TEST_F(SQTest_Comparison, sq_gt__same_type_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>();
    auto b = i32sq10_t::fromReal<-12345.67>();
    auto c = i32sq10_t::fromReal<+23456.43>();
    auto d = i32sq10_t::fromReal<+38976.84>();

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
    auto a = i32sq10_t::fromReal<-34456.78>();
    auto b = i16sq5_t::fromReal<-689.25>();
    auto c = i16sq5_t::fromReal<+789.25>();
    auto d = u16sq6_t::fromReal<+889.99>();

    ASSERT_FALSE(a > b);
    ASSERT_FALSE(a > c);
    ASSERT_FALSE(a > d);
    ASSERT_FALSE(b > c);
    // b > d does not work
    // c > d does not work
}

TEST_F(SQTest_Comparison, sq_gt__same_type_same_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>();
    auto b = i32sq10_t::fromReal<+56897.129>();
    auto c = i32sq10_t::fromReal<-0.0>();
    auto d = i32sq10_t::fromReal<+0.0>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i32sq10_t > ));

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
    auto a = i32sq10_t::fromReal<+678.25>();
    auto b = u16sq6_t::fromReal<+678.25>();
    auto c = i16sq5_t::fromReal<+678.25>();
    auto d = i16sq5_t::fromReal<+678.25>();

    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< u16sq6_t, u16sq6_t > ));

    ASSERT_FALSE(a > b);
    ASSERT_FALSE(a > c);
    ASSERT_FALSE(a > d);
    // b > c does not work
    // b > d does not work
    ASSERT_FALSE(c > d);
}

TEST_F(SQTest_Comparison, sq_gteq__same_type_some_value_and_smaller_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<+89999.99>();
    auto b = i32sq10_t::fromReal<+16789.25>();
    auto c = i32sq10_t::fromReal<-16789.25>();
    auto d = i32sq10_t::fromReal<-34456.78>();

    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a >= c);
    ASSERT_TRUE(a >= d);
    ASSERT_TRUE(b >= c);
    ASSERT_TRUE(b >= d);
    ASSERT_TRUE(c >= d);
}

TEST_F(SQTest_Comparison, sq_gteq__different_types_some_value_and_smaller_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+34456.78>();
    auto b = u16sq6_t::fromReal<+889.99>();
    auto c = i16sq5_t::fromReal<+789.25>();
    auto d = i16sq5_t::fromReal<-689.25>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, u16sq6_t > ));
    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));

    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a >= c);
    ASSERT_TRUE(a >= d);
    ASSERT_TRUE(c >= d);
    // b >= c does not work
    // b >= d does not work
    ASSERT_FALSE(( ThreewayComparable< i16sq5_t, u16sq6_t > ));
}

TEST_F(SQTest_Comparison, sq_gteq__same_type_some_value_and_smaller_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>();
    auto b = i32sq10_t::fromReal<-12345.67>();
    auto c = i32sq10_t::fromReal<+23456.43>();
    auto d = i32sq10_t::fromReal<+38976.84>();

    ASSERT_FALSE(a >= b);
    ASSERT_FALSE(a >= c);
    ASSERT_FALSE(a >= d);
    ASSERT_FALSE(b >= c);
    ASSERT_FALSE(b >= d);
    ASSERT_FALSE(c >= d);
}

TEST_F(SQTest_Comparison, sq_gteq__different_types_some_value_and_larger_value__returns_false) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<-34456.78>();
    auto b = i16sq5_t::fromReal<-689.25>();
    auto c = i16sq5_t::fromReal<+789.25>();
    auto d = u16sq6_t::fromReal<+889.99>();

    ASSERT_FALSE(a >= b);
    ASSERT_FALSE(a >= c);
    ASSERT_FALSE(a >= d);
    ASSERT_FALSE(b >= c);
    // b >= d does not work
    // c >= d does not work
}

TEST_F(SQTest_Comparison, sq_gteq__same_type_same_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    auto a = i32sq10_t::fromReal<-65432.19>();
    auto b = i32sq10_t::fromReal<+56897.129>();
    auto c = i32sq10_t::fromReal<-0.0>();
    auto d = i32sq10_t::fromReal<+0.0>();

    EXPECT_TRUE(( ThreewayComparable< i32sq10_t, i32sq10_t > ));

    ASSERT_TRUE(a >= a);
    ASSERT_TRUE(b >= b);

    // +/- zero
    ASSERT_TRUE(c >= c);
    ASSERT_TRUE(d >= d);
    ASSERT_TRUE(c >= d);
    ASSERT_TRUE(d >= c);
}

TEST_F(SQTest_Comparison, sq_gteq__different_types_same_value__returns_true) {
    using i32sq10_t = i32sq10<-100000., 100000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    using u16sq6_t = u16sq6<0., 1000.>;
    auto a = i32sq10_t::fromReal<+678.25>();
    auto b = u16sq6_t::fromReal<+678.25>();
    auto c = i16sq5_t::fromReal<+678.25>();
    auto d = i16sq5_t::fromReal<+678.25>();

    EXPECT_TRUE(( ThreewayComparable< i16sq5_t, i16sq5_t > ));
    EXPECT_TRUE(( ThreewayComparable< u16sq6_t, u16sq6_t > ));

    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a >= c);
    ASSERT_TRUE(a >= d);
    // b >= c does not work
    // b >= d does not work
    ASSERT_TRUE(c >= d);
}

TEST_F(SQTest_Comparison, sq_equal__various_types_same_value__returns_true) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+488.7>();
    auto b = u16sq5_t::fromReal<+488.7>();
    auto c = i16sq5_t::fromReal<+488.7>();

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
    ASSERT_FALSE(( EqComparable< i16sq5_t, u16sq5_t > ));
}

TEST_F(SQTest_Comparison, sq_equal__various_types_different_value__returns_false) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+1488.7>();
    auto b = u16sq5_t::fromReal<+488.7>();
    auto c = i16sq5_t::fromReal<-288.4>();

    ASSERT_FALSE(a == b);
    ASSERT_FALSE(a == c);
    // b == c does not work
}

TEST_F(SQTest_Comparison, sq_not_equal__various_types_same_value__returns_false) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+488.7>();
    auto b = u16sq5_t::fromReal<+488.7>();
    auto c = i16sq5_t::fromReal<+488.7>();

    ASSERT_FALSE(a != a);
    ASSERT_FALSE(a != b);
    ASSERT_FALSE(a != c);
    ASSERT_FALSE(b != b);
    ASSERT_FALSE(c != c);
    // b != c does not work
}

TEST_F(SQTest_Comparison, sq_not_equal__various_types_different_value__returns_true) {
    using i32sq5_t = i32sq5<-20000., 20000.>;
    using u16sq5_t = u16sq5<0., 1000.>;
    using i16sq5_t = i16sq5<-1000., 1000.>;
    auto a = i32sq5_t::fromReal<+1488.7>();
    auto b = u16sq5_t::fromReal<+488.7>();
    auto c = i16sq5_t::fromReal<-288.4>();

    ASSERT_TRUE(a != b);
    ASSERT_TRUE(a != c);
    // b != c does not work
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Shift ------------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Shift : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Shift, sq_shiftL__some_value__shifted_value) {
    using i32sq14_t = i32sq14<-1000., +1000.>;
    auto value = i32sq14_t::fromReal<-555.55>();

    EXPECT_TRUE(( LeftShiftable<i32sq14_t, decltype(7_ic)> ));
    auto shifted = value << 7_ic;

    using expected_t = i32sq14_t::clamp_t<-128000., +128000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(shifted)> ));
    ASSERT_NEAR(128*value.real(), shifted.real(), 128*i32sq14_t::resolution);
}

TEST_F(SQTest_Shift, sq_shiftL__some_value_shifted_by_0__same_value) {
    using i32sq14_t = i32sq14<-10000., +10000.>;
    auto value = i32sq14_t::fromReal<-5555.55>();

    EXPECT_TRUE(( LeftShiftable<i32sq14_t, decltype(0_ic)> ));
    auto shifted = value << 0_ic;

    ASSERT_TRUE(( std::is_same_v<i32sq14_t, decltype(shifted)> ));
    ASSERT_NEAR(value.real(), shifted.real(), i32sq14_t::resolution);
}

TEST_F(SQTest_Shift, sq_shiftL__invalid_shift__not_possible) {
    ASSERT_FALSE(( LeftShiftable< i32sq14<-128., 127.>, std::integral_constant<int, -2> > ));
    ASSERT_FALSE(( LeftShiftable< i32sq14<-128., 127.>, std::integral_constant<int, 4> > ));
    EXPECT_TRUE(( LeftShiftable< i32sq14<-128., 127.>, decltype(10_ic) > ));
    ASSERT_FALSE(( LeftShiftable< i32sq14<-128., 127.>, decltype(11_ic) > ));
}

TEST_F(SQTest_Shift, sq_shiftR__some_value__shifted_value) {
    using i32sq14_t = i32sq14<-10000., +10000.>;
    auto value = i32sq14_t::fromReal<-5555.55>();

    EXPECT_TRUE(( RightShiftable<i32sq14_t, decltype(2_ic)> ));
    auto shifted = value >> 2_ic;

    using expected_t = i32sq14_t::clamp_t<-2500., +2500.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(shifted)> ));
    ASSERT_NEAR(value.real()/4, shifted.real(), i32sq14_t::resolution);
}

TEST_F(SQTest_Shift, sq_shiftR__some_value_shifted_by_much__shifted_value_is_minus_one) {
    using i32sq14_t = i32sq14<-10000., +10000.>;
    auto value1 = i32sq14_t::fromReal<-5555.55>();
    auto value2 = i32sq14_t::fromReal<+5555.55>();

    EXPECT_TRUE(( RightShiftable<i32sq14_t, decltype(31_ic)> ));
    auto shifted1 = value1 >> 31_ic;  //< smallest: -1 when source value is negative
    auto shifted2 = value2 >> 31_ic;  //< smallest:  0 when source value is positive

    ASSERT_NEAR(-i32sq14_t::resolution, shifted1.real(), 1e-10);  // -1 * 2^-14
    ASSERT_NEAR(0.0, shifted2.real(), 1e-10);  // 0 * 2^-14
}

TEST_F(SQTest_Shift, sq_shiftR__some_value_shifted_by_0__same_value) {
    using i32sq14_t = i32sq14<-10000., +10000.>;
    auto value = i32sq14_t::fromReal<-5555.55>();

    EXPECT_TRUE(( RightShiftable<i32sq14_t, decltype(0_ic)> ));
    auto shifted = value >> 0_ic;

    ASSERT_TRUE(( std::is_same_v<i32sq14_t, decltype(shifted)> ));
    ASSERT_NEAR(value.real(), shifted.real(), i32sq14_t::resolution);
}

TEST_F(SQTest_Shift, sq_shiftR__invalid_shift__not_possible) {
    ASSERT_FALSE(( RightShiftable< i32sq14<-1024., 1023.>, std::integral_constant<int, -2> > ));
    ASSERT_FALSE(( RightShiftable< i32sq14<-1024., 1023.>, std::integral_constant<int, 4> > ));
    EXPECT_TRUE(( RightShiftable< i32sq14<-1024., 1023.>, decltype(8_ic) > ));
    EXPECT_TRUE(( RightShiftable< i32sq14<-1024., 1023.>, decltype(11_ic) > ));
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Square ----------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Square : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Square, sq_square__positive_value__squared_value) {
    using i32sq12_t = i32sq12<-100., +60.>;
    auto value = i32sq12_t::fromReal<23.4>();

    EXPECT_TRUE(( Squarable<i32sq12_t> ));
    auto squared = sqr(value);

    using expected_t = i32sq12_t::clamp_t<0., 10000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(squared)> ));
    ASSERT_NEAR(23.4*23.4, squared.real(), 45*i32sq12_t::resolution);
}

TEST_F(SQTest_Square, sq_square__positive_value_smaller_type_with_positive_range__squared_value_i32) {
    using i16sq10_t = i16sq10<6., +25.>;
    auto value = i16sq10_t::fromReal<23.4>();

    EXPECT_TRUE(( Squarable<i16sq10_t> ));
    auto squared = sqr(value);

    using expected_t = i32sq10<36., 625.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(squared)> ));
    ASSERT_NEAR(23.4*23.4, squared.real(), 45*i16sq10_t::resolution);
}

TEST_F(SQTest_Square, sq_square__negative_value__squared_value) {
    using i32sq12_t = i32sq12<-50., +100.>;
    auto value = i32sq12_t::fromReal<-45.999>();

    EXPECT_TRUE(( Squarable<i32sq12_t> ));
    auto squared = sqr(value);

    using expected_t = i32sq12_t::clamp_t<0., 10000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(squared)> ));
    ASSERT_NEAR(45.999*45.999, squared.real(), 90*i32sq12_t::resolution);
}

TEST_F(SQTest_Square, sq_square__negative_value_smaller_type_with_negative_range__squared_value_i32) {
    using i16sq10_t = i16sq10<-25., -6.>;
    auto value = i16sq10_t::fromReal<-18.9>();

    EXPECT_TRUE(( Squarable<i16sq10_t> ));
    auto squared = sqr(value);

    using expected_t = i32sq10<36., 625.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(squared)> ));
    ASSERT_NEAR(18.9*18.9, squared.real(), 38*i16sq10_t::resolution);
}

TEST_F(SQTest_Square, sq_square__value_zero__value_squared_is_zero) {
    using i32sq12_t = i32sq12<-50., +100.>;
    auto value1 = i32sq12_t::fromReal<-0.>();
    auto value2 = i32sq12_t::fromReal<+0.>();

    EXPECT_TRUE(( Squarable<i32sq12_t> ));
    auto squared1 = sqr(value1);
    auto squared2 = sqr(value2);

    using expected_t = i32sq12_t::clamp_t<0., 10000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(squared1)> ));
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(squared2)> ));
    ASSERT_NEAR(0., squared1.real(), i32sq12_t::resolution);
    ASSERT_NEAR(0., squared2.real(), i32sq12_t::resolution);
}

TEST_F(SQTest_Square, sq_square__various_types__squareable_or_not) {
    ASSERT_TRUE(( Squarable< i32sq12<-50., -0.> > ));
    ASSERT_TRUE(( Squarable< i32sq12<-0., +0.> > ));
    ASSERT_TRUE(( Squarable< i32sq12<+0., +66.> > ));
    ASSERT_FALSE(( Squarable< i32sq12<+0., +725.> > ));  // 725*725 exceeds 2^19
}

TEST_F(SQTest_Square, sq_sqrt__some_positive_value__root_taken) {
    using u32sq12_t = i32sq12<0., +1000.>;
    auto value = u32sq12_t::fromReal<900.>();

    EXPECT_TRUE(( SquareRootable<u32sq12_t> ));
    auto root = sqrt(value);

    using expected_t = u32sq12_t::clamp_t<0., 32.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(root)> ));
    ASSERT_NEAR(30., root.real(), u32sq12_t::resolution);
}

TEST_F(SQTest_Square, sq_sqrt__maximum_u32_value__root_taken) {
    using u32sq12_t = u32sq12<>;
    auto value = u32sq12_t::fromScaled< std::numeric_limits<uint32_t>::max() >();

    EXPECT_TRUE(( SquareRootable<u32sq12_t> ));
    auto root = sqrt(value);

    using expected_t = u32sq12_t::clamp_t<0., 1025.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(root)> ));
    ASSERT_NEAR(1024., root.real(), u32sq12_t::resolution);
}

TEST_F(SQTest_Square, sq_sqrt__zero_value__root_zero) {
    using i32sq12_t = i32sq12<0., +100.>;
    auto value = i32sq12_t::fromReal<+0.>();

    EXPECT_TRUE(( SquareRootable<i32sq12_t> ));
    auto root = sqrt(value);

    using expected_t = i32sq12_t::clamp_t<0., 11.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(root)> ));
    ASSERT_NEAR(0., root.real(), i32sq12_t::resolution);
}

TEST_F(SQTest_Square, sq_sqrt__various_types__not_rootable) {
    ASSERT_FALSE(( SquareRootable< i32sq12<-1000., -0.> > ));
}

TEST_F(SQTest_Square, sq_rsqrt__some_positive_value__reciprocal_root_taken) {
    using i32sq20_t = i32sq20<10., 1500.>;
    auto value = i32sq20_t::fromReal<25.0485>();

    EXPECT_TRUE(( SquareRootable<i32sq20_t> ));
    auto rRoot = rsqrt(value);

    using expected_t = i32sq20_t::clamp_t<0., 1.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(rRoot)> ));
    ASSERT_NEAR(0.199806282, rRoot.real(), i32sq20_t::resolution);
}

TEST_F(SQTest_Square, sq_rsqrt__maximum_positive_value__reciprocal_root_taken) {
    using u32sq30_t = u32sq30< 1.0, u32sq30<>::realMax >;
    auto value = u32sq30_t::fromScaled<std::numeric_limits<u32sq30_t::base_t>::max()>();

    EXPECT_TRUE(( RSquareRootable<u32sq30_t> ));
    auto rRoot = rsqrt(value);

    using expected_t = u32sq30_t::clamp_t<0., 1.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(rRoot)> ));
    ASSERT_NEAR(0.5, rRoot.real(), u32sq30_t::resolution);
}

TEST_F(SQTest_Square, sq_rsqrt__small_value_large_f__reciprocal_root_returned) {
    using i8sq6_t = i8sq6< i8sq6<>::resolution >;
    auto value = i8sq6_t::fromReal<0.95>();

    EXPECT_TRUE(( RSquareRootable<i8sq6_t> ));
    auto rRoot = rsqrt(value);

    using expected_t = i8sq6_t::clamp_t<0., i8sq6<>::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(rRoot)> ));
    ASSERT_NEAR(1.025978352, rRoot.real(), i8sq6_t::resolution * 1.4);
}

TEST_F(SQTest_Square, sq_rsqrt__minimum_value__maximum_value_returned) {
    using i32sq29_t = i32sq29< i32sq29<>::resolution, 1. >;
    auto value = i32sq29_t::fromScaled<1>();

    EXPECT_TRUE(( RSquareRootable<i32sq29_t> ));
    auto rRoot = rsqrt(value);

    using expected_t = i32sq29_t::clamp_t<0., i32sq29<>::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(rRoot)> ));
    ASSERT_NEAR(i32sq29<>::realMax, rRoot.real(), i32sq29_t::resolution);
}

TEST_F(SQTest_Square, sq_rsqrt__minimum_value2__reciprocal_root_returned) {
    using i32sq20_t = i32sq20< i32sq20<>::resolution, 1000. >;
    auto value = i32sq20_t::fromScaled<1>();

    EXPECT_TRUE(( RSquareRootable<i32sq20_t> ));
    auto rRoot = rsqrt(value);

    using expected_t = i32sq20_t::clamp_t<0., 1024.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(rRoot)> ));
    ASSERT_NEAR(1024., rRoot.real(), i32sq20_t::resolution);
}

TEST_F(SQTest_Square, sq_rsqrt__various_types__not_rootable) {
    ASSERT_FALSE(( RSquareRootable< i32sq20<-1000., -0.> > ));
    ASSERT_FALSE(( RSquareRootable< i32sq20<-1000., +1000.> > ));
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Cube ------------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Cube : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Cube, sq_cube__positive_value__value_cubed) {
    using u32sq12_t = u32sq12<0., 80.>;
    auto value = u32sq12_t::fromReal<55.999>();

    EXPECT_TRUE(( Cubeable<u32sq12_t> ));
    auto cubed = cube(value);

    using expected_t = u32sq12_t::clamp_t<0., 512000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(cubed)> ));
    ASSERT_NEAR(175606.5922, cubed.real(), 3*56*56*u32sq12_t::resolution);
}

TEST_F(SQTest_Cube, sq_cube__positive_value_smaller_type__value_cubed_i32) {
    using u16sq4_t = u16sq4<0., 500.>;
    auto value = u16sq4_t::fromReal<144.999>();

    EXPECT_TRUE(( Cubeable<u16sq4_t> ));
    auto cubed = cube(value);

    using expected_t = i32sq4<0., 1.25e8>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(cubed)> ));
    ASSERT_NEAR(3048561.925, cubed.real(), 3*145*145*u16sq4_t::resolution);
}

TEST_F(SQTest_Cube, sq_cube__zero_value__value_cubed_is_zero) {
    using i32sq12_t = i32sq12<-40., 40.>;
    auto value1 = i32sq12_t::fromReal<-0.>();
    auto value2 = i32sq12_t::fromReal<+0.>();

    EXPECT_TRUE(( Cubeable<i32sq12_t> ));
    auto cubed1 = cube(value1);
    auto cubed2 = cube(value2);

    using expected_t = i32sq12_t::clamp_t<-64000., 64000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(cubed1)> ));
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(cubed2)> ));
    ASSERT_NEAR(0., cubed1.real(), i32sq12_t::resolution);
    ASSERT_NEAR(0., cubed2.real(), i32sq12_t::resolution);
}

TEST_F(SQTest_Cube, sq_cube__negative_value__value_cubed) {
    using i32sq12_t = i32sq12<-40., 40.>;
    auto value = i32sq12_t::fromReal<-35.999>();

    EXPECT_TRUE(( Cubeable<i32sq12_t> ));
    auto cubed = cube(value);

    using expected_t = i32sq12_t::clamp_t<-64000., 64000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(cubed)> ));
    ASSERT_NEAR(-46652.11211, cubed.real(), 3*36*36*i32sq12_t::resolution);
}

TEST_F(SQTest_Cube, sq_cube__smallest_value__value_cubed_is_minimum_i32) {
    using i32sq7_t = i32sq7<-256., -0.>;
    auto value = i32sq7_t::fromReal<-256.>();

    EXPECT_TRUE(( Cubeable<i32sq7_t> ));
    auto cubed = cube(value);

    using expected_t = i32sq7_t::clamp_t<-16777216., -0.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(cubed)> ));
    ASSERT_NEAR(-16777216., cubed.real(), i32sq7_t::resolution);
}

TEST_F(SQTest_Square, sq_cube__various_types__cubeable_or_not) {
    ASSERT_TRUE(( Cubeable< i32sq12<-50., -0.> > ));
    ASSERT_TRUE(( Cubeable< i32sq12<-0., +0.> > ));
    ASSERT_TRUE(( Cubeable< i32sq12<+0., +66.> > ));
    ASSERT_FALSE(( Cubeable< i32sq12<+0., +81.> > ));  // 81*81*81 exceeds 2^19
    ASSERT_FALSE(( Cubeable< i32sq12<-81., -0.> > ));
}

TEST_F(SQTest_Cube, sq_cbrt__positive_value__cube_root_taken) {
    using u32sq8_t = u32sq8<0., 45000.>;
    auto value = u32sq8_t::fromReal<41599.999>();

    EXPECT_TRUE(( CubeRootable<u32sq8_t> ));
    auto root = cbrt(value);

    using expected_t = u32sq8_t::clamp_t<0., 36.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(root)> ));
    ASSERT_NEAR(34.64956394, root.real(), u32sq8_t::resolution);
}

TEST_F(SQTest_Cube, sq_cbrt__maximum_u32_value__cube_root_taken) {
    using u32sq16_t = u32sq16<>;
    auto value = u32sq16_t::fromScaled< std::numeric_limits<uint32_t>::max() >();

    EXPECT_TRUE(( CubeRootable<u32sq16_t> ));
    auto root = cbrt(value);

    using expected_t = u32sq16_t::clamp_t<0., 41.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(root)> ));
    ASSERT_NEAR(40.31747359, root.real(), u32sq16_t::resolution);
}

TEST_F(SQTest_Cube, sq_cbrt__zero_value__cube_root_is_zero) {
    using i32sq12_t = i32sq12<0., 400.>;
    auto value = i32sq12_t::fromReal<+0.>();

    EXPECT_TRUE(( CubeRootable<i32sq12_t> ));
    auto root = cbrt(value);

    using expected_t = i32sq12_t::clamp_t<0., 8.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(root)> ));
    ASSERT_NEAR(0., root.real(), i32sq12_t::resolution);
}

TEST_F(SQTest_Cube, sq_cbrt__various_types__not_rootable) {
    ASSERT_FALSE(( CubeRootable< i32sq12<-1000., -0.> > ));
    ASSERT_FALSE(( CubeRootable< i32sq12<-1000., +1000.> > ));
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Clamp ------------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Clamp : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Clamp, sq_clamp__some_value_in_same_range__same_value_same_type) {
    using u32sq10_t = u32sq10<50.0, 5000.0>;
    auto a = u32sq10_t::fromReal<555.555>();
    auto min = u32sq10_t::fromReal<u32sq10_t::realMin>();
    auto max = u32sq10_t::fromReal<u32sq10_t::realMax>();

    auto clamped = clamp(a, min, max);  // argument-dependent lookup (ADL)
    auto clamped2 = clamp<u32sq10_t::realMin, u32sq10_t::realMax>(a);

    ASSERT_TRUE(( std::is_same_v<u32sq10_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<u32sq10_t, decltype(clamped2)> ));
    ASSERT_NEAR(a.real(), clamped.real(), u32sq10_t::resolution);
    ASSERT_NEAR(a.real(), clamped2.real(), u32sq10_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_value_in_narrower_range__same_value_new_type) {
    using value_t = u32sq10<50.0, 5000.0>;
    using limit_t = value_t::clamp_t<60., 600.>;
    auto a = value_t::fromReal<555.555>();
    auto min = limit_t::fromReal<61.>();
    auto max = limit_t::fromReal<599.>();

    auto clamped = clamp(a, min, max);

    constexpr double sLo = 70., sHi = 580.;
    auto clamped2 = clamp<sLo, sHi>(a);

    ASSERT_TRUE(( std::is_same_v<limit_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<sLo, sHi>, decltype(clamped2)> ));
    ASSERT_NEAR(a.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(a.real(), clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_value_larger_than_narrower_range__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using limit_t = value_t::clamp_t<60., 500.>;
    auto a = value_t::fromReal<555.555>();
    auto min = limit_t::fromReal<61.>();
    auto max = limit_t::fromReal<499.>();

    auto clamped = clamp(a, min, max);

    constexpr double sLo = 70., sHi = 480.;
    auto clamped2 = clamp<sLo, sHi>(a);

    ASSERT_TRUE(( std::is_same_v<limit_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<sLo, sHi>, decltype(clamped2)> ));
    ASSERT_NEAR(max.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(sHi, clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_value_smaller_than_narrower_range__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using limit_t = value_t::clamp_t<60., 500.>;
    auto a = value_t::fromReal<-22.22>();
    auto min = limit_t::fromReal<61.>();
    auto max = limit_t::fromReal<499.>();

    auto clamped = clamp(a, min, max);

    constexpr double sLo = 70., sHi = 480.;
    auto clamped2 = clamp<sLo, sHi>(a);

    ASSERT_TRUE(( std::is_same_v<limit_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<sLo, sHi>, decltype(clamped2)> ));
    ASSERT_NEAR(min.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(sLo, clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_value_in_narrower_range_with_different_f__same_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using min_t = i32sq15<60., 300.>;
    using max_t = i32sq20<80., 600.>;
    auto a = value_t::fromReal<555.555>();
    auto min = min_t::fromReal<61.>();
    auto max = max_t::fromReal<599.>();

    auto clamped = clamp(a, min, max);

    using expected_t = value_t::clamp_t<min_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(a.real(), clamped.real(), expected_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_value_larger_than_narrower_range_with_different_f__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using min_t = i32sq15<60., 400.>;
    using max_t = i32sq20<80., 500.>;
    auto a = value_t::fromReal<555.555>();
    auto min = min_t::fromReal<61.>();
    auto max = max_t::fromReal<499.>();

    auto clamped = clamp(a, min, max);

    using expected_t = value_t::clamp_t<min_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(max.real(), clamped.real(), expected_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_value_smaller_than_narrower_range_with_different_f__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using min_t = i32sq15<60., 300.>;
    using max_t = i32sq20<80., 500.>;
    auto a = value_t::fromReal<-22.22>();
    auto min = min_t::fromReal<61.>();
    auto max = max_t::fromReal<499.>();

    auto clamped = clamp(a, min, max);

    using expected_t = value_t::clamp_t<min_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(min.real(), clamped.real(), expected_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clamp__some_cases_not_clampable__does_not_compile) {
    // different base types
    ASSERT_FALSE(( Clampable< i16sq10<>, u16sq10<>, u16sq10<> > ));
    ASSERT_FALSE(( Clampable< u16sq10<>, i16sq10<>, u16sq10<> > ));
    ASSERT_FALSE(( Clampable< u16sq10<>, u16sq10<>, i16sq10<> > ));

    // lo type is not implicitly convertible to value type (lo has lower minimum)
    ASSERT_FALSE(( Clampable< i16sq8<-10., 10.>, i16sq8<-12., 10.>, i16sq8<-5., 10.> > ));

    // hi type is not implicitly convertible to value type (hi has higher maximum)
    ASSERT_FALSE(( Clampable< i16sq8<-10., 10.>, i16sq8<-5., 10.>, i16sq8<0., 15.> > ));

    // minimum of lo is larger than maximum of hi
    ASSERT_FALSE(( Clampable< i16sq8<-10., 10.>, i16sq8<-5., 20.>, i16sq8<-10., -6.> > ));
}

TEST_F(SQTest_Clamp, sq_clampLower__some_value_in_same_range__same_value_same_type) {
    using u32sq10_t = u32sq10<50.0, 5000.0>;
    auto a = u32sq10_t::fromReal<443.210>();
    auto min = u32sq10_t::fromReal<u32sq10_t::realMin>();

    auto clamped = clampLower(a, min);
    auto clamped2 = clampLower<u32sq10_t::realMin>(a);

    ASSERT_TRUE(( std::is_same_v<u32sq10_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<u32sq10_t, decltype(clamped2)> ));
    ASSERT_NEAR(a.real(), clamped.real(), u32sq10_t::resolution);
    ASSERT_NEAR(a.real(), clamped2.real(), u32sq10_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampLower__some_value_in_narrower_range__same_value_new_type) {
    using value_t = u32sq10<50.0, 5000.0>;
    using min_t = value_t::clamp_t<60., 600.>;
    auto a = value_t::fromReal<70.12>();
    auto min = min_t::fromReal<66.67>();

    auto clamped = clampLower(a, min);

    constexpr double sLo = 55.5;
    auto clamped2 = clampLower<sLo>(a);

    using expected_t = value_t::clamp_t<min_t::realMin, value_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<sLo, value_t::realMax>, decltype(clamped2)> ));
    ASSERT_NEAR(a.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(a.real(), clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampLower__some_value_smaller_than_min__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using min_t = value_t::clamp_t<-10., 500.>;
    auto a = value_t::fromReal<-12.22>();
    auto min = min_t::fromReal<-8.498>();

    auto clamped = clampLower(a, min);

    constexpr double sLo = -12.12;
    auto clamped2 = clampLower<sLo>(a);

    using expected_t = value_t::clamp_t<min_t::realMin, value_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<sLo, value_t::realMax>, decltype(clamped2)> ));
    ASSERT_NEAR(min.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(sLo, clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampLower__some_value_in_narrower_range_with_different_f__same_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using min_t = i32sq15<60., 300.>;
    auto a = value_t::fromReal<68.555>();
    auto min = min_t::fromReal<61.>();

    auto clamped = clampLower(a, min);

    using expected_t = value_t::clamp_t<min_t::realMin, value_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(a.real(), clamped.real(), expected_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampLower__some_value_smaller_than_min_with_different_f__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using min_t = i32sq15<-20., 300.>;
    auto a = value_t::fromReal<-22.22>();
    auto min = min_t::fromReal<-18.98>();

    auto clamped = clampLower(a, min);

    using expected_t = value_t::clamp_t<min_t::realMin, value_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(min.real(), clamped.real(), expected_t::resolution);
}


TEST_F(SQTest_Clamp, sq_clampUpper__some_value_in_same_range__same_value_same_type) {
    using u32sq10_t = u32sq10<50.0, 5000.0>;
    auto a = u32sq10_t::fromReal<543.21>();
    auto max = u32sq10_t::fromReal<u32sq10_t::realMax>();

    auto clamped = clampUpper(a, max);
    auto clamped2 = clampUpper<u32sq10_t::realMax>(a);

    ASSERT_TRUE(( std::is_same_v<u32sq10_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<u32sq10_t, decltype(clamped2)> ));
    ASSERT_NEAR(a.real(), clamped.real(), u32sq10_t::resolution);
    ASSERT_NEAR(a.real(), clamped2.real(), u32sq10_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampUpper__some_value_in_narrower_range__same_value_new_type) {
    using value_t = u32sq10<50.0, 5000.0>;
    using max_t = value_t::clamp_t<60., 600.>;
    auto a = value_t::fromReal<521.09>();
    auto max = max_t::fromReal<599.>();

    auto clamped = clampUpper(a, max);

    constexpr double sHi = 577.78;
    auto clamped2 = clampUpper<sHi>(a);

    using expected_t = value_t::clamp_t<value_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<value_t::realMin, sHi>, decltype(clamped2)> ));
    ASSERT_NEAR(a.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(a.real(), clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampUpper__some_value_larger_than_max__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using max_t = value_t::clamp_t<60., 500.>;
    auto a = value_t::fromReal<504.12>();
    auto max = max_t::fromReal<487.65>();

    auto clamped = clampUpper(a, max);

    constexpr double sHi = 466.67;
    auto clamped2 = clampUpper<sHi>(a);

    using expected_t = value_t::clamp_t<value_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_TRUE(( std::is_same_v<value_t::clamp_t<value_t::realMin, sHi>, decltype(clamped2)> ));
    ASSERT_NEAR(max.real(), clamped.real(), value_t::resolution);
    ASSERT_NEAR(sHi, clamped2.real(), value_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampUpper__some_value_in_narrower_range_with_different_f__same_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using max_t = i32sq20<80., 600.>;
    auto a = value_t::fromReal<598.10>();
    auto max = max_t::fromReal<598.99>();

    auto clamped = clampUpper(a, max);

    using expected_t = value_t::clamp_t<value_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(a.real(), clamped.real(), expected_t::resolution);
}

TEST_F(SQTest_Clamp, sq_clampUpper__some_value_larger_than_max_with_different_f__clamped_value_new_type) {
    using value_t = i32sq10<-50.0, 5000.0>;
    using max_t = i32sq20<80., 500.>;
    auto a = value_t::fromReal<505.55>();
    auto max = max_t::fromReal<494.22>();

    auto clamped = clampUpper(a, max);

    using expected_t = value_t::clamp_t<value_t::realMin, max_t::realMax>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(clamped)> ));
    ASSERT_NEAR(max.real(), clamped.real(), expected_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Min/Max ---------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_MinMax : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_MinMax, sq_min__two_positive_values__returns_smaller) {
    using value1_t = i32sq14<100., 12000.>;
    using value2_t = i32sq14<0., 1000.>;
    auto a = value1_t::fromReal<568.47>();
    auto b = value2_t::fromReal<570.55>();

    auto minimum1 = min(a, b);

    using expected_t = value1_t::clamp_t<0., 1000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(minimum1)> ));
    ASSERT_NEAR(std::min(a.real(), b.real()), minimum1.real(), expected_t::resolution);
}

TEST_F(SQTest_MinMax, sq_min__two_negative_values__returns_smaller) {
    using value1_t = i32sq14<-12000., 12000.>;
    using value2_t = i32sq14<-2000., -0.>;
    auto a = value1_t::fromReal<-568.47>();
    auto b = value2_t::fromReal<-570.55>();

    auto minimum1 = min(a, b);

    using expected_t = value1_t::clamp_t<-12000., -0.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(minimum1)> ));
    ASSERT_NEAR(std::min(a.real(), b.real()), minimum1.real(), expected_t::resolution);
}

TEST_F(SQTest_MinMax, sq_min__two_mixed_values__returns_smaller) {
    using value1_t = i32sq14<-12000., 12000.>;
    using value2_t = i32sq14<-2000., 5000.>;
    auto a = value1_t::fromReal<-1689.47>();
    auto b = value2_t::fromReal<+1572.78>();

    auto minimum1 = min(a, b);

    using expected_t = value1_t::clamp_t<-12000., 5000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(minimum1)> ));
    ASSERT_NEAR(std::min(a.real(), b.real()), minimum1.real(), expected_t::resolution);
}

TEST_F(SQTest_MinMax, sq_min__neg_zero_and_pos_zero__returns_positive_zero) {
    using value_t = i32sq14<-0., +0.>;
    auto a = value_t::fromReal<-0.>();
    auto b = value_t::fromReal<+0.>();

    auto minimum1 = min(a, b);  // will give +0., no matter if it is the first or second value (because int-zero is always positive)

    ASSERT_TRUE(( std::is_same_v<value_t, decltype(minimum1)> ));
    ASSERT_NEAR(+0., minimum1.real(), value_t::resolution);
}

TEST_F(SQTest_MinMax, sq_max__two_positive_values__returns_smaller) {
    using value1_t = i32sq14<100., 12000.>;
    using value2_t = i32sq14<0., 1000.>;
    auto a = value1_t::fromReal<568.47>();
    auto b = value2_t::fromReal<570.55>();

    auto maximum1 = max(a, b);

    using expected_t = value1_t::clamp_t<100., 12000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(maximum1)> ));
    ASSERT_NEAR(std::max(a.real(), b.real()), maximum1.real(), expected_t::resolution);
}

TEST_F(SQTest_MinMax, sq_max__two_negative_values__returns_smaller) {
    using value1_t = i32sq14<-12000., 12000.>;
    using value2_t = i32sq14<-2000., -0.>;
    auto a = value1_t::fromReal<-568.47>();
    auto b = value2_t::fromReal<-570.55>();

    auto maximum1 = max(a, b);

    using expected_t = value1_t::clamp_t<-2000., 12000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(maximum1)> ));
    ASSERT_NEAR(std::max(a.real(), b.real()), maximum1.real(), expected_t::resolution);
}

TEST_F(SQTest_MinMax, sq_max__two_mixed_values__returns_smaller) {
    using value1_t = i32sq14<-2000., 12000.>;
    using value2_t = i32sq14<0., 2000.>;
    auto a = value1_t::fromReal<-1689.47>();
    auto b = value2_t::fromReal<1572.78>();

    auto maximum1 = max(a, b);

    using expected_t = value1_t::clamp_t<0., 12000.>;
    ASSERT_TRUE(( std::is_same_v<expected_t, decltype(maximum1)> ));
    ASSERT_NEAR(std::max(a.real(), b.real()), maximum1.real(), expected_t::resolution);
}

TEST_F(SQTest_MinMax, sq_max__neg_zero_and_pos_zero__returns_positive_zero) {
    using value_t = i32sq14<-0., +0.>;
    auto a = value_t::fromReal<-0.>();
    auto b = value_t::fromReal<+0.>();

    auto maximum1 = max(a, b);  // will give +0., no matter if it is the first or second value (because int-zero is always positive)

    ASSERT_TRUE(( std::is_same_v<value_t, decltype(maximum1)> ));
    ASSERT_NEAR(+0., maximum1.real(), value_t::resolution);
}

// EOF
