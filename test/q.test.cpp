/* \file
 * Tests for q.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
#include <cassert>

#include <fpm.hpp>
using namespace fpm::types;


/// Concept which checks whether a value with the given Q type can be constructed from the given real value.
template< class Q, double real, fpm::Overflow ovfBxOvrd = Q::ovfBx >
concept CanConstructQFromReal = requires {
    { Q::template fromReal<real, ovfBxOvrd>() } -> std::same_as<Q>;
};

/// Concept which checks whether a value with the given Q type can be constructed from the given scaled value.
template< class Q, Q::base_t scaled, fpm::Overflow ovfBxOvrd = Q::ovfBx >
concept CanConstructQFromScaled = requires {
    { Q::template fromScaled<scaled, ovfBxOvrd>() } -> std::same_as<Q>;
};

/// Concept which checks whether an instance of a given Q type can be converted to an instance of a
// given Sq type.
template< class Q, class Sq, fpm::Overflow ovfBxOvrd = Q::ovfBx >
concept CanConvertQToSq = requires (Q q) {
    { q.template toSq<Sq::realMin, Sq::realMax, ovfBxOvrd>() } -> std::same_as<Sq>;
};

/// Concept which checks whether an instance of a given Sq type can be converted to an instance of a
/// given Q type via explicit conversion.
template< class Sq, class Q >
concept CanConvertSqToQExplicitly = requires (Sq sq) {
    { Q::template fromSq(sq) } -> std::same_as<Q>;
};

/// Concept which checks whether an instance of a given Sq type can be converted to an instance of a
/// given Q type via implicit conversion.
template< class Sq, class Q >
concept CanConvertSqToQImplicitly = requires (Q q, Sq sq) {
    q = sq;
};

/// Concept which checks whether an instance of a given Q type can be static_cast to an instance of
/// another given Q type.
/// \note If this returns false, try static_q_cast with an overflow override option.
template< class QSrc, class QTarget >
concept StaticCastable = requires (QSrc qSrc) {
    { static_cast<QTarget>(qSrc) } -> std::same_as<QTarget>;
    { static_q_cast<QTarget>(qSrc) } -> std::same_as<QTarget>;
};

/// Concept which checks whether an instance of a given Q type can be static_cast to an instance of
/// another given Q type.
/// \note If this returns false, try static_q_cast with an overflow override option.
template< class QSrc, class QTarget, fpm::Ovf ovfBx >
concept SafeCastable = requires (QSrc qSrc) {
    { safe_q_cast<QTarget, ovfBx>(qSrc) } -> std::same_as<QTarget>;
};


// Implement assert trap function for manual tests.
void fpm::ovfAssertTrap() {
    assert(false);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Construction ------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Construct : public ::testing::Test {
protected:
    using i16q6_t = i16q6<-500., 500.>;  // compiler error on overflow
    using i16q6_clamp_t = i16q6<-500., 500., fpm::Ovf::clamp>;
    using i16q6_ovf_t = i16q6<-500., 500., fpm::Ovf::allowed>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Construct, q_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double real = -495.1;
    auto qValue = i16q6_t::fromReal<real>();  // this does not perform overflow checks
    auto qValue2 = i16q6_clamp_t::fromReal<real, fpm::Ovf::error>();  // overflow check is forbidden

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.scaled())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(qValue.real())>));

    constexpr int16_t resultMemValue = -31686;
    ASSERT_EQ(resultMemValue, qValue.scaled());
    ASSERT_EQ(-495, qValue.real<int>());  // test this once to make sure it works as expected
    ASSERT_NEAR(real, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValue2.scaled());
    ASSERT_NEAR(real, qValue2.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_negative_int16_positiveF_clamp__expected_value) {
    constexpr double real = -511.;
    auto qValue = i16q6_t::fromReal<real, fpm::Ovf::clamp>();
    auto qValueClamp2 = i16q6_clamp_t::fromReal<real>();

    ASSERT_EQ(i16q6_t::scaledMin, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMin, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMin, qValueClamp2.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMin, qValueClamp2.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_positive_int16_positiveF_clamp__expected_value) {
    constexpr double real = +511.;
    auto qValue = i16q6_t::fromReal<real, fpm::Ovf::clamp>();
    auto qValueClamp2 = i16q6_clamp_t::fromReal<real>();

    ASSERT_EQ(i16q6_t::scaledMax, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMax, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMax, qValueClamp2.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMax, qValueClamp2.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double real = -511.;
    auto qValue = i16q6_t::fromReal<real, fpm::Ovf::allowed>();
    auto qValueOvf1 = i16q6_t::fromReal<real, fpm::Ovf::unchecked>();
    auto qValueOvf2 = i16q6_ovf_t::fromReal<real>();

    constexpr int16_t resultMemValue = -32704;
    ASSERT_EQ(resultMemValue, qValue.scaled());
    ASSERT_NEAR(real, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf1.scaled());
    ASSERT_NEAR(real, qValueOvf1.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf2.scaled());
    ASSERT_NEAR(real, qValueOvf2.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double real = +511.;
    auto qValue = i16q6_t::fromReal<real, fpm::Ovf::allowed>();
    auto qValueOvf1 = i16q6_t::fromReal<real, fpm::Ovf::unchecked>();
    auto qValueOvf2 = i16q6_ovf_t::fromReal<real>();

    constexpr int16_t resultMemValue = +32704;
    ASSERT_EQ(resultMemValue, qValue.scaled());
    ASSERT_NEAR(real, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf1.scaled());
    ASSERT_NEAR(real, qValueOvf1.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf2.scaled());
    ASSERT_NEAR(real, qValueOvf2.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_from_real__negative_value_out_of_range__construction_not_possible) {
    using i16q6_t2 = i16q6<-500., 500.>;
    EXPECT_TRUE(( CanConstructQFromReal< i16q6_t2, -500. > ));
    ASSERT_FALSE(( CanConstructQFromReal< i16q6_t2, -501. > ));
}

TEST_F(QTest_Construct, q_from_real__positive_value_out_of_range__construction_not_possible) {
    using i16q6_t2 = i16q6<-500., 500.>;
    EXPECT_TRUE(( CanConstructQFromReal< i16q6_t2, +500. > ));
    ASSERT_FALSE(( CanConstructQFromReal< i16q6_t2, +501. > ));
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t memValue = -31686;
    auto qValue = i16q6_t::fromScaled<memValue>();  // this does not perform overflow checks
    auto qValue2 = i16q6_clamp_t::fromScaled<memValue, fpm::Ovf::error>();  // overflow check is forbidden

    constexpr double resultreal = -495.1;
    ASSERT_EQ(memValue, qValue.scaled());
    ASSERT_NEAR(resultreal, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValue2.scaled());
    ASSERT_NEAR(resultreal, qValue2.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_negative_int16_positiveF_clamp__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6_t::fromScaled<memValue, fpm::Ovf::clamp>();
    auto qValueClamp2 = i16q6_clamp_t::fromScaled<memValue>();

    ASSERT_EQ(i16q6_t::scaledMin, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMin, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMin, qValueClamp2.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMin, qValueClamp2.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_positive_int16_positiveF_clamp__expected_value) {
    constexpr int16_t memValue = +32704;
    auto qValue = i16q6_t::fromScaled<memValue, fpm::Ovf::clamp>();
    auto qValueClamp2 = i16q6_clamp_t::fromScaled<memValue>();

    ASSERT_EQ(i16q6_t::scaledMax, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMax, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMax, qValueClamp2.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMax, qValueClamp2.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6_t::fromScaled<memValue, fpm::Ovf::allowed>();
    auto qValueOvf1 = i16q6_t::fromScaled<memValue, fpm::Ovf::unchecked>();
    auto qValueOvf2 = i16q6_ovf_t::fromScaled<memValue>();

    constexpr double resultreal = -511.;
    ASSERT_EQ(memValue, qValue.scaled());
    ASSERT_NEAR(resultreal, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValueOvf1.scaled());
    ASSERT_NEAR(resultreal, qValueOvf1.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValueOvf2.scaled());
    ASSERT_NEAR(resultreal, qValueOvf2.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = +32704;
    auto qValue = i16q6_t::fromScaled<memValue, fpm::Ovf::allowed>();
    auto qValueOvf1 = i16q6_t::fromScaled<memValue, fpm::Ovf::unchecked>();
    auto qValueOvf2 = i16q6_ovf_t::fromScaled<memValue>();

    constexpr double resultreal = +511.;
    ASSERT_EQ(memValue, qValue.scaled());
    ASSERT_NEAR(resultreal, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValueOvf1.scaled());
    ASSERT_NEAR(resultreal, qValueOvf1.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValueOvf2.scaled());
    ASSERT_NEAR(resultreal, qValueOvf2.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__negative_value_out_of_range__construction_not_possible) {
    using i16q6_t2 = i16q6<-500., 500.>;  // scaled: -32000 .. +32000
    EXPECT_TRUE(( CanConstructQFromScaled< i16q6_t2, -32000 > ));
    ASSERT_FALSE(( CanConstructQFromScaled< i16q6_t2, -32001 > ));
}

TEST_F(QTest_Construct, q_from_scaled__positive_value_out_of_range__construction_not_possible) {
    using i16q6_t2 = i16q6<-500., 500.>;  // scaled: -32000 .. +32000
    EXPECT_TRUE(( CanConstructQFromScaled< i16q6_t2, +32000 > ));
    ASSERT_FALSE(( CanConstructQFromScaled< i16q6_t2, +32001 > ));
}

TEST_F(QTest_Construct, q_construct__constexpr_negative_int16_positiveF_clamp__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6_t::construct<fpm::Ovf::clamp>(memValue);
    auto qValueClamp = i16q6_clamp_t::construct(memValue);

    ASSERT_EQ(i16q6_t::scaledMin, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMin, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMin, qValueClamp.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMin, qValueClamp.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_positive_int16_positiveF_clamp__expected_value) {
    constexpr int16_t memValue = 32704;
    auto qValue = i16q6_t::construct<fpm::Ovf::clamp>(memValue);
    auto qValueClamp = i16q6_clamp_t::construct(memValue);

    ASSERT_EQ(i16q6_t::scaledMax, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMax, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMax, qValueClamp.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMax, qValueClamp.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6_t::construct<fpm::Ovf::allowed>(memValue);
    auto qValueOvf = i16q6_ovf_t::construct(memValue);

    constexpr double resultreal = -511.;
    ASSERT_EQ(memValue, qValue.scaled());
    ASSERT_NEAR(resultreal, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValueOvf.scaled());
    ASSERT_NEAR(resultreal, qValueOvf.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = +32704;
    auto qValue = i16q6_t::construct<fpm::Ovf::allowed>(memValue);
    auto qValueOvf = i16q6_ovf_t::construct(memValue);

    constexpr double resultreal = +511.;
    ASSERT_EQ(memValue, qValue.scaled());
    ASSERT_NEAR(resultreal, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(memValue, qValueOvf.scaled());
    ASSERT_NEAR(resultreal, qValueOvf.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_negative_int16_positiveF_clamp__expected_value) {
    constexpr double real = -511.;
    int16_t volatile value = fpm::v2s<6,int16_t>(real);
    auto qValue = i16q6_t::construct<fpm::Ovf::clamp>(value);
    auto qValueClamp = i16q6_clamp_t::construct(value);

    ASSERT_EQ(i16q6_t::scaledMin, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMin, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMin, qValueClamp.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMin, qValueClamp.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_positive_int16_positiveF_clamp__expected_value) {
    constexpr double real = +511.;
    int16_t volatile value = fpm::v2s<6,int16_t>(real);
    auto qValue = i16q6_t::construct<fpm::Ovf::clamp>(value);
    auto qValueClamp = i16q6_clamp_t::construct(value);

    ASSERT_EQ(i16q6_t::scaledMax, qValue.scaled());
    ASSERT_NEAR(i16q6_t::realMax, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(i16q6_clamp_t::scaledMax, qValueClamp.scaled());
    ASSERT_NEAR(i16q6_clamp_t::realMax, qValueClamp.real(), i16q6_clamp_t::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double real = -511.;
    int16_t volatile value = fpm::v2s<6,int16_t>(real);
    auto qValue = i16q6_t::construct<fpm::Ovf::allowed>(value);
    auto qValueOvf = i16q6_ovf_t::construct(value);

    constexpr int16_t resultMemValue = -32704;
    ASSERT_EQ(resultMemValue, qValue.scaled());
    ASSERT_NEAR(real, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf.scaled());
    ASSERT_NEAR(real, qValueOvf.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double real = +511.;
    int16_t volatile value = fpm::v2s<6,int16_t>(real);
    auto qValue = i16q6_t::construct<fpm::Ovf::allowed>(value);
    auto qValueOvf = i16q6_ovf_t::construct(value);

    constexpr int16_t resultMemValue = +32704;
    ASSERT_EQ(resultMemValue, qValue.scaled());
    ASSERT_NEAR(real, qValue.real(), i16q6_t::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf.scaled());
    ASSERT_NEAR(real, qValueOvf.real(), i16q6_ovf_t::resolution);
}

TEST_F(QTest_Construct, q_construct_via_literal__int_value_positive__expected_value) {
    i16q6_t qValue = 481_i16q6;

    constexpr int16_t resultMemValue = +30784;
    ASSERT_EQ(resultMemValue, qValue.scaled());
    ASSERT_NEAR(481., qValue.real(), i16q6_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Related Sq -------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Sq : public ::testing::Test {
protected:
    using i32q20_t = i32q20< -500., 500.>;  // overflow forbidden by default
    using i32q20_clamp_t = i32q20< -500., 500., fpm::Ovf::clamp>;
    using i32q20_ovf_t = i32q20< -500., 500., fpm::Ovf::allowed>;
    using i32q20_l_t = i32q20<-1000., 1000.>;  // overflow forbidden by default

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Sq, q_embedded_sq__from_real__sq_value_is_correct) {
    constexpr double realSqValue = +356.;
    auto sqValue = i32q20_t::Sq<>::fromReal<realSqValue>();

    ASSERT_NEAR(realSqValue, sqValue.real(), i32q20_t::Sq<>::resolution);
}

TEST_F(QTest_Sq, q_embedded_sq__smaller_value_range_from_real__sq_value_is_correct) {
    constexpr double realSqValue = -400.;
    auto sqValue = i32q20_t::Sq<-400., 400.>::fromReal<realSqValue>();

    ASSERT_NEAR(realSqValue, sqValue.real(), i32q20_t::resolution);
}

TEST_F(QTest_Sq, q_to_sq__same_value_range__no_overflow_check_performed) {
    constexpr double real = -400.;
    auto sqValue = i32q20_t::fromReal<real>().toSq();  // does not include a range check

    ASSERT_NEAR(real, sqValue.real(), i32q20_t::resolution);
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_forbidden__does_not_compile) {
    // Note: Although the value is inside the value range in this case, the compiler will add
    //       overflow checks as soon as the value range for sq is narrower (because the value of q
    //       might be changed before toSq() is called). Due to fpm::Ovf::error, this must not compile.
    using SqT = i32q20_t::Sq<i32q20_t::realMin + 1., i32q20_t::realMax - 1.>;
    ASSERT_FALSE(( CanConvertQToSq< i32q20_t, SqT > ));
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_asserted__does_not_compile) {
    // this should trigger an assertion trap at runtime
    //auto sqValue = i32q20_t::fromReal<-410.>().toSq<-400., 400., fpm::Ovf::assert>();
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_clamped__value_is_clamped) {
    constexpr double real = 450.;
    constexpr double sqRangeN = -400., sqRangeP = +400.;
    auto sqValueN = i32q20_t::fromReal<-real>().toSq<sqRangeN, sqRangeP, fpm::Ovf::clamp>();
    auto sqValueP = i32q20_t::fromReal<+real>().toSq<sqRangeN, sqRangeP, fpm::Ovf::clamp>();
    auto sqValueNClamp = i32q20_clamp_t::fromReal<-real>().toSq<sqRangeN, sqRangeP>();
    auto sqValuePClamp = i32q20_clamp_t::fromReal<+real>().toSq<sqRangeN, sqRangeP>();

    ASSERT_NEAR(sqRangeN, sqValueN.real(), i32q20_t::resolution);
    ASSERT_NEAR(sqRangeP, sqValueP.real(), i32q20_t::resolution);
    ASSERT_NEAR(sqRangeN, sqValueNClamp.real(), i32q20_clamp_t::resolution);
    ASSERT_NEAR(sqRangeP, sqValuePClamp.real(), i32q20_clamp_t::resolution);
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_allowed__value_can_overflow) {
    constexpr double real = 450.;
    auto sqValueN = i32q20_t::fromReal<-real>().toSq<-400., +400., fpm::Ovf::allowed>();
    auto sqValueP = i32q20_t::fromReal<+real>().toSq<-400., +400., fpm::Ovf::allowed>();
    auto sqValueNOvf = i32q20_ovf_t::fromReal<-real>().toSq<-400., +400.>();
    auto sqValuePOvf = i32q20_ovf_t::fromReal<+real>().toSq<-400., +400.>();

    ASSERT_NEAR(-real, sqValueN.real(), i32q20_t::resolution);
    ASSERT_NEAR(+real, sqValueP.real(), i32q20_t::resolution);
    ASSERT_NEAR(-real, sqValueNOvf.real(), i32q20_ovf_t::resolution);
    ASSERT_NEAR(+real, sqValuePOvf.real(), i32q20_ovf_t::resolution);
}

TEST_F(QTest_Sq, q_from_sq__from_narrower_value_range__q_value_equal_no_overflow_check) {
    constexpr double real = 234.5;
    using i32sq20_n_t = i32sq20< i32q20_t::realMin + 100., i32q20_t::realMax - 100. >;
    auto sqValue = i32sq20_n_t::fromReal<real>();
    auto qValue = i32q20_t::fromSq(sqValue);

    EXPECT_TRUE(( CanConvertSqToQImplicitly< i32sq20_n_t, i32q20_t > ));
    i32q20_t qValueI = sqValue;  // implicit conversion from similar type with narrower range

    ASSERT_NEAR(real, qValue.real(), i32q20_t::resolution);
    ASSERT_NEAR(real, qValueI.real(), i32q20_t::resolution);
}

TEST_F(QTest_Sq, q_from_sq__from_narrower_value_range_and_lower_f__q_value_upscaled_no_overflow_check) {
    constexpr double real = -234.5;
    using i32sq16_t = i32sq16< i32q20_t::realMin + 100., i32q20_t::realMax - 100. >;
    auto sqValue = i32sq16_t::fromReal<real>();
    auto qValue = i32q20_t::fromSq(sqValue);

    EXPECT_TRUE(( CanConvertSqToQImplicitly< i32sq16_t, i32q20_t > ));
    i32q20_t qValueI = sqValue;  // implicit conversion from narrower range

    ASSERT_NEAR(real, qValue.real(), i32q20_t::resolution);
    ASSERT_NEAR(real, qValueI.real(), i32q20_t::resolution);
}

TEST_F(QTest_Sq, q_from_sq__from_narrower_value_range_and_larger_f__q_value_downscaled_no_overflow_check) {
    constexpr double real = -94.5;
    using i32sq24_t = i32sq24< -100., +100. >;
    auto sqValue = i32sq24_t::fromReal<real>();
    auto qValue = i32q20_t::fromSq(sqValue);

    EXPECT_TRUE(( CanConvertSqToQImplicitly< i32sq24_t, i32q20_t > ));
    i32q20_t qValueI = sqValue;  // implicit conversion from narrower range

    ASSERT_NEAR(real, qValue.real(), i32q20_t::resolution);
    ASSERT_NEAR(real, qValueI.real(), i32q20_t::resolution);
}

TEST_F(QTest_Sq, q_from_sq__same_value_range_value_within_limits__q_value_equal_no_overflow_check) {
    constexpr double real = -234.5;
    auto sqValue = i32q20_t::fromReal<real>().toSq();
    auto qValue = i32q20_t::fromSq(sqValue);  // does not perform overflow checks in this case

    EXPECT_TRUE(( CanConvertSqToQImplicitly< i32q20_t::Sq<>, i32q20_t > ));
    i32q20_t qValueI = sqValue;  // implicit conversion from similar type with same range

    ASSERT_NEAR(real, qValue.real(), i32q20_t::resolution);
    ASSERT_NEAR(real, qValueI.real(), i32q20_t::resolution);
}

TEST_F(QTest_Sq, q_from_sq__from_larger_value_range__does_not_compile) {
    // sq with wider range cannot be converted to Q with a narrower range if overflow is forbidden
    using SqT = i32sq20<-1010., 1010.>;
    ASSERT_FALSE(( CanConvertSqToQExplicitly< SqT, i32q20_t > ));
    ASSERT_FALSE(( CanConvertSqToQImplicitly< SqT, i32q20_t > ));
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_clamp__q_value_is_clamped) {
    constexpr double real = 654.;
    auto sqValueN = i32q20_l_t::fromReal<-real>().toSq();
    auto sqValueP = i32q20_l_t::fromReal<+real>().toSq();
    auto qValueNClamp1 = i32q20_t::fromSq<fpm::Ovf::clamp>(sqValueN);
    auto qValuePClamp1 = i32q20_t::fromSq<fpm::Ovf::clamp>(sqValueP);
    auto qValueNClamp2 = i32q20_clamp_t::fromSq(sqValueN);
    auto qValuePClamp2 = i32q20_clamp_t::fromSq(sqValueP);

    ASSERT_NEAR(i32q20_t::realMin, qValueNClamp1.real(), i32q20_t::resolution);
    ASSERT_NEAR(i32q20_t::realMax, qValuePClamp1.real(), i32q20_t::resolution);
    ASSERT_NEAR(i32q20_clamp_t::realMin, qValueNClamp2.real(), i32q20_clamp_t::resolution);
    ASSERT_NEAR(i32q20_clamp_t::realMax, qValuePClamp2.real(), i32q20_clamp_t::resolution);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_overflow_allowed__q_value_can_overflow) {
    constexpr double real = 654.;
    auto sqValueN = i32q20_l_t::fromReal<-real>().toSq();
    auto sqValueP = i32q20_l_t::fromReal<+real>().toSq();
    auto qValueNOvf1 = i32q20_t::fromSq<fpm::Ovf::allowed>(sqValueN);
    auto qValuePOvf1 = i32q20_t::fromSq<fpm::Ovf::allowed>(sqValueP);
    auto qValueNOvf2 = i32q20_ovf_t::fromSq(sqValueN);
    auto qValuePOvf2 = i32q20_ovf_t::fromSq(sqValueP);

    ASSERT_NEAR(-real, qValueNOvf1.real(), i32q20_t::resolution);
    ASSERT_NEAR(+real, qValuePOvf1.real(), i32q20_t::resolution);
    ASSERT_NEAR(-real, qValueNOvf2.real(), i32q20_ovf_t::resolution);
    ASSERT_NEAR(+real, qValuePOvf2.real(), i32q20_ovf_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ---------------------------------- Q Test: Copy & Scale -------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_CopyScale : public ::testing::Test {
protected:
    using i16q4_t = i16q4<-2048., 2047.9>;  // overflow forbidden by default
    using i32q4_t = i32q4<-2048., 2048.>;  // overflow forbidden by default
    using i32q4_clamp_t = i32q4<-2048., 2048., fpm::Ovf::clamp>;
    using i32q4_ovf_t = i32q4<-2048., 2048., fpm::Ovf::allowed>;
    using i32qm2_t = i32qm2<-2048., 2048.>;  // overflow forbidden by default
    using i32qm2_clamp_t = i32qm2<-2048., 2048., fpm::Ovf::clamp>;
    using i32qm2_ovf_t = i32qm2<-2048., 2048., fpm::Ovf::allowed>;
    using i32q8_t = i32q8<-2048.1, 2048.1>;  // overflow forbidden by default
    using i32q8_clamp_t = i32q8<-2048.1, 2048.1, fpm::Ovf::clamp>;
    using i32q8_ovf_t = i32q8<-2048.1, 2048.1, fpm::Ovf::allowed>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_CopyScale, q_clamp_t__some_q_type__relimited_q_type) {
    constexpr double restrictedLimit = i32q4_t::realMax/2;
    constexpr double extendedLimit = i32q4_t::realMax*2;
    using restricted_q_t   = i32q4_t::clamp_t<-restrictedLimit,  +restrictedLimit,  fpm::Ovf::clamp>;
    using restricted_l_q_t = i32q4_t::clamp_t<-restrictedLimit,  i32q4_t::realMax, fpm::Ovf::clamp>;
    using restricted_r_q_t = i32q4_t::clamp_t<i32q4_t::realMin, +restrictedLimit,  fpm::Ovf::clamp>;
    using extended_q_t     = i32q4_t::clamp_t<-extendedLimit,    +extendedLimit,    fpm::Ovf::clamp>;
    using extended_l_q_t   = i32q4_t::clamp_t<-extendedLimit,    i32q4_t::realMax, fpm::Ovf::clamp>;
    using extended_r_q_t   = i32q4_t::clamp_t<i32q4_t::realMin, +extendedLimit,    fpm::Ovf::clamp>;
    using shifted_q_l_t    = i32q4_t::clamp_t<-extendedLimit,    +restrictedLimit,  fpm::Ovf::clamp>;
    using shifted_q_r_t    = i32q4_t::clamp_t<-restrictedLimit,  +extendedLimit,    fpm::Ovf::clamp>;

    ASSERT_TRUE((std::is_same_v< i32q4<-restrictedLimit,  +restrictedLimit,  fpm::Ovf::clamp>, restricted_q_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<-restrictedLimit,  i32q4_t::realMax, fpm::Ovf::clamp>, restricted_l_q_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<i32q4_t::realMin, +restrictedLimit,  fpm::Ovf::clamp>, restricted_r_q_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<-extendedLimit,    +extendedLimit,    fpm::Ovf::clamp>, extended_q_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<-extendedLimit,    i32q4_t::realMax, fpm::Ovf::clamp>, extended_l_q_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<i32q4_t::realMin, +extendedLimit,    fpm::Ovf::clamp>, extended_r_q_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<-extendedLimit,    +restrictedLimit,  fpm::Ovf::clamp>, shifted_q_l_t >));
    ASSERT_TRUE((std::is_same_v< i32q4<-restrictedLimit,  +extendedLimit,    fpm::Ovf::clamp>, shifted_q_r_t >));
}

TEST_F(QTest_CopyScale, q_copy_constructor__int16_someF__int16_sameF) {
    constexpr double realA = -1024.2;
    auto a = i16q4_t::fromReal<realA>();
    auto b = i16q4_t(a);  // explicit copy-constructor from same Q-type
    auto c = i16q4_t::fromQ(a);  // copy-construction via named constructor
    i16q4_t d = b;  // implicit copy-construction

    ASSERT_NEAR(realA, b.real(), i16q4_t::resolution);
    ASSERT_NEAR(realA, c.real(), i16q4_t::resolution);
    ASSERT_NEAR(realA, d.real(), i16q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_move_constructor__int16_someF__int16_sameF) {
    constexpr double realA = -1024.2;
    auto a = i16q4_t::fromReal<realA>();
    i16q4_t b = std::move(a);

    ASSERT_NEAR(realA, b.real(), i16q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double realA = -1024.2;
    auto a = i32q4_t::fromReal<realA>();
    auto b = i32q8_t::fromQ(a);  // no overflow check needed here
    i32q8_t c = a;

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       q4 type and does not change if the value is up-scaled to a larger f
    ASSERT_NEAR(realA, b.real(), i32q4_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_clamp__int16_someF__int16_largerF) {
    constexpr double realA = -2060.;
    auto a = i32q4_ovf_t::fromReal<realA>();
    auto b = i32q8_t::fromQ<fpm::Ovf::clamp>(a);
    auto c = i32q8_clamp_t::fromQ(a);
    i32q8_clamp_t d = a;

    ASSERT_NEAR(i32q8_clamp_t::realMin, b.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR(i32q8_clamp_t::realMin, c.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR(i32q8_clamp_t::realMin, d.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_overflow_allowed__int16_someF__int16_largerF) {
    constexpr double realA = -2060.;
    auto a = i32q4_ovf_t::fromReal<realA>();
    auto b = i32q8_t::fromQ<fpm::Ovf::allowed>(a);
    auto c = i32q8_ovf_t::fromQ(a);
    i32q8_ovf_t d = a;

    ASSERT_NEAR(realA, b.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR(realA, d.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double realA = -1024.2;
    auto a = i32q4_t::fromReal<realA>();
    auto b = i32qm2_t::fromQ(a);  // no overflow check needed here
    i32qm2_t c = a;  // no overflow check needed here

    // note: for down-scaling to a smaller f, the representation error is at most the sum of the two
    //       resolutions before and after the scaling operation
    ASSERT_NEAR(realA, b.real(), i32q4_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_t::resolution + i32qm2_t::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_clamp__int16_someF__int16_smallerF) {
    constexpr double realA = -2060.;
    auto a = i32q4_ovf_t::fromReal<realA>();
    auto b = i32qm2_t::fromQ<fpm::Ovf::clamp>(a);
    auto c = i32qm2_clamp_t::fromQ(a);
    i32qm2_clamp_t d = a;

    ASSERT_NEAR(i32qm2_clamp_t::realMin, b.real(), i32q4_ovf_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR(i32qm2_clamp_t::realMin, c.real(), i32q4_ovf_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_NEAR(i32qm2_clamp_t::realMin, d.real(), i32q4_ovf_t::resolution + i32qm2_clamp_t::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_overflow_allowed__int16_someF__int16_smallerF) {
    constexpr double realA = -2060.;
    auto a = i32q4_ovf_t::fromReal<realA>();
    auto b = i32qm2_t::fromQ<fpm::Ovf::allowed>(a);
    auto c = i32qm2_ovf_t::fromQ(a);
    i32qm2_ovf_t d = a;

    ASSERT_NEAR(realA, b.real(), i32q4_ovf_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_ovf_t::resolution + i32qm2_ovf_t::resolution);
    ASSERT_NEAR(realA, d.real(), i32q4_ovf_t::resolution + i32qm2_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_assignment__same_q_type__value_is_copied) {
    constexpr double realA = -1024.2;
    auto a = i32q4_t::fromReal<realA>();
    auto b = i32q4_t::fromScaled<0>();
    b = a;  // copy-assignment (no runtime checks needed here)

    ASSERT_NEAR(realA, b.real(), i32q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_assignment__different_q_type__value_is_scaled_and_assigned) {
    constexpr double realA = -1024.2;
    auto a = i32q4_t::fromReal<realA>();
    auto b = i32qm2_t::fromScaled<0>();
    auto c = i32q8_t::fromScaled<0>();
    b = a;  // downscale-assignment (no runtime checks needed here)
    c = a;  // upscale-assignment (no runtime checks needed here)

    ASSERT_NEAR(realA, b.real(), i32q4_t::resolution + i32qm2_t::resolution);  // downscale decreases resolution
    ASSERT_NEAR(realA, c.real(), i32q4_t::resolution);  // upscale does not change initial resolution
}

TEST_F(QTest_CopyScale, q_assignment_clamp__different_q_type__value_is_scaled_and_assigned) {
    constexpr double realA = -2060.;
    auto a = i32q4_ovf_t::fromReal<realA>();
    auto b = i32qm2_clamp_t::fromScaled<0>();
    auto c = i32q8_clamp_t::fromScaled<0>();
    b = a;  // assignment from different type
    c = a;

    ASSERT_NEAR(i32qm2_clamp_t::realMin, b.real(), i32q4_ovf_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_NEAR(i32q8_clamp_t::realMin, c.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_assignment_overflow__different_q_type__value_is_scaled_and_assigned) {
    constexpr double realA = -2060.;
    auto a = i32q4_ovf_t::fromReal<realA>();
    auto b = i32qm2_ovf_t::fromScaled<0>();
    auto c = i32q8_ovf_t::fromScaled<0>();
    b = a;  // assignment from different type
    c = a;

    ASSERT_NEAR(realA, b.real(), i32q4_ovf_t::resolution + i32qm2_ovf_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_copy_constructor__int16_someF_literal__int16_sameF) {
    i16q4_t a = 1024_i16q4;  // implicit copy-construction
    auto b = i16q4_t(1024_i16q4);  // explicit copy-constructor from same Q-type
    auto c = i16q4_t::fromQ(1024_i16q4);  // copy-construction via named constructor

    ASSERT_NEAR((1024_i16q4).real(), a.real(), i16q4_t::resolution);
    ASSERT_NEAR((1024_i16q4).real(), b.real(), i16q4_t::resolution);
    ASSERT_NEAR((1024_i16q4).real(), c.real(), i16q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_move_constructor__int16_someF_literal__int16_sameF) {
    i16q4_t b = std::move(1024_i16q4);

    ASSERT_NEAR((1024_i16q4).real(), b.real(), i16q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor__int16_someF_literal__int16_largerF) {
    i32q8_t a = 1024_i32q4;
    auto b = i32q8_t::fromQ(1024_i32q4);  // no overflow check needed here

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       q4 type and does not change if the value is up-scaled to a larger f
    ASSERT_NEAR((1024_i32q4).real(), a.real(), i32q4_t::resolution);
    ASSERT_NEAR((1024_i32q4).real(), b.real(), i32q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_clamp__int16_someF_literal__int16_largerF) {
    i32q8_clamp_t a = 2060_i32q4;
    auto b = i32q8_t::fromQ<fpm::Ovf::clamp>(2060_i32q4);
    auto c = i32q8_clamp_t::fromQ(2060_i32q4);

    ASSERT_NEAR(i32q8_clamp_t::realMax, a.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR(i32q8_clamp_t::realMax, b.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR(i32q8_clamp_t::realMax, c.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_overflow_allowed__int16_someF_literal__int16_largerF) {
    i32q8_ovf_t a = 2060_i32q4;
    auto b = i32q8_t::fromQ<fpm::Ovf::allowed>(2060_i32q4);
    auto c = i32q8_ovf_t::fromQ(2060_i32q4);

    ASSERT_NEAR((2060_i32q4).real(), a.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR((2060_i32q4).real(), b.real(), i32q4_ovf_t::resolution);
    ASSERT_NEAR((2060_i32q4).real(), c.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor__int16_someF_literal__int16_smallerF) {
    i32qm2_t a = 1024_i32q4;  // no overflow check needed here
    auto b = i32qm2_t::fromQ(1024_i32q4);  // no overflow check needed here

    // note: for down-scaling to a smaller f, the representation error is at most the sum of the two
    //       resolutions before and after the scaling operation
    ASSERT_NEAR((1024_i32q4).real(), a.real(), i32q4_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR((1024_i32q4).real(), b.real(), i32q4_t::resolution + i32qm2_t::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_clamp__int16_someF_literal__int16_smallerF) {
    i32qm2_clamp_t a = 2060_i32q4;
    auto b = i32qm2_t::fromQ<fpm::Ovf::clamp>(2060_i32q4);
    auto c = i32qm2_clamp_t::fromQ(2060_i32q4);

    ASSERT_NEAR(i32qm2_clamp_t::realMax, a.real(), i32q4_ovf_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR(i32qm2_clamp_t::realMax, b.real(), i32q4_ovf_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_NEAR(i32qm2_clamp_t::realMax, c.real(), i32q4_ovf_t::resolution + i32qm2_clamp_t::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_overflow_allowed__int16_someF_literal__int16_smallerF) {
    i32qm2_ovf_t a = 2060_i32q4;
    auto b = i32qm2_t::fromQ<fpm::Ovf::allowed>(2060_i32q4);
    auto c = i32qm2_ovf_t::fromQ(2060_i32q4);

    ASSERT_NEAR((2060_i32q4).real(), a.real(), i32q4_ovf_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR((2060_i32q4).real(), b.real(), i32q4_ovf_t::resolution + i32qm2_ovf_t::resolution);
    ASSERT_NEAR((2060_i32q4).real(), c.real(), i32q4_ovf_t::resolution + i32qm2_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_assignment__same_q_type_literal__value_is_copied) {
    auto a = i32q4_t::fromScaled<0>();
    a = 1024_i32q4;  // copy-assignment (no runtime checks needed here)

    ASSERT_NEAR((1024_i32q4).real(), a.real(), i32q4_t::resolution);
}

TEST_F(QTest_CopyScale, q_assignment__different_q_type_literal__value_is_scaled_and_assigned) {
    auto a = i32qm2_t::fromScaled<0>();
    auto b = i32q8_t::fromScaled<0>();
    a = 1024_i32q4;  // downscale-assignment (no runtime checks needed here)
    b = 1024_i32q4;  // upscale-assignment (no runtime checks needed here)

    ASSERT_NEAR((1024_i32q4).real(), a.real(), i32q4_t::resolution + i32qm2_t::resolution);  // downscale decreases resolution
    ASSERT_NEAR((1024_i32q4).real(), b.real(), i32q4_t::resolution);  // upscale does not change initial resolution
}

TEST_F(QTest_CopyScale, q_assignment_clamp__different_q_type_literal__value_is_scaled_and_assigned) {
    auto a = i32qm2_clamp_t::fromScaled<0>();
    auto b = i32q8_clamp_t::fromScaled<0>();
    a = 2060_i32q4;  // assignment from different type
    b = 2060_i32q4;

    ASSERT_NEAR(i32qm2_clamp_t::realMax, a.real(), i32q4_ovf_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_NEAR(i32q8_clamp_t::realMax, b.real(), i32q4_ovf_t::resolution);
}

TEST_F(QTest_CopyScale, q_assignment_overflow__different_q_type_literal__value_is_scaled_and_assigned) {
    auto a = i32qm2_ovf_t::fromScaled<0>();
    auto b = i32q8_ovf_t::fromScaled<0>();
    a = 2060_i32q4;  // assignment from different type
    b = 2060_i32q4;

    ASSERT_NEAR((2060_i32q4).real(), a.real(), i32q4_ovf_t::resolution + i32qm2_ovf_t::resolution);
    ASSERT_NEAR((2060_i32q4).real(), b.real(), i32q4_ovf_t::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Casting ----------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Casting : public ::testing::Test {
protected:
    using i32q20_t = i32q20<-500., 500.>;  // overflow forbidden by default
    using i32q20_clamp_t = i32q20<-500., 500., fpm::Ovf::clamp>;
    using i32q20_ovf_t = i32q20<-500., 500., fpm::Ovf::allowed>;

    using i32q4_t = i32q4<-2048., 2048.>;  // overflow forbidden by default
    using i32q4_clamp_t = i32q4<-2048., 2048., fpm::Ovf::clamp>;
    using i32q4_ovf_t = i32q4<-2048., 2048., fpm::Ovf::allowed>;
    using i32qm2_t = i32qm2<-2048., 2048.>;  // overflow forbidden by default
    using i32qm2_clamp_t = i32qm2<-2048., 2048., fpm::Ovf::clamp>;
    using i32qm2_ovf_t = i32qm2<-2048., 2048., fpm::Ovf::allowed>;

    using u16q6_t = u16q6<0., 500.>;  // overflow forbidden by default
    using u16q6_clamp_t = u16q6<0., 500., fpm::Ovf::clamp>;
    using u16q6_ovf_t = u16q6<0., 500., fpm::Ovf::allowed>;
    using u32qm2_t = u32qm2<0., 2048.>;  // overflow forbidden by default
    using u32qm2_clamp_t = u32qm2<0., 2048., fpm::Ovf::clamp>;
    using u32qm2_ovf_t = u32qm2<0., 2048., fpm::Ovf::allowed>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed__unsigned_type_smallerF_same_value) {
    constexpr double realA = 1024.2;
    auto a = i32q4_t::fromReal<realA>();
    auto b = static_cast<u32qm2_clamp_t>(a);
    auto c = static_q_cast<u32qm2_t, fpm::Ovf::clamp>(a);
    auto d = static_q_cast<u32qm2_clamp_t>(a);

    // static cast from i32q4_t to u32q4_t must not work here because first range is wider than second
    // and types have fpm::Ovf::error
    ASSERT_FALSE(( StaticCastable< i32q4_t, u32qm2_t > ));

    ASSERT_NEAR(realA, b.real(), i32q4_t::resolution + u32qm2_clamp_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_t::resolution + u32qm2_t::resolution);
    ASSERT_NEAR(realA, d.real(), i32q4_t::resolution + u32qm2_clamp_t::resolution);
    ASSERT_TRUE((std::is_same_v<u32qm2_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u32qm2_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u32qm2_clamp_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr double realA = 498.7;
    auto a = u16q6_t::fromReal<realA>();
    auto b = static_cast<i32q20_clamp_t>(a);
    auto c = static_q_cast<i32q20_t, fpm::Ovf::clamp>(a);
    auto d = static_q_cast<i32q20_clamp_t>(a);

    // static cast from u16q6_t to i32q20_t works here, because the signed type includes the range of the unsigned type
    ASSERT_TRUE(( StaticCastable< u16q6_t, i32q20_t > ));

    // note: for up-scaling to a larger integral type, the resulting resolution is the resolution
    //       of the source type (because the base type of both target and source is integral)
    ASSERT_NEAR(realA, b.real(), u16q6_t::resolution);
    ASSERT_NEAR(realA, c.real(), u16q6_t::resolution);
    ASSERT_NEAR(realA, d.real(), u16q6_t::resolution);
    ASSERT_TRUE((std::is_same_v<i32q20_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32q20_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<i32q20_clamp_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr double realA = 498.7;
    auto a = u16q6_t::fromReal<realA>();
    auto b = static_cast<i32qm2_clamp_t>(a);
    auto c = static_q_cast<i32qm2_t, fpm::Ovf::clamp>(a);
    auto d = static_q_cast<i32qm2_clamp_t>(a);

    // static cast from u16q6_t to i32qm2_t works here, because the signed type includes the range of the unsigned type
    ASSERT_TRUE(( StaticCastable< u16q6_t, i32qm2_t > ));

    ASSERT_NEAR(realA, b.real(), u16q6_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_NEAR(realA, c.real(), u16q6_t::resolution + i32qm2_t::resolution);
    ASSERT_NEAR(realA, d.real(), u16q6_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_TRUE((std::is_same_v<i32qm2_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32qm2_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<i32qm2_clamp_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr double realA = 498.7;
    auto a = i32q4_t::fromReal<realA>();
    auto b = static_cast<u16q6_clamp_t>(a);
    auto c = static_q_cast<u16q6_t, fpm::Ovf::clamp>(a);
    auto d = static_q_cast<u16q6_clamp_t>(a);

    // static cast from i32q4_t to u16q6_t must not work here because first range is wider than second
    // and types have fpm::Ovf::error
    ASSERT_FALSE(( StaticCastable< i32q4_t, u16q6_t > ));

    ASSERT_NEAR(realA, b.real(), u16q6_clamp_t::resolution);
    ASSERT_NEAR(realA, c.real(), u16q6_t::resolution);
    ASSERT_NEAR(realA, d.real(), u16q6_clamp_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed_too_large__smaller_unsigned_type_largerF_value_clamped) {
    constexpr double realA = 512.5;
    auto a = i32q4_t::fromReal<realA>();
    auto b = static_cast<u16q6_clamp_t>(a);
    auto c = static_q_cast<u16q6_t, fpm::Ovf::clamp>(a);
    auto d = static_q_cast<u16q6_clamp_t>(a);

    ASSERT_NEAR(u16q6_clamp_t::realMax, b.real(), u16q6_clamp_t::resolution);
    ASSERT_NEAR(u16q6_t::realMax, c.real(), u16q6_t::resolution);
    ASSERT_NEAR(u16q6_clamp_t::realMax, d.real(), u16q6_clamp_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__negative_real_value__smaller_unsigned_type_largerF_clamped_value) {
    constexpr double realA = -498.7;
    auto a = i32q4_t::fromReal<realA>();
    auto b = static_cast<u16q6_clamp_t>(a);
    auto c = static_q_cast<u16q6_t, fpm::Ovf::clamp>(a);
    auto d = static_q_cast<u16q6_clamp_t>(a);

    ASSERT_NEAR(u16q6_clamp_t::realMin, b.real(), u16q6_clamp_t::resolution);
    ASSERT_NEAR(u16q6_t::realMin, c.real(), u16q6_t::resolution);
    ASSERT_NEAR(u16q6_clamp_t::realMin, d.real(), u16q6_clamp_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__negative_real_value__smaller_unsigned_type_largerF_value_overflow) {
    constexpr double realA = -498.7;
    auto a = i32q4_t::fromReal<realA>();
    auto b = static_cast<u16q6_ovf_t>(a);  // this is possible, however hardly predictable if a is signed
    auto c = static_q_cast<u16q6_t, fpm::Ovf::allowed>(a);
    auto d = static_q_cast<u16q6_ovf_t>(a);

    constexpr double expectedValue = 525.3125;
    ASSERT_NEAR(expectedValue, b.real(), u16q6_ovf_t::resolution);
    ASSERT_NEAR(expectedValue, c.real(), u16q6_t::resolution);
    ASSERT_NEAR(expectedValue, d.real(), u16q6_ovf_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_ovf_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_ovf_t, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__small_signed_type_2_large_unsigned_type__expected_real_value) {
    // i8[min,max]:
    // i8::min     min        0         max      i8::max
    // |------------|---------|----------|-----------|
    //              ^^^^^^^^^^^^^^^^^^^^^^  <= user value range i8
    //
    // static_cast< u32[MIN,MAX] >( i8_scaled[min,max] ):
    // 0         MIN                                   MAX    u32::max
    // |----------|-----|-----------||-----------|------|---------|
    // |               max   i8::max   i8::min  min               |
    // ^^^^^^^^^^^^^^^^^^                        ^^^^^^^^^^^^^^^^^^  <= scaled i8 user value range in u32
    //
    // note: every i8 < 0 before cast will result in u32 > i8::max; these values are mapped to u32::MIN

    using i8qm3_t = i8qm3<-1000., 1000.>;  // i8 -> u32, max delta f is 25
    using u32q22_t = u32q22<800., 1020.>;
    auto a = i8qm3_t::fromReal<-1010., fpm::Ovf::allowed>();
    auto b = i8qm3_t::fromReal<i8qm3_t::realMin>();
    auto c = i8qm3_t::fromReal<500.>();
    auto d = i8qm3_t::fromReal<900.>();
    auto e = i8qm3_t::fromReal<i8qm3_t::realMax>();
    auto f = i8qm3_t::fromReal<1010., fpm::Ovf::allowed>();
    auto g = i8qm3_t::fromReal<1022., fpm::Ovf::allowed>();
    auto ac = static_q_cast<u32q22_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(b);
    auto bc_ovf = static_q_cast<u32q22_t, fpm::Ovf::allowed>(b);
    auto cc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<u32q22_t, fpm::Ovf::clamp>(e);
    auto fc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(f);
    auto gc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(g);
    auto gc_ovf = static_q_cast<u32q22_t, fpm::Ovf::allowed>(g);

    ASSERT_NEAR(u32q22_t::realMin, ac.real(), i8qm3_t::resolution);  // (u32::max - 1010*2^22) / 2^22 clamped
    ASSERT_NEAR(u32q22_t::realMin, bc.real(), i8qm3_t::resolution);  // (u32::max - 1000*2^22) / 2^22 clamped
    ASSERT_NEAR(23.99999976, bc_ovf.real(), i8qm3_t::resolution);    // (u32::max - 1000*2^22) / 2^22
    ASSERT_NEAR(u32q22_t::realMin, cc.real(), i8qm3_t::resolution);  // 500 clamped to u32q22::realMin
    ASSERT_NEAR( 900., dc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(i8qm3_t::realMax, ec.real(), i8qm3_t::resolution);
    ASSERT_NEAR(1010., fc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(u32q22_t::realMax, gc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(1022., gc_ovf.real(), i8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__smaller_signed_type_2_larger_unsigned_type__expected_real_value) {
    // i16[min,max]:
    // i16::min    min        0         max      i16::max
    // |------------|---------|----------|-----------|
    //              ^^^^^^^^^^^^^^^^^^^^^^  <= user value range i16
    //
    // static_cast< u32[MIN,MAX] >( i16_scaled[min,max] ):
    // 0         MIN                                   MAX    u32::max
    // |----------|-----|-----------||-----------|------|---------|
    // |               max  i16::max  i16::min  min               |
    // ^^^^^^^^^^^^^^^^^^                        ^^^^^^^^^^^^^^^^^^  <= scaled i16 user value range in u32
    //
    // note: every i16 < 0 before cast will result in u32 > i16::max; these values are mapped to u32::MIN

    using i16qm3_t = i16qm3<-100000., 100000.>;  // i16 -> u32, max delta f is 17
    using u32q14_t = u32q14<80000., 160000.>;
    auto a = i16qm3_t::fromReal<-110000., fpm::Ovf::allowed>();
    auto b = i16qm3_t::fromReal<i16qm3_t::realMin>();
    auto c = i16qm3_t::fromReal<50000.>();
    auto d = i16qm3_t::fromReal<90000.>();
    auto e = i16qm3_t::fromReal<i16qm3_t::realMax>();
    auto f = i16qm3_t::fromReal<110000., fpm::Ovf::allowed>();
    auto g = i16qm3_t::fromReal<200000., fpm::Ovf::allowed>();
    auto ac = static_q_cast<u32q14_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(b);
    auto bc_ovf = static_q_cast<u32q14_t, fpm::Ovf::allowed>(b);
    auto cc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<u32q14_t, fpm::Ovf::clamp>(e);
    auto fc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(f);
    auto gc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(g);
    auto gc_ovf = static_q_cast<u32q14_t, fpm::Ovf::allowed>(g);

    ASSERT_NEAR(u32q14_t::realMin, ac.real(), i16qm3_t::resolution);  // (u32::max - 110000*2^14) / 2^14 clamped
    ASSERT_NEAR(u32q14_t::realMin, bc.real(), i16qm3_t::resolution);  // (u32::max - 100000*2^14) / 2^14 clamped
    ASSERT_NEAR(162144., bc_ovf.real(), i16qm3_t::resolution);  // (u32::max - 100000*2^14) / 2^14
    ASSERT_NEAR(u32q14_t::realMin, cc.real(), i16qm3_t::resolution);  // 50000 clamped to u32q14::realMin
    ASSERT_NEAR( 90000., dc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(i16qm3_t::realMax, ec.real(), i16qm3_t::resolution);
    ASSERT_NEAR(110000., fc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(u32q14_t::realMax, gc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(200000., gc_ovf.real(), i16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__small_unsigned_type_2_large_signed_type__expected_real_value) {
    // u8[min,max]:
    // 0           min                 max          u8::max
    // |------------|----------|--------|--------------|
    //              ^^^^^^^^^^^^^^^^^^^^^  <= user value range u8
    //
    // static_cast< i32[MIN,MAX] >( u8_scaled[min,max] ):
    // i32::min  MIN                0                 MAX    i32::max
    // |----------|----|------------|-------|----------|---------|
    // |              max                  min                   |
    // ^^^^^^^^^^^^^^^^^                    ^^^^^^^^^^^^^^^^^^^^^^  <= scaled u8 user value range in i32
    //
    // note: everything > u8::max/2 before cast will result in negative i32; these values are mapped to i32::MAX

    using u8qm3_t = u8qm3<0., 1000.>;  // u8 -> i32, max delta f is 23
    using i32q20_t = i32q20<-800., 1020.>;
    auto a = u8qm3_t::fromReal<u8qm3_t::realMin>();
    auto b = u8qm3_t::fromReal<500.>();
    auto c = u8qm3_t::fromReal<u8qm3_t::realMax>();
    auto d = u8qm3_t::fromReal<1010., fpm::Ovf::allowed>();
    auto e = u8qm3_t::fromReal<1022., fpm::Ovf::allowed>();
    auto ac = static_q_cast<i32q20_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<i32q20_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i32q20_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i32q20_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i32q20_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i32q20_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(u8qm3_t::realMin, ac.real(), u8qm3_t::resolution);
    ASSERT_NEAR( 500., bc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(u8qm3_t::realMax, cc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(1010., dc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(i32q20_t::realMax, ec.real(), u8qm3_t::resolution);  // 1022 clamped to i32q20_t::realMax
    ASSERT_NEAR(1022., ec_ovf.real(), u8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__smaller_unsigned_type_2_larger_signed_type__expected_real_value) {
    // u16[min,max]:
    // 0           min                 max         u16::max
    // |------------|----------|--------|--------------|
    //              ^^^^^^^^^^^^^^^^^^^^^  <= user value range u16
    //
    // static_cast< i32[MIN,MAX] >( u16_scaled[min,max] ):
    // i32::min  MIN                0                 MAX    i32::max
    // |----------|----|------------|-------|----------|---------|
    // |              max                  min                   |
    // ^^^^^^^^^^^^^^^^^                    ^^^^^^^^^^^^^^^^^^^^^^  <= scaled u16 user value range in i32
    //
    // note: everything > u16::max/2 before cast will result in negative i32; these values are mapped to i32::MAX

    using u16qm3_t = u16qm3<0., 400000.>;  // u16 -> i32, max delta f is 15
    using i32q12_t = i32q12<-80000., 500000.>;
    auto a = u16qm3_t::fromReal<u16qm3_t::realMin>();
    auto b = u16qm3_t::fromReal<50000.>();
    auto c = u16qm3_t::fromReal<u16qm3_t::realMax>();
    auto d = u16qm3_t::fromReal<410000., fpm::Ovf::allowed>();
    auto e = u16qm3_t::fromReal<520000., fpm::Ovf::allowed>();
    auto ac = static_q_cast<i32q12_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<i32q12_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i32q12_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i32q12_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i32q12_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i32q12_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(u16qm3_t::realMin, ac.real(), u16qm3_t::resolution);
    ASSERT_NEAR( 50000., bc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(u16qm3_t::realMax, cc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(410000., dc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(i32q12_t::realMax, ec.real(), u16qm3_t::resolution);  // 520000 clamped to i32q12::realMax
    ASSERT_NEAR(520000., ec_ovf.real(), u16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__small_signed_type_2_large_signed_type__expected_real_value) {
    // i8[min,max]:
    // i8::min       min       0       max          i8::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= i8 user value range
    //
    // static_cast< i32[MIN,MAX] >( i8_scaled[min,max] ):
    // i32::min  MIN           0            MAX    i32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled i8 user value range before clamping
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= i32 user value range (clamped value range)

    using i8qm4_t = i8qm4<-2000., 2000.>;  // i8 -> i32, max delta f is 24
    using i32q20_t = i32q20<-1000., 2020.>;
    auto a = i8qm4_t::fromReal<i8qm4_t::realMin>();
    auto b = i8qm4_t::fromReal<-500.>();
    auto c = i8qm4_t::fromReal<i8qm4_t::realMax>();
    auto d = i8qm4_t::fromReal<2010., fpm::Ovf::allowed>();
    auto e = i8qm4_t::fromReal<2030., fpm::Ovf::allowed>();
    auto ac = static_q_cast<i32q20_t, fpm::Ovf::clamp>(a);
    auto ac_ovf = static_q_cast<i32q20_t, fpm::Ovf::allowed>(a);
    auto bc = static_q_cast<i32q20_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i32q20_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i32q20_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i32q20_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i32q20_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(i32q20_t::realMin, ac.real(), i8qm4_t::resolution);  // -2000 clamped to i32q12::realMin
    ASSERT_NEAR(i8qm4_t::realMin, ac_ovf.real(), i8qm4_t::resolution);  // -2000 not clamped
    ASSERT_NEAR(-500., bc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i8qm4_t::realMax, cc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(2010., dc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i32q20_t::realMax, ec.real(), i8qm4_t::resolution);  // 2010 clamped to i32q12::realMax
    ASSERT_NEAR(2030., ec_ovf.real(), i8qm4_t::resolution);  // not clamped
}

TEST_F(QTest_Casting, q_static_cast__smaller_signed_type_2_larger_signed_type__expected_real_value) {
    // i16[min,max]:
    // i16::min      min       0       max         i16::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= i16 user value range
    //
    // static_cast< i32[MIN,MAX] >( i16_scaled[min,max] ):
    // i32::min  MIN           0            MAX    i32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled i16 user value range before clamping
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= i32 user value range (clamped value range)

    using i16qm4_t = i16qm4<-400000., 400000.>;  // i16 -> i32, max delta f is 16
    using i32q12_t = i32q12<-80000., 500000.>;
    auto a = i16qm4_t::fromReal<i16qm4_t::realMin>();
    auto b = i16qm4_t::fromReal<-50000.>();
    auto c = i16qm4_t::fromReal<i16qm4_t::realMax>();
    auto d = i16qm4_t::fromReal<410000., fpm::Ovf::allowed>();
    auto e = i16qm4_t::fromReal<520000., fpm::Ovf::allowed>();
    auto ac = static_q_cast<i32q12_t, fpm::Ovf::clamp>(a);
    auto ac_ovf = static_q_cast<i32q12_t, fpm::Ovf::allowed>(a);
    auto bc = static_q_cast<i32q12_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i32q12_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i32q12_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i32q12_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i32q12_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(i32q12_t::realMin, ac.real(), i16qm4_t::resolution);  // -400000 clamped to i32q12::realMin
    ASSERT_NEAR(i16qm4_t::realMin, ac_ovf.real(), i16qm4_t::resolution);  // -400000 not clamped
    ASSERT_NEAR(-50000., bc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, cc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(410000., dc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i32q12_t::realMax, ec.real(), i16qm4_t::resolution);  // 520000 clamped to i32q12::realMax
    ASSERT_NEAR(520000., ec_ovf.real(), i16qm4_t::resolution);  // not clamped
}

TEST_F(QTest_Casting, q_static_cast__small_unsigned_type_2_large_unsigned_type__expected_real_value) {
    // u8[min,max]:
    // 0             min               max         u8::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= u8 user value range
    //
    // static_cast< u32[MIN,MAX] >( u8_scaled[min,max] ):
    // 0         MIN                        MAX    u32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled u8 user value range before clamping
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= u32 user value range (clamped value range)

    using u8qm2_t = u8qm2<100., 1000.>;  // u8 -> u32, max delta f is 24
    using u32q22_t = u32q22<800., 1020.>;
    auto a = u8qm2_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u8qm2_t::fromReal<u8qm2_t::realMin>();
    auto c = u8qm2_t::fromReal<500.>();
    auto d = u8qm2_t::fromReal<900.>();
    auto e = u8qm2_t::fromReal<u8qm2_t::realMax>();
    auto f = u8qm2_t::fromReal<1023., fpm::Ovf::allowed>();
    auto ac = static_q_cast<u32q22_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<u32q22_t, fpm::Ovf::clamp>(e);
    auto fc = static_q_cast<u32q22_t, fpm::Ovf::clamp>(f);
    auto fc_ovf = static_q_cast<u32q22_t, fpm::Ovf::allowed>(f);

    ASSERT_NEAR(u32q22_t::realMin, ac.real(), u8qm2_t::resolution);  // 0 clamped to u32q14::realMin
    ASSERT_NEAR(u32q22_t::realMin, bc.real(), u8qm2_t::resolution);  // 100 clamped to u32q14::realMin
    ASSERT_NEAR(u32q22_t::realMin, cc.real(), u8qm2_t::resolution);  // 500 clamped to u32q14::realMin
    ASSERT_NEAR( 900., dc.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u8qm2_t::realMax, ec.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u32q22_t::realMax, fc.real(), u8qm2_t::resolution);  // 1023 clamped to u32q14::realMax
    ASSERT_NEAR(1023., fc_ovf.real(), u8qm2_t::resolution);  // not clamped
}

TEST_F(QTest_Casting, q_static_cast__smaller_unsigned_type_2_larger_unsigned_type__expected_real_value) {
    // u16[min,max]:
    // 0             min               max         u16::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= u16 user value range
    //
    // static_cast< u32[MIN,MAX] >( u16_scaled[min,max] ):
    // 0         MIN                        MAX    u32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled u16 user value range before clamping
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= u32 user value range (clamped value range)

    using u16qm2_t = u16qm2<10000., 100000.>;  // u16 -> u32, max delta f is 16
    using u32q14_t = u32q14<80000., 160000.>;
    auto a = u16qm2_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u16qm2_t::fromReal<u16qm2_t::realMin>();
    auto c = u16qm2_t::fromReal<50000.>();
    auto d = u16qm2_t::fromReal<90000.>();
    auto e = u16qm2_t::fromReal<u16qm2_t::realMax>();
    auto f = u16qm2_t::fromReal<210000., fpm::Ovf::allowed>();
    auto ac = static_q_cast<u32q14_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<u32q14_t, fpm::Ovf::clamp>(e);
    auto fc = static_q_cast<u32q14_t, fpm::Ovf::clamp>(f);
    auto fc_ovf = static_q_cast<u32q14_t, fpm::Ovf::allowed>(f);

    ASSERT_NEAR(u32q14_t::realMin, ac.real(), u16qm2_t::resolution);  // 0 clamped to u32q14::realMin
    ASSERT_NEAR(u32q14_t::realMin, bc.real(), u16qm2_t::resolution);  // 10000 clamped to u32q14::realMin
    ASSERT_NEAR(u32q14_t::realMin, cc.real(), u16qm2_t::resolution);  // 50000 clamped to u32q14::realMin
    ASSERT_NEAR( 90000., dc.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u16qm2_t::realMax, ec.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u32q14_t::realMax, fc.real(), u16qm2_t::resolution);  // 210000 clamped to u32q14::realMax
    ASSERT_NEAR(210000., fc_ovf.real(), u16qm2_t::resolution);  // not clamped
}

TEST_F(QTest_Casting, q_static_cast__large_unsigned_type_2_small_signed_type__expected_real_value) {
    using u32q22_t = u32q22<800., 1020.>;
    using i8qm3_t = i8qm3<-1000., 1000.>;  // u32 -> i8, max delta f is 25
    auto a = u32q22_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q22_t::fromReal<u32q22_t::realMin>();
    auto c = u32q22_t::fromReal<900.>();
    auto d = u32q22_t::fromReal<i8qm3_t::realMax>();
    auto e = u32q22_t::fromReal<u32q22_t::realMax>();
    auto ac = static_q_cast<i8qm3_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<i8qm3_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i8qm3_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i8qm3_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i8qm3_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i8qm3_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(   0., ac.real(), i8qm3_t::resolution);
    ASSERT_NEAR(u32q22_t::realMin, bc.real(), i8qm3_t::resolution);
    ASSERT_NEAR( 900., cc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(i8qm3_t::realMax, dc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(i8qm3_t::realMax, ec.real(), i8qm3_t::resolution);
    ASSERT_NEAR(1020., ec_ovf.real(), i8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__larger_unsigned_type_2_smaller_signed_type__expected_real_value) {
    using u32q14_t = u32q14<80000., 160000.>;
    using i16qm3_t = i16qm3<-100000., 100000.>;  // u32 -> i16, max delta f is 17
    auto a = u32q14_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q14_t::fromReal<u32q14_t::realMin>();
    auto c = u32q14_t::fromReal<90000.>();
    auto d = u32q14_t::fromReal<i16qm3_t::realMax>();
    auto e = u32q14_t::fromReal<u32q14_t::realMax>();
    auto ac = static_q_cast<i16qm3_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<i16qm3_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i16qm3_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i16qm3_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i16qm3_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i16qm3_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(   0., ac.real(), i16qm3_t::resolution);
    ASSERT_NEAR(u32q14_t::realMin, bc.real(), i16qm3_t::resolution);
    ASSERT_NEAR( 90000., cc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(i16qm3_t::realMax, dc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(i16qm3_t::realMax, ec.real(), i16qm3_t::resolution);
    ASSERT_NEAR(160000., ec_ovf.real(), i16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__large_signed_type_2_small_unsigned_type__expected_real_value) {
    using i32q20_t = i32q20<-1000., 1000.>;
    using u8qm3_t = u8qm3<100., 900.>;  // i32 -> u8, max delta f is 23
    auto a = i32q20_t::fromReal<i32q20_t::realMin>();
    auto b = i32q20_t::fromReal<500.>();
    auto c = i32q20_t::fromReal<u8qm3_t::realMax>();
    auto d = i32q20_t::fromReal<i32q20_t::realMax>();
    auto ac = static_q_cast<u8qm3_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u8qm3_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<u8qm3_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<u8qm3_t, fpm::Ovf::clamp>(d);
    auto dc_ovf = static_q_cast<u8qm3_t, fpm::Ovf::allowed>(d);

    ASSERT_NEAR(u8qm3_t::realMin, ac.real(), u8qm3_t::resolution);
    ASSERT_NEAR( 500., bc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(u8qm3_t::realMax, cc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(u8qm3_t::realMax, dc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(i32q20_t::realMax, dc_ovf.real(), u8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__larger_signed_type_2_smaller_unsigned_type__expected_real_value) {
    using i32q12_t = i32q12<-1000., 1000.>;
    using u16qm3_t = u8qm3<100., 900.>;  // i32 -> u16, max delta f is 15
    auto a = i32q12_t::fromReal<i32q12_t::realMin>();
    auto b = i32q12_t::fromReal<500.>();
    auto c = i32q12_t::fromReal<u16qm3_t::realMax>();
    auto d = i32q12_t::fromReal<i32q12_t::realMax>();
    auto ac = static_q_cast<u16qm3_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u16qm3_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<u16qm3_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<u16qm3_t, fpm::Ovf::clamp>(d);
    auto dc_ovf = static_q_cast<u16qm3_t, fpm::Ovf::allowed>(d);

    ASSERT_NEAR(u16qm3_t::realMin, ac.real(), u16qm3_t::resolution);
    ASSERT_NEAR( 500., bc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(u16qm3_t::realMax, cc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(u16qm3_t::realMax, dc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(i32q12_t::realMax, dc_ovf.real(), u16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__large_signed_type_2_small_signed_type__expected_real_value) {
    using i32q20_t = i32q20<-1000., 2020.>;
    using i8qm4_t = i8qm4<-900., 2000.>;  // i32 -> i8, max delta f is 24
    auto a = i32q20_t::fromReal<i32q20_t::realMin>();
    auto b = i32q20_t::fromReal<-500.>();
    auto c = i32q20_t::fromReal<i32q20_t::realMax>();
    auto d = i32q20_t::fromReal<2030., fpm::Ovf::allowed>();
    auto ac = static_q_cast<i8qm4_t, fpm::Ovf::clamp>(a);
    auto ac_ovf = static_q_cast<i8qm4_t, fpm::Ovf::allowed>(a);
    auto bc = static_q_cast<i8qm4_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i8qm4_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i8qm4_t, fpm::Ovf::clamp>(d);
    auto dc_ovf = static_q_cast<i8qm4_t, fpm::Ovf::allowed>(d);

    ASSERT_NEAR(i8qm4_t::realMin, ac.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i32q20_t::realMin, ac_ovf.real(), i8qm4_t::resolution);
    ASSERT_NEAR(-500., bc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i8qm4_t::realMax, cc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i8qm4_t::realMax, dc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(2030., dc_ovf.real(), i8qm4_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__larger_signed_type_2_smaller_signed_type__expected_real_value) {
    using i32q12_t = i32q12<-80000., 500000.>;
    using i16qm4_t = i16qm4<-40000., 400000.>;  // i32 -> i16, max delta f is 16
    auto a = i32q12_t::fromReal<i32q12_t::realMin>();
    auto b = i32q12_t::fromReal<-10000.>();
    auto c = i32q12_t::fromReal<i32q12_t::realMax>();
    auto d = i32q12_t::fromReal<410000., fpm::Ovf::allowed>();
    auto e = i32q12_t::fromReal<520000., fpm::Ovf::allowed>();
    auto ac = static_q_cast<i16qm4_t, fpm::Ovf::clamp>(a);
    auto ac_ovf = static_q_cast<i16qm4_t, fpm::Ovf::allowed>(a);
    auto bc = static_q_cast<i16qm4_t, fpm::Ovf::clamp>(b);
    auto cc = static_q_cast<i16qm4_t, fpm::Ovf::clamp>(c);
    auto dc = static_q_cast<i16qm4_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<i16qm4_t, fpm::Ovf::clamp>(e);
    auto ec_ovf = static_q_cast<i16qm4_t, fpm::Ovf::allowed>(e);

    ASSERT_NEAR(i16qm4_t::realMin, ac.real(), i16qm4_t::resolution);
    ASSERT_NEAR(-80000., ac_ovf.real(), i16qm4_t::resolution);
    ASSERT_NEAR(-10000., bc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, cc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, dc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, ec.real(), i16qm4_t::resolution);
    ASSERT_NEAR(520000., ec_ovf.real(), i16qm4_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__large_unsigned_type_2_small_unsigned_type__expected_real_value) {
    using u32q22_t = u32q22<800., 1010.>;
    using u8qm2_t = u8qm2<100., 1000.>;  // u32 -> u8, max delta f is 24
    auto a = u32q22_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q22_t::fromReal<u32q22_t::realMin>();
    auto d = u32q22_t::fromReal<900.>();
    auto e = u32q22_t::fromReal<u32q22_t::realMax>();
    auto f = u32q22_t::fromReal<1023., fpm::Ovf::allowed>();
    auto ac = static_q_cast<u8qm2_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u8qm2_t, fpm::Ovf::clamp>(b);
    auto dc = static_q_cast<u8qm2_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<u8qm2_t, fpm::Ovf::clamp>(e);
    auto fc = static_q_cast<u8qm2_t, fpm::Ovf::clamp>(f);
    auto fc_ovf = static_q_cast<u8qm2_t, fpm::Ovf::allowed>(f);

    ASSERT_NEAR(u8qm2_t::realMin, ac.real(), u8qm2_t::resolution);
    ASSERT_NEAR( 800., bc.real(), u8qm2_t::resolution);
    ASSERT_NEAR( 900., dc.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u8qm2_t::realMax, ec.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u8qm2_t::realMax, fc.real(), u8qm2_t::resolution);
    ASSERT_NEAR(1023., fc_ovf.real(), u8qm2_t::resolution);
}

TEST_F(QTest_Casting, q_static_cast__larger_unsigned_type_2_smaller_unsigned_type__expected_real_value) {
    using u32q14_t = u32q14<80000., 160000.>;
    using u16qm2_t = u16qm2<10000., 100000.>;  // u32 -> u16, max delta f is 16
    auto a = u32q14_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q14_t::fromReal<u32q14_t::realMin>();
    auto d = u32q14_t::fromReal<90000.>();
    auto e = u32q14_t::fromReal<u32q14_t::realMax>();
    auto f = u32q14_t::fromReal<210000., fpm::Ovf::allowed>();
    auto ac = static_q_cast<u16qm2_t, fpm::Ovf::clamp>(a);
    auto bc = static_q_cast<u16qm2_t, fpm::Ovf::clamp>(b);
    auto dc = static_q_cast<u16qm2_t, fpm::Ovf::clamp>(d);
    auto ec = static_q_cast<u16qm2_t, fpm::Ovf::clamp>(e);
    auto fc = static_q_cast<u16qm2_t, fpm::Ovf::clamp>(f);
    auto fc_ovf = static_q_cast<u16qm2_t, fpm::Ovf::allowed>(f);

    ASSERT_NEAR(u16qm2_t::realMin, ac.real(), u16qm2_t::resolution);
    ASSERT_NEAR( 80000., bc.real(), u16qm2_t::resolution);
    ASSERT_NEAR( 90000., dc.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u16qm2_t::realMax, ec.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u16qm2_t::realMax, fc.real(), u16qm2_t::resolution);
    ASSERT_NEAR(210000., fc_ovf.real(), u16qm2_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed__unsigned_type_smallerF_same_value) {
    constexpr double realA = 1024.2;
    auto a = i32q4_t::fromReal<realA>();
    auto b = safe_q_cast<u32qm2_clamp_t>(a);  // this would not compile if u32qm2 was used
    auto c = safe_q_cast<u32qm2_t, fpm::Ovf::clamp>(a);

    ASSERT_NEAR(realA, b.real(), i32q4_t::resolution + u32qm2_clamp_t::resolution);
    ASSERT_NEAR(realA, c.real(), i32q4_t::resolution + u32qm2_t::resolution);
    ASSERT_TRUE((std::is_same_v<u32qm2_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u32qm2_t, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr double realA = 498.7;
    auto a = u16q6_t::fromReal<realA>();
    auto b = safe_q_cast<i32q20_clamp_t>(a);  // no clamping needed
    auto c = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(a);  // no clamping needed

    // note: for up-scaling to a larger integral type, the resulting resolution is the resolution
    //       of the source type (because the base type of both target and source is integral)
    ASSERT_NEAR(realA, b.real(), u16q6_t::resolution);
    ASSERT_NEAR(realA, c.real(), u16q6_t::resolution);
    ASSERT_TRUE((std::is_same_v<i32q20_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32q20_t, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr double realA = 498.7;
    auto a = u16q6_t::fromReal<realA>();
    auto b = safe_q_cast<i32qm2_clamp_t>(a);  // no clamping needed
    auto c = safe_q_cast<i32qm2_t, fpm::Ovf::clamp>(a);  // no clamping needed

    ASSERT_NEAR(realA, b.real(), u16q6_t::resolution + i32qm2_clamp_t::resolution);
    ASSERT_NEAR(realA, c.real(), u16q6_t::resolution + i32qm2_t::resolution);
    ASSERT_TRUE((std::is_same_v<i32qm2_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32qm2_t, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr double realA = 498.7;
    auto a = i32q4_t::fromReal<realA>();
    auto b = safe_q_cast<u16q6_clamp_t>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6_t, fpm::Ovf::clamp>(a);

    ASSERT_NEAR(realA, b.real(), u16q6_clamp_t::resolution);
    ASSERT_NEAR(realA, c.real(), u16q6_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed_too_large__smaller_unsigned_type_largerF_value_clamped) {
    constexpr double realA = 512.5;
    auto a = i32q4_t::fromReal<realA>();
    auto b = safe_q_cast<u16q6_clamp_t>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6_t, fpm::Ovf::clamp>(a);

    ASSERT_NEAR(u16q6_clamp_t::realMax, b.real(), u16q6_clamp_t::resolution);
    ASSERT_NEAR(u16q6_t::realMax, c.real(), u16q6_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__negative_real_value__smaller_unsigned_type_largerF_clamped_value) {
    constexpr double realA = -498.7;
    auto a = i32q4_t::fromReal<realA>();
    auto b = safe_q_cast<u16q6_clamp_t>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6_t, fpm::Ovf::clamp>(a);

    ASSERT_NEAR(u16q6_clamp_t::realMin, b.real(), u16q6_clamp_t::resolution);
    ASSERT_NEAR(u16q6_t::realMin, c.real(), u16q6_t::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__small_signed_type_2_large_unsigned_type__expected_real_value) {
    using i8qm3_t = i8qm3<-1000., 1000.>;  // i8 -> u32, max delta f is 25
    using u32q22_t = u32q22<800., 1020.>;
    auto a = i8qm3_t::fromReal<-1010., fpm::Ovf::allowed>();
    auto b = i8qm3_t::fromReal<i8qm3_t::realMin>();
    auto c = i8qm3_t::fromReal<500.>();
    auto d = i8qm3_t::fromReal<900.>();
    auto e = i8qm3_t::fromReal<i8qm3_t::realMax>();
    auto f = i8qm3_t::fromReal<1010., fpm::Ovf::allowed>();
    auto g = i8qm3_t::fromReal<1022., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(e);
    auto fc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(f);
    auto gc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(g);

    ASSERT_NEAR(u32q22_t::realMin, ac.real(), i8qm3_t::resolution);
    ASSERT_NEAR(u32q22_t::realMin, bc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(u32q22_t::realMin, cc.real(), i8qm3_t::resolution);
    ASSERT_NEAR( 900., dc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(i8qm3_t::realMax, ec.real(), i8qm3_t::resolution);
    ASSERT_NEAR(1010., fc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(u32q22_t::realMax, gc.real(), i8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__smaller_signed_type_2_larger_unsigned_type__expected_real_value) {
    using i16qm3_t = i16qm3<-100000., 100000.>;  // i16 -> u32, max delta f is 17
    using u32q14_t = u32q14<80000., 160000.>;
    auto a = i16qm3_t::fromReal<-110000., fpm::Ovf::allowed>();
    auto b = i16qm3_t::fromReal<i16qm3_t::realMin>();
    auto c = i16qm3_t::fromReal<50000.>();
    auto d = i16qm3_t::fromReal<90000.>();
    auto e = i16qm3_t::fromReal<i16qm3_t::realMax>();
    auto f = i16qm3_t::fromReal<110000., fpm::Ovf::allowed>();
    auto g = i16qm3_t::fromReal<200000., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(e);
    auto fc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(f);
    auto gc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(g);

    ASSERT_NEAR(u32q14_t::realMin, ac.real(), i16qm3_t::resolution);
    ASSERT_NEAR(u32q14_t::realMin, bc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(u32q14_t::realMin, cc.real(), i16qm3_t::resolution);
    ASSERT_NEAR( 90000., dc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(i16qm3_t::realMax, ec.real(), i16qm3_t::resolution);
    ASSERT_NEAR(110000., fc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(u32q14_t::realMax, gc.real(), i16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__small_unsigned_type_2_large_signed_type__expected_real_value) {
    using u8qm3_t = u8qm3<0., 1000.>;  // u8 -> i32, max delta f is 23
    using i32q20_t = i32q20<-800., 1020.>;
    auto a = u8qm3_t::fromReal<u8qm3_t::realMin>();
    auto b = u8qm3_t::fromReal<500.>();
    auto c = u8qm3_t::fromReal<u8qm3_t::realMax>();
    auto d = u8qm3_t::fromReal<1010., fpm::Ovf::allowed>();
    auto e = u8qm3_t::fromReal<1022., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(u8qm3_t::realMin, ac.real(), u8qm3_t::resolution);
    ASSERT_NEAR( 500., bc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(u8qm3_t::realMax, cc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(1010., dc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(i32q20_t::realMax, ec.real(), u8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__smaller_unsigned_type_2_larger_signed_type__expected_real_value) {
    using u16qm3_t = u16qm3<0., 400000.>;  // u16 -> i32, max delta f is 15
    using i32q12_t = i32q12<-80000., 500000.>;
    auto a = u16qm3_t::fromReal<u16qm3_t::realMin>();
    auto b = u16qm3_t::fromReal<50000.>();
    auto c = u16qm3_t::fromReal<u16qm3_t::realMax>();
    auto d = u16qm3_t::fromReal<410000., fpm::Ovf::allowed>();
    auto e = u16qm3_t::fromReal<520000., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(u16qm3_t::realMin, ac.real(), u16qm3_t::resolution);
    ASSERT_NEAR( 50000., bc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(u16qm3_t::realMax, cc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(410000., dc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(i32q12_t::realMax, ec.real(), u16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__small_signed_type_2_large_signed_type__expected_real_value) {
    using i8qm4_t = i8qm4<-2000., 2000.>;  // i8 -> i32, max delta f is 24
    using i32q20_t = i32q20<-1000., 2020.>;
    auto a = i8qm4_t::fromReal<i8qm4_t::realMin>();
    auto b = i8qm4_t::fromReal<-500.>();
    auto c = i8qm4_t::fromReal<i8qm4_t::realMax>();
    auto d = i8qm4_t::fromReal<2010., fpm::Ovf::allowed>();
    auto e = i8qm4_t::fromReal<2030., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i32q20_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(i32q20_t::realMin, ac.real(), i8qm4_t::resolution);
    ASSERT_NEAR(-500., bc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i8qm4_t::realMax, cc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(2010., dc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i32q20_t::realMax, ec.real(), i8qm4_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__smaller_signed_type_2_larger_signed_type__expected_real_value) {
    using i16qm4_t = i16qm4<-400000., 400000.>;  // i16 -> i32, max delta f is 16
    using i32q12_t = i32q12<-80000., 500000.>;
    auto a = i16qm4_t::fromReal<i16qm4_t::realMin>();
    auto b = i16qm4_t::fromReal<-50000.>();
    auto c = i16qm4_t::fromReal<i16qm4_t::realMax>();
    auto d = i16qm4_t::fromReal<410000., fpm::Ovf::allowed>();
    auto e = i16qm4_t::fromReal<520000., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i32q12_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(i32q12_t::realMin, ac.real(), i16qm4_t::resolution);
    ASSERT_NEAR(-50000., bc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, cc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(410000., dc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i32q12_t::realMax, ec.real(), i16qm4_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__small_unsigned_type_2_large_unsigned_type__expected_real_value) {
    using u8qm2_t = u8qm2<100., 1000.>;  // u8 -> u32, max delta f is 24
    using u32q22_t = u32q22<800., 1020.>;
    auto a = u8qm2_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u8qm2_t::fromReal<u8qm2_t::realMin>();
    auto c = u8qm2_t::fromReal<500.>();
    auto d = u8qm2_t::fromReal<900.>();
    auto e = u8qm2_t::fromReal<u8qm2_t::realMax>();
    auto f = u8qm2_t::fromReal<1023., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(e);
    auto fc = safe_q_cast<u32q22_t, fpm::Ovf::clamp>(f);

    ASSERT_NEAR(u32q22_t::realMin, ac.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u32q22_t::realMin, bc.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u32q22_t::realMin, cc.real(), u8qm2_t::resolution);
    ASSERT_NEAR( 900., dc.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u8qm2_t::realMax, ec.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u32q22_t::realMax, fc.real(), u8qm2_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__smaller_unsigned_type_2_larger_unsigned_type__expected_real_value) {
    using u16qm2_t = u16qm2<10000., 100000.>;  // u16 -> u32, max delta f is 16
    using u32q14_t = u32q14<80000., 160000.>;
    auto a = u16qm2_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u16qm2_t::fromReal<u16qm2_t::realMin>();
    auto c = u16qm2_t::fromReal<50000.>();
    auto d = u16qm2_t::fromReal<90000.>();
    auto e = u16qm2_t::fromReal<u16qm2_t::realMax>();
    auto f = u16qm2_t::fromReal<210000., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(e);
    auto fc = safe_q_cast<u32q14_t, fpm::Ovf::clamp>(f);

    ASSERT_NEAR(u32q14_t::realMin, ac.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u32q14_t::realMin, bc.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u32q14_t::realMin, cc.real(), u16qm2_t::resolution);
    ASSERT_NEAR( 90000., dc.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u16qm2_t::realMax, ec.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u32q14_t::realMax, fc.real(), u16qm2_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__large_unsigned_type_2_small_signed_type__expected_real_value) {
    using u32q22_t = u32q22<800., 1020.>;
    using i8qm3_t = i8qm3<-1000., 1000.>;  // u32 -> i8, max delta f is 25
    auto a = u32q22_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q22_t::fromReal<u32q22_t::realMin>();
    auto c = u32q22_t::fromReal<900.>();
    auto d = u32q22_t::fromReal<i8qm3_t::realMax>();
    auto e = u32q22_t::fromReal<u32q22_t::realMax>();
    auto ac = safe_q_cast<i8qm3_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i8qm3_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i8qm3_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i8qm3_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i8qm3_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(   0., ac.real(), i8qm3_t::resolution);
    ASSERT_NEAR(u32q22_t::realMin, bc.real(), i8qm3_t::resolution);
    ASSERT_NEAR( 900., cc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(i8qm3_t::realMax, dc.real(), i8qm3_t::resolution);
    ASSERT_NEAR(i8qm3_t::realMax, ec.real(), i8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__larger_unsigned_type_2_smaller_signed_type__expected_real_value) {
    using u32q14_t = u32q14<80000., 160000.>;
    using i16qm3_t = i16qm3<-100000., 100000.>;  // u32 -> i16, max delta f is 17
    auto a = u32q14_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q14_t::fromReal<u32q14_t::realMin>();
    auto c = u32q14_t::fromReal<90000.>();
    auto d = u32q14_t::fromReal<i16qm3_t::realMax>();
    auto e = u32q14_t::fromReal<u32q14_t::realMax>();
    auto ac = safe_q_cast<i16qm3_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i16qm3_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i16qm3_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i16qm3_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i16qm3_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(   0., ac.real(), i16qm3_t::resolution);
    ASSERT_NEAR(u32q14_t::realMin, bc.real(), i16qm3_t::resolution);
    ASSERT_NEAR( 90000., cc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(i16qm3_t::realMax, dc.real(), i16qm3_t::resolution);
    ASSERT_NEAR(i16qm3_t::realMax, ec.real(), i16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__large_signed_type_2_small_unsigned_type__expected_real_value) {
    using i32q20_t = i32q20<-1000., 1000.>;
    using u8qm3_t = u8qm3<100., 900.>;  // i32 -> u8, max delta f is 23
    auto a = i32q20_t::fromReal<i32q20_t::realMin>();
    auto b = i32q20_t::fromReal<500.>();
    auto c = i32q20_t::fromReal<u8qm3_t::realMax>();
    auto d = i32q20_t::fromReal<i32q20_t::realMax>();
    auto ac = safe_q_cast<u8qm3_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u8qm3_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<u8qm3_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<u8qm3_t, fpm::Ovf::clamp>(d);

    ASSERT_NEAR(u8qm3_t::realMin, ac.real(), u8qm3_t::resolution);
    ASSERT_NEAR( 500., bc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(u8qm3_t::realMax, cc.real(), u8qm3_t::resolution);
    ASSERT_NEAR(u8qm3_t::realMax, dc.real(), u8qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__larger_signed_type_2_smaller_unsigned_type__expected_real_value) {
    using i32q12_t = i32q12<-1000., 1000.>;
    using u16qm3_t = u8qm3<100., 900.>;  // i32 -> u16, max delta f is 15
    auto a = i32q12_t::fromReal<i32q12_t::realMin>();
    auto b = i32q12_t::fromReal<500.>();
    auto c = i32q12_t::fromReal<u16qm3_t::realMax>();
    auto d = i32q12_t::fromReal<i32q12_t::realMax>();
    auto ac = safe_q_cast<u16qm3_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u16qm3_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<u16qm3_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<u16qm3_t, fpm::Ovf::clamp>(d);

    ASSERT_NEAR(u16qm3_t::realMin, ac.real(), u16qm3_t::resolution);
    ASSERT_NEAR( 500., bc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(u16qm3_t::realMax, cc.real(), u16qm3_t::resolution);
    ASSERT_NEAR(u16qm3_t::realMax, dc.real(), u16qm3_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__large_signed_type_2_small_signed_type__expected_real_value) {
    using i32q20_t = i32q20<-1000., 2020.>;
    using i8qm4_t = i8qm4<-900., 2000.>;  // i32 -> i8, max delta f is 24
    auto a = i32q20_t::fromReal<i32q20_t::realMin>();
    auto b = i32q20_t::fromReal<-500.>();
    auto c = i32q20_t::fromReal<i32q20_t::realMax>();
    auto d = i32q20_t::fromReal<2030., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<i8qm4_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i8qm4_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i8qm4_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i8qm4_t, fpm::Ovf::clamp>(d);

    ASSERT_NEAR(i8qm4_t::realMin, ac.real(), i8qm4_t::resolution);
    ASSERT_NEAR(-500., bc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i8qm4_t::realMax, cc.real(), i8qm4_t::resolution);
    ASSERT_NEAR(i8qm4_t::realMax, dc.real(), i8qm4_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__larger_signed_type_2_smaller_signed_type__expected_real_value) {
    using i32q12_t = i32q12<-80000., 500000.>;
    using i16qm4_t = i16qm4<-40000., 400000.>;  // i32 -> i16, max delta f is 16
    auto a = i32q12_t::fromReal<i32q12_t::realMin>();
    auto b = i32q12_t::fromReal<-10000.>();
    auto c = i32q12_t::fromReal<i32q12_t::realMax>();
    auto d = i32q12_t::fromReal<410000., fpm::Ovf::allowed>();
    auto e = i32q12_t::fromReal<520000., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<i16qm4_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<i16qm4_t, fpm::Ovf::clamp>(b);
    auto cc = safe_q_cast<i16qm4_t, fpm::Ovf::clamp>(c);
    auto dc = safe_q_cast<i16qm4_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<i16qm4_t, fpm::Ovf::clamp>(e);

    ASSERT_NEAR(i16qm4_t::realMin, ac.real(), i16qm4_t::resolution);
    ASSERT_NEAR(-10000., bc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, cc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, dc.real(), i16qm4_t::resolution);
    ASSERT_NEAR(i16qm4_t::realMax, ec.real(), i16qm4_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__large_unsigned_type_2_small_unsigned_type__expected_real_value) {
    using u32q22_t = u32q22<800., 1010.>;
    using u8qm2_t = u8qm2<100., 1000.>;  // u32 -> u8, max delta f is 24
    auto a = u32q22_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q22_t::fromReal<u32q22_t::realMin>();
    auto d = u32q22_t::fromReal<900.>();
    auto e = u32q22_t::fromReal<u32q22_t::realMax>();
    auto f = u32q22_t::fromReal<1023., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<u8qm2_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u8qm2_t, fpm::Ovf::clamp>(b);
    auto dc = safe_q_cast<u8qm2_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<u8qm2_t, fpm::Ovf::clamp>(e);
    auto fc = safe_q_cast<u8qm2_t, fpm::Ovf::clamp>(f);

    ASSERT_NEAR(u8qm2_t::realMin, ac.real(), u8qm2_t::resolution);
    ASSERT_NEAR( 800., bc.real(), u8qm2_t::resolution);
    ASSERT_NEAR( 900., dc.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u8qm2_t::realMax, ec.real(), u8qm2_t::resolution);
    ASSERT_NEAR(u8qm2_t::realMax, fc.real(), u8qm2_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__larger_unsigned_type_2_smaller_unsigned_type__expected_real_value) {
    using u32q14_t = u32q14<80000., 160000.>;
    using u16qm2_t = u16qm2<10000., 100000.>;  // u32 -> u16, max delta f is 16
    auto a = u32q14_t::fromReal<0., fpm::Ovf::allowed>();
    auto b = u32q14_t::fromReal<u32q14_t::realMin>();
    auto d = u32q14_t::fromReal<90000.>();
    auto e = u32q14_t::fromReal<u32q14_t::realMax>();
    auto f = u32q14_t::fromReal<210000., fpm::Ovf::allowed>();
    auto ac = safe_q_cast<u16qm2_t, fpm::Ovf::clamp>(a);
    auto bc = safe_q_cast<u16qm2_t, fpm::Ovf::clamp>(b);
    auto dc = safe_q_cast<u16qm2_t, fpm::Ovf::clamp>(d);
    auto ec = safe_q_cast<u16qm2_t, fpm::Ovf::clamp>(e);
    auto fc = safe_q_cast<u16qm2_t, fpm::Ovf::clamp>(f);

    ASSERT_NEAR(u16qm2_t::realMin, ac.real(), u16qm2_t::resolution);
    ASSERT_NEAR( 80000., bc.real(), u16qm2_t::resolution);
    ASSERT_NEAR( 90000., dc.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u16qm2_t::realMax, ec.real(), u16qm2_t::resolution);
    ASSERT_NEAR(u16qm2_t::realMax, fc.real(), u16qm2_t::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__various_invalid_casts__cast_does_not_work) {
    ASSERT_FALSE(( SafeCastable< u32q14<>, u16qm2<>, fpm::Ovf::error > ));
    ASSERT_FALSE(( SafeCastable< u32q14<>, u16qm2<>, fpm::Ovf::unchecked > ));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_signed__unsigned_type_smallerF_same_value) {
    constexpr int32_t scaledA = 1024;
    auto a = i32q4_t::fromScaled<scaledA>();
    auto b = force_q_cast<u32qm2_t>(a);

    ASSERT_EQ(static_cast<u32qm2_t::base_t>(scaledA), b.scaled());
    ASSERT_TRUE((std::is_same_v<u32qm2_t, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr uint16_t scaledA = 498u;
    auto a = u16q6_t::fromScaled<scaledA>();
    auto b = force_q_cast<i32q20_t>(a);

    ASSERT_EQ(static_cast<i32q20_t::base_t>(scaledA), b.scaled());
    ASSERT_TRUE((std::is_same_v<i32q20_t, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr uint16_t scaledA = 498u;
    auto a = u16q6_t::fromScaled<scaledA>();
    auto b = force_q_cast<i32qm2_t>(a);

    ASSERT_EQ(static_cast<i32qm2_t::base_t>(scaledA), b.scaled());
    ASSERT_TRUE((std::is_same_v<i32qm2_t, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr int32_t scaledA = 498;
    auto a = i32q4_t::fromScaled<scaledA>();
    auto b = force_q_cast<u16q6_t>(a);

    ASSERT_EQ(static_cast<u16q6_t::base_t>(scaledA), b.scaled());  // expect: 498u
    ASSERT_TRUE((std::is_same_v<u16q6_t, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__negative_scaled_value__smaller_unsigned_type_largerF_same_value_clamp_ignored) {
    constexpr int32_t scaledA = -498;
    auto a = i32q4_t::fromScaled<scaledA>();
    auto b = force_q_cast<u16q6_clamp_t>(a);

    ASSERT_EQ(static_cast<u16q6_clamp_t::base_t>(scaledA), b.scaled());  // expect: 65038u
    ASSERT_TRUE((std::is_same_v<u16q6_clamp_t, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__negative_scaled_value__smaller_unsigned_type_largerF_same_value_overflow_ignored) {
    constexpr int32_t scaledA = -498;
    auto a = i32q4_t::fromScaled<scaledA>();
    auto b = force_q_cast<u16q6_ovf_t>(a);

    ASSERT_EQ(static_cast<u16q6_t::base_t>(scaledA), b.scaled());  // expect: 65038u
    ASSERT_TRUE((std::is_same_v<u16q6_ovf_t, decltype(b)>));
}

// EOF
