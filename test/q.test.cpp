/* \file
 * Tests for q.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm.hpp>
using namespace fpm;


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Construction ------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Construct : public ::testing::Test {
protected:
    using i16q6 = q<int16_t, 6, -500., 500.>;
    using i16q6_sat = q<int16_t, 6, -500., 500., Ovf::saturate>;
    using i16q6_ovf = q<int16_t, 6, -500., 500., Ovf::allowed>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Construct, q_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double realValue = -495.1;
    auto qValue = i16q6::fromReal<realValue>;  // this does not perform overflow checks
    auto qValue2 = i16q6_sat::fromRealNOvf<realValue>;  // overflow check is forbidden

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.toReal<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(qValue.toReal())>));

    constexpr int16_t resultMemValue = -31686;
    ASSERT_EQ(resultMemValue, qValue.reveal());
    ASSERT_EQ(-495, qValue.toReal<int>());  // test this once to make sure it works as expected
    ASSERT_NEAR(realValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValue2.reveal());
    ASSERT_NEAR(realValue, qValue2.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr double realValue = -511.;
    auto qValue = i16q6::fromReal<realValue, Ovf::saturate>;
    auto qValueSat1 = i16q6::fromRealSat<realValue>;
    auto qValueSat2 = i16q6_sat::fromReal<realValue>;

    ASSERT_EQ(i16q6::vMin, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMin, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6::vMin, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::realVMin, qValueSat1.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMin, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::realVMin, qValueSat2.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr double realValue = +511.;
    auto qValue = i16q6::fromReal<realValue, Ovf::saturate>;
    auto qValueSat1 = i16q6::fromRealSat<realValue>;
    auto qValueSat2 = i16q6_sat::fromReal<realValue>;

    ASSERT_EQ(i16q6::vMax, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMax, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6::vMax, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::realVMax, qValueSat1.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMax, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::realVMax, qValueSat2.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double realValue = -511.;
    auto qValue = i16q6::fromReal<realValue, Ovf::allowed>;
    auto qValueOvf1 = i16q6::fromRealOvf<realValue>;
    auto qValueOvf2 = i16q6_ovf::fromReal<realValue>;

    constexpr int16_t resultMemValue = -32704;
    ASSERT_EQ(resultMemValue, qValue.reveal());
    ASSERT_NEAR(realValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf1.reveal());
    ASSERT_NEAR(realValue, qValueOvf1.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf2.reveal());
    ASSERT_NEAR(realValue, qValueOvf2.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_from_real__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double realValue = +511.;
    auto qValue = i16q6::fromReal<realValue, Ovf::allowed>;
    auto qValueOvf1 = i16q6::fromRealOvf<realValue>;
    auto qValueOvf2 = i16q6_ovf::fromReal<realValue>;

    constexpr int16_t resultMemValue = +32704;
    ASSERT_EQ(resultMemValue, qValue.reveal());
    ASSERT_NEAR(realValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf1.reveal());
    ASSERT_NEAR(realValue, qValueOvf1.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf2.reveal());
    ASSERT_NEAR(realValue, qValueOvf2.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t memValue = -31686;
    auto qValue = i16q6::fromScaled<memValue>;  // this does not perform overflow checks
    auto qValue2 = i16q6_sat::fromScaledNOvf<memValue>;  // overflow check is forbidden

    constexpr double resultRealValue = -495.1;
    ASSERT_EQ(memValue, qValue.reveal());
    ASSERT_NEAR(resultRealValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValue2.reveal());
    ASSERT_NEAR(resultRealValue, qValue2.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6::fromScaled<memValue, Ovf::saturate>;
    auto qValueSat1 = i16q6::fromScaledSat<memValue>;
    auto qValueSat2 = i16q6_sat::fromScaled<memValue>;

    ASSERT_EQ(i16q6::vMin, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMin, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6::vMin, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::realVMin, qValueSat1.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMin, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::realVMin, qValueSat2.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr int16_t memValue = +32704;
    auto qValue = i16q6::fromScaled<memValue, Ovf::saturate>;
    auto qValueSat1 = i16q6::fromScaledSat<memValue>;
    auto qValueSat2 = i16q6_sat::fromScaled<memValue>;

    ASSERT_EQ(i16q6::vMax, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMax, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6::vMax, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::realVMax, qValueSat1.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMax, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::realVMax, qValueSat2.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6::fromScaled<memValue, Ovf::allowed>;
    auto qValueOvf1 = i16q6::fromScaledOvf<memValue>;
    auto qValueOvf2 = i16q6_ovf::fromScaled<memValue>;

    constexpr double resultRealValue = -511.;
    ASSERT_EQ(memValue, qValue.reveal());
    ASSERT_NEAR(resultRealValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValueOvf1.reveal());
    ASSERT_NEAR(resultRealValue, qValueOvf1.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValueOvf2.reveal());
    ASSERT_NEAR(resultRealValue, qValueOvf2.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = +32704;
    auto qValue = i16q6::fromScaled<memValue, Ovf::allowed>;
    auto qValueOvf1 = i16q6::fromScaledOvf<memValue>;
    auto qValueOvf2 = i16q6_ovf::fromScaled<memValue>;

    constexpr double resultRealValue = +511.;
    ASSERT_EQ(memValue, qValue.reveal());
    ASSERT_NEAR(resultRealValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValueOvf1.reveal());
    ASSERT_NEAR(resultRealValue, qValueOvf1.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValueOvf2.reveal());
    ASSERT_NEAR(resultRealValue, qValueOvf2.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6::construct<Ovf::saturate>(memValue);
    auto qValueSat = i16q6_sat::construct(memValue);

    ASSERT_EQ(i16q6::vMin, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMin, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMin, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::realVMin, qValueSat.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr int16_t memValue = 32704;
    auto qValue = i16q6::construct<Ovf::saturate>(memValue);
    auto qValueSat = i16q6_sat::construct(memValue);

    ASSERT_EQ(i16q6::vMax, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMax, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMax, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::realVMax, qValueSat.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = -32704;
    auto qValue = i16q6::construct<Ovf::allowed>(memValue);
    auto qValueOvf = i16q6_ovf::construct(memValue);

    constexpr double resultRealValue = -511.;
    ASSERT_EQ(memValue, qValue.reveal());
    ASSERT_NEAR(resultRealValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValueOvf.reveal());
    ASSERT_NEAR(resultRealValue, qValueOvf.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_construct__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t memValue = +32704;
    auto qValue = i16q6::construct<Ovf::allowed>(memValue);
    auto qValueOvf = i16q6_ovf::construct(memValue);

    constexpr double resultRealValue = +511.;
    ASSERT_EQ(memValue, qValue.reveal());
    ASSERT_NEAR(resultRealValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(memValue, qValueOvf.reveal());
    ASSERT_NEAR(resultRealValue, qValueOvf.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_negative_int16_positiveF_saturate__expected_value) {
    constexpr double realValue = -511.;
    int16_t volatile value = v2s<int16_t,6>(realValue);
    auto qValue = i16q6::construct<Ovf::saturate>(value);
    auto qValueSat = i16q6_sat::construct(value);

    ASSERT_EQ(i16q6::vMin, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMin, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMin, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::realVMin, qValueSat.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_positive_int16_positiveF_saturate__expected_value) {
    constexpr double realValue = +511.;
    int16_t volatile value = v2s<int16_t,6>(realValue);
    auto qValue = i16q6::construct<Ovf::saturate>(value);
    auto qValueSat = i16q6_sat::construct(value);

    ASSERT_EQ(i16q6::vMax, qValue.reveal());
    ASSERT_NEAR(i16q6::realVMax, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(i16q6_sat::vMax, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::realVMax, qValueSat.toReal(), i16q6_sat::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double realValue = -511.;
    int16_t volatile value = v2s<int16_t,6>(realValue);
    auto qValue = i16q6::construct<Ovf::allowed>(value);
    auto qValueOvf = i16q6_ovf::construct(value);

    constexpr int16_t resultMemValue = -32704;
    ASSERT_EQ(resultMemValue, qValue.reveal());
    ASSERT_NEAR(realValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf.reveal());
    ASSERT_NEAR(realValue, qValueOvf.toReal(), i16q6_ovf::resolution);
}

TEST_F(QTest_Construct, q_construct__variable_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double realValue = +511.;
    int16_t volatile value = v2s<int16_t,6>(realValue);
    auto qValue = i16q6::construct<Ovf::allowed>(value);
    auto qValueOvf = i16q6_ovf::construct(value);

    constexpr int16_t resultMemValue = +32704;
    ASSERT_EQ(resultMemValue, qValue.reveal());
    ASSERT_NEAR(realValue, qValue.toReal(), i16q6::resolution);
    ASSERT_EQ(resultMemValue, qValueOvf.reveal());
    ASSERT_NEAR(realValue, qValueOvf.toReal(), i16q6_ovf::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Related Sq -------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Sq : public ::testing::Test {
protected:
    using i32q20 = q<int32_t, 20, -500., 500.>;
    using i32q20_sat = q<int32_t, 20, -500., 500., Ovf::saturate>;
    using i32q20_ovf = q<int32_t, 20, -500., 500., Ovf::allowed>;
    using i32q20_l = q<int32_t, 20, -1000., 1000.>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Sq, q_embedded_sq__from_real__sq_value_is_correct) {
    constexpr double realSqValue = +356.;
    auto sqValue = i32q20::sq<>::fromReal<realSqValue>;

    ASSERT_NEAR(realSqValue, sqValue.toReal(), i32q20::sq<>::resolution);
}

TEST_F(QTest_Sq, q_embedded_sq__smaller_value_range_from_real__sq_value_is_correct) {
    constexpr double realSqValue = -400.;
    auto sqValue = i32q20::sq<-400., 400.>::fromReal<realSqValue>;

    ASSERT_NEAR(realSqValue, sqValue.toReal(), i32q20::resolution);
}

TEST_F(QTest_Sq, q_to_sq__same_value_range__no_overflow_check_performed) {
    constexpr double realValue = -400.;
    auto sqValue = i32q20::fromReal<realValue>.toSq();  // does not include a range check

    ASSERT_NEAR(realValue, sqValue.toReal(), i32q20::resolution);
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_forbidden__does_not_compile) {
    // this must not compile!
    // note: although the value is inside the value range in this case, the compiler will add
    //       overflow checks as soon as the value range for sq is smaller (because the value of q
    //       might be changed before toSq() is called).
    //auto sqValue = i32q20::fromReal<-400.>.toSq<-400., 400., Ovf::forbidden>();
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_asserted__does_not_compile) {
    // this must trigger an assertion trap at runtime
    //auto sqValue = i32q20::fromReal<-410.>.toSq<-400., 400., Ovf::assert>();
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_saturated__value_is_saturated) {
    constexpr double realValue = 450.;
    constexpr double sqRangeN = -400., sqRangeP = +400.;
    auto sqValueN = i32q20::fromReal<-realValue>.toSq<sqRangeN, sqRangeP, Ovf::saturate>();
    auto sqValueP = i32q20::fromReal<+realValue>.toSq<sqRangeN, sqRangeP, Ovf::saturate>();
    auto sqValueNSat = i32q20_sat::fromReal<-realValue>.toSq<sqRangeN, sqRangeP>();
    auto sqValuePSat = i32q20_sat::fromReal<+realValue>.toSq<sqRangeN, sqRangeP>();

    ASSERT_NEAR(sqRangeN, sqValueN.toReal(), i32q20::resolution);
    ASSERT_NEAR(sqRangeP, sqValueP.toReal(), i32q20::resolution);
    ASSERT_NEAR(sqRangeN, sqValueNSat.toReal(), i32q20_sat::resolution);
    ASSERT_NEAR(sqRangeP, sqValuePSat.toReal(), i32q20_sat::resolution);
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_allowed__value_can_overflow) {
    constexpr double realValue = 450.;
    auto sqValueN = i32q20::fromReal<-realValue>.toSq<-400., +400., Ovf::allowed>();
    auto sqValueP = i32q20::fromReal<+realValue>.toSq<-400., +400., Ovf::allowed>();
    auto sqValueNOvf = i32q20_ovf::fromReal<-realValue>.toSq<-400., +400.>();
    auto sqValuePOvf = i32q20_ovf::fromReal<+realValue>.toSq<-400., +400.>();

    ASSERT_NEAR(-realValue, sqValueN.toReal(), i32q20::resolution);
    ASSERT_NEAR(+realValue, sqValueP.toReal(), i32q20::resolution);
    ASSERT_NEAR(-realValue, sqValueNOvf.toReal(), i32q20_ovf::resolution);
    ASSERT_NEAR(+realValue, sqValuePOvf.toReal(), i32q20_ovf::resolution);
}

TEST_F(QTest_Sq, q_from_sq__same_value_range_value_within_limits__q_value_equal_no_overflow_check) {
    constexpr double realValue = -234.5;
    auto sqValue = i32q20::fromReal<realValue>.toSq();
    auto qValue = i32q20::fromSq(sqValue);  // does not perform overflow checks in this case

    ASSERT_NEAR(realValue, qValue.toReal(), i32q20::resolution);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_default__does_not_compile) {
    // this must not compile!
    //auto sqValue = i32sq20<-1010., 1010.>::fromReal<-654.>;
    //auto qValue = i32q20::fromSq(sqValue);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_saturate__q_value_is_saturated) {
    constexpr double realValue = 654.;
    auto sqValueN = i32q20_l::fromReal<-realValue>.toSq();
    auto sqValueP = i32q20_l::fromReal<+realValue>.toSq();
    auto qValueN = i32q20::fromSq<Ovf::saturate>(sqValueN);
    auto qValueP = i32q20::fromSq<Ovf::saturate>(sqValueP);
    auto qValueNSat = i32q20_sat::fromSq(sqValueN);
    auto qValuePSat = i32q20_sat::fromSq(sqValueP);

    ASSERT_NEAR(i32q20::realVMin, qValueN.toReal(), i32q20::resolution);
    ASSERT_NEAR(i32q20::realVMax, qValueP.toReal(), i32q20::resolution);
    ASSERT_NEAR(i32q20_sat::realVMin, qValueNSat.toReal(), i32q20_sat::resolution);
    ASSERT_NEAR(i32q20_sat::realVMax, qValuePSat.toReal(), i32q20_sat::resolution);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_overflow_allowed__q_value_can_overflow) {
    constexpr double realValue = 654.;
    auto sqValueN = i32q20_l::fromReal<-realValue>.toSq();
    auto sqValueP = i32q20_l::fromReal<+realValue>.toSq();
    auto qValueN = i32q20::fromSq<Ovf::allowed>(sqValueN);
    auto qValueP = i32q20::fromSq<Ovf::allowed>(sqValueP);
    auto qValueNOvf = i32q20_ovf::fromSq(sqValueN);
    auto qValuePOvf = i32q20_ovf::fromSq(sqValueP);

    ASSERT_NEAR(-realValue, qValueN.toReal(), i32q20::resolution);
    ASSERT_NEAR(+realValue, qValueP.toReal(), i32q20::resolution);
    ASSERT_NEAR(-realValue, qValueNOvf.toReal(), i32q20_ovf::resolution);
    ASSERT_NEAR(+realValue, qValuePOvf.toReal(), i32q20_ovf::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ---------------------------------- Q Test: Copy & Scale -------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_CopyScale : public ::testing::Test {
protected:
    using i16q4 = q<int16_t, 4, -2048., 2047.9>;
    using i32q4 = q<int32_t, 4, -2048., 2048.>;
    using i32q4_sat = q<int32_t, 4, -2048., 2048., Ovf::saturate>;
    using i32q4_ovf = q<int32_t, 4, -2048., 2048., Ovf::allowed>;
    using i32qm2 = q<int32_t, -2, -2048., 2048.>;
    using i32qm2_sat = q<int32_t, -2, -2048., 2048., Ovf::saturate>;
    using i32qm2_ovf = q<int32_t, -2, -2048., 2048., Ovf::allowed>;
    using i32q8 = q<int32_t, 8, -2048.1, 2048.1>;
    using i32q8_sat = q<int32_t, 8, -2048.1, 2048.1, Ovf::saturate>;
    using i32q8_ovf = q<int32_t, 8, -2048.1, 2048.1, Ovf::allowed>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_CopyScale, q_relimit__some_q_type__relimited_q_type) {
    constexpr double restrictedLimit = 1024.;
    constexpr double extendedLimit = 4096.;
    using restricted_q_t   = i32q4::relimit_t<-restrictedLimit, +restrictedLimit, Ovf::saturate>;
    using restricted_l_q_t = i32q4::relimit_t<-restrictedLimit, i32q4::realVMax,  Ovf::saturate>;
    using restricted_r_q_t = i32q4::relimit_t<i32q4::realVMin,  +restrictedLimit, Ovf::saturate>;
    using extended_q_t     = i32q4::relimit_t<-extendedLimit,   +extendedLimit,   Ovf::saturate>;
    using extended_l_q_t   = i32q4::relimit_t<-extendedLimit,   i32q4::realVMax,  Ovf::saturate>;
    using extended_r_q_t   = i32q4::relimit_t<i32q4::realVMin,  +extendedLimit,   Ovf::saturate>;
    using shifted_q_l_t    = i32q4::relimit_t<-extendedLimit,   +restrictedLimit, Ovf::saturate>;
    using shifted_q_r_t    = i32q4::relimit_t<-restrictedLimit, +extendedLimit,   Ovf::saturate>;

    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -restrictedLimit, +restrictedLimit, Ovf::saturate>, restricted_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -restrictedLimit, i32q4::realVMax,  Ovf::saturate>, restricted_l_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, i32q4::realVMin,  +restrictedLimit, Ovf::saturate>, restricted_r_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -extendedLimit,   +extendedLimit,   Ovf::saturate>, extended_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -extendedLimit,   i32q4::realVMax,  Ovf::saturate>, extended_l_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, i32q4::realVMin,  +extendedLimit,   Ovf::saturate>, extended_r_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -extendedLimit,   +restrictedLimit, Ovf::saturate>, shifted_q_l_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -restrictedLimit, +extendedLimit,   Ovf::saturate>, shifted_q_r_t >));
}

TEST_F(QTest_CopyScale, q_copy_constructor__int16_someF__int16_sameF) {
    constexpr double realValueA = -1024.2;
    auto a = i16q4::fromReal<realValueA>;
    auto b = i16q4(a);  // explicit copy-constructor from same q-type
    auto c = i16q4::fromQ(a);  // copy-construction via named constructor
    i16q4 d = b;  // implicit copy-construction

    ASSERT_NEAR(realValueA, b.toReal(), i16q4::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i16q4::resolution);
    ASSERT_NEAR(realValueA, d.toReal(), i16q4::resolution);
}

TEST_F(QTest_CopyScale, q_move_constructor__int16_someF__int16_sameF) {
    constexpr double realValueA = -1024.2;
    auto a = i16q4::fromReal<realValueA>;
    i16q4 b = std::move(a);

    ASSERT_NEAR(realValueA, b.toReal(), i16q4::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double realValueA = -1024.2;
    auto a = i32q4::fromReal<realValueA>;
    auto b = i32q8::fromQ(a);  // no overflow check needed here

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       q4 type and does not change if the value is up-scaled to a larger f
    ASSERT_NEAR(realValueA, b.toReal(), i32q4::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_saturate__int16_someF__int16_largerF) {
    constexpr double realValueA = -2060.;
    auto a = i32q4_ovf::fromReal<realValueA>;
    auto b = i32q8::fromQ<Ovf::saturate>(a);
    auto c = i32q8_sat::fromQ(a);

    ASSERT_NEAR(i32q8_sat::realVMin, b.toReal(), i32q4_ovf::resolution);
    ASSERT_NEAR(i32q8_sat::realVMin, c.toReal(), i32q4_ovf::resolution);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_overflow_allowed__int16_someF__int16_largerF) {
    constexpr double realValueA = -2060.;
    auto a = i32q4_ovf::fromReal<realValueA>;
    auto b = i32q8::fromQ<Ovf::allowed>(a);
    auto c = i32q8_ovf::fromQ(a);

    ASSERT_NEAR(realValueA, b.toReal(), i32q4_ovf::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32q4_ovf::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double realValueA = -1024.2;
    auto a = i32q4::fromReal<realValueA>;
    auto b = i32qm2::fromQ(a);  // no overflow check needed here

    // note: for down-scaling to a smaller f, the representation error is at most the sum of the two
    //       resolutions before and after the scaling operation
    ASSERT_NEAR(realValueA, b.toReal(), i32q4::resolution + i32qm2::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_saturate__int16_someF__int16_smallerF) {
    constexpr double realValueA = -2060.;
    auto a = i32q4_ovf::fromReal<realValueA>;
    auto b = i32qm2::fromQ<Ovf::saturate>(a);
    auto c = i32qm2_sat::fromQ(a);

    ASSERT_NEAR(i32qm2_sat::realVMin, b.toReal(), i32q4_ovf::resolution + i32qm2::resolution);
    ASSERT_NEAR(i32qm2_sat::realVMin, c.toReal(), i32q4_ovf::resolution + i32qm2_sat::resolution);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_overflow_allowed__int16_someF__int16_smallerF) {
    constexpr double realValueA = -2060.;
    auto a = i32q4_ovf::fromReal<realValueA>;
    auto b = i32qm2::fromQ<Ovf::allowed>(a);
    auto c = i32qm2_ovf::fromQ(a);

    ASSERT_NEAR(realValueA, b.toReal(), i32q4_ovf::resolution + i32qm2::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32q4_ovf::resolution + i32qm2_ovf::resolution);
}

TEST_F(QTest_CopyScale, q_assignment__different_q_type__value_is_scaled_and_assigned_only_downscale) {
    constexpr double realValueA = -1024.2;
    auto a = i32q4::fromReal<realValueA>;
    auto b = i32qm2::fromScaled<0>;
    auto c = i32q8::fromScaled<0>;
    b = a;  // downscale-assignment (no runtime checks needed here)
    c = a;  // upscale-assignment (no runtime checks needed here)

    ASSERT_NEAR(realValueA, b.toReal(), i32q4::resolution + i32qm2::resolution);  // downscale decreases resolution
    ASSERT_NEAR(realValueA, c.toReal(), i32q4::resolution);  // upscale does not change initial resolution
}

TEST_F(QTest_CopyScale, q_assignment_saturate__different_q_type__value_is_scaled_and_assigned) {
    constexpr double realValueA = -2060.;
    auto a = i32q4_ovf::fromReal<realValueA>;
    auto b = i32qm2_sat::fromScaled<0>;
    auto c = i32q8_sat::fromScaled<0>;
    b = a;  // assignment from different type
    c = a;

    ASSERT_NEAR(i32qm2_sat::realVMin, b.toReal(), i32q4_ovf::resolution + i32qm2_sat::resolution);
    ASSERT_NEAR(i32q8_sat::realVMin, c.toReal(), i32q4_ovf::resolution);
}

TEST_F(QTest_CopyScale, q_assignment_overflow__different_q_type__value_is_scaled_and_assigned) {
    constexpr double realValueA = -2060.;
    auto a = i32q4_ovf::fromReal<realValueA>;
    auto b = i32qm2_ovf::fromScaled<0>;
    auto c = i32q8_ovf::fromScaled<0>;
    b = a;  // assignment from different type
    c = a;

    ASSERT_NEAR(realValueA, b.toReal(), i32q4_ovf::resolution + i32qm2_ovf::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32q4_ovf::resolution);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Casting ----------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Casting : public ::testing::Test {
protected:
    using i32q20 = q<int32_t, 20, -500., 500.>;
    using i32q20_sat = q<int32_t, 20, -500., 500., Ovf::saturate>;
    using i32q20_ovf = q<int32_t, 20, -500., 500., Ovf::allowed>;

    using i32q4 = q<int32_t, 4, -2048., 2048.>;
    using i32q4_sat = q<int32_t, 4, -2048., 2048., Ovf::saturate>;
    using i32q4_ovf = q<int32_t, 4, -2048., 2048., Ovf::allowed>;
    using i32qm2 = q<int32_t, -2, -2048., 2048.>;
    using i32qm2_sat = q<int32_t, -2, -2048., 2048., Ovf::saturate>;
    using i32qm2_ovf = q<int32_t, -2, -2048., 2048., Ovf::allowed>;

    using u16q6 = q<uint16_t, 6, 0., 500.>;
    using u16q6_sat = q<uint16_t, 6, 0., 500., Ovf::saturate>;
    using u16q6_ovf = q<uint16_t, 6, 0., 500., Ovf::allowed>;
    using u32qm2 = q<uint32_t, -2, 0., 2048.>;
    using u32qm2_sat = q<uint32_t, -2, 0., 2048., Ovf::saturate>;
    using u32qm2_ovf = q<uint32_t, -2, 0., 2048., Ovf::allowed>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed__unsigned_type_smallerF_same_value) {
    constexpr double realValueA = 1024.2;
    auto a = i32q4::fromReal<realValueA>;
    auto b = static_cast<u32qm2_sat>(a);  // this would not compile if u32qm2 was used
    auto c = static_q_cast<u32qm2, Ovf::saturate>(a);
    auto d = static_q_cast<u32qm2_sat>(a);

    ASSERT_NEAR(realValueA, b.toReal(), i32q4::resolution + u32qm2_sat::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32q4::resolution + u32qm2::resolution);
    ASSERT_NEAR(realValueA, d.toReal(), i32q4::resolution + u32qm2_sat::resolution);
    ASSERT_TRUE((std::is_same_v<u32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u32qm2, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u32qm2_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr double realValueA = 498.7;
    auto a = u16q6::fromReal<realValueA>;
    auto b = static_cast<i32q20_sat>(a);  // this would not compile if i32q20 was used
    auto c = static_q_cast<i32q20, Ovf::saturate>(a);
    auto d = static_q_cast<i32q20_sat>(a);

    // note: for up-scaling to a larger integral type, the resulting resolution is the resolution
    //       of the source type (because the base type of both target and source is integral)
    ASSERT_NEAR(realValueA, b.toReal(), u16q6::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), u16q6::resolution);
    ASSERT_NEAR(realValueA, d.toReal(), u16q6::resolution);
    ASSERT_TRUE((std::is_same_v<i32q20_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32q20, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<i32q20_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr double realValueA = 498.7;
    auto a = u16q6::fromReal<realValueA>;
    auto b = static_cast<i32qm2_sat>(a);  // this would not compile if i32qm2 was used
    auto c = static_q_cast<i32qm2, Ovf::saturate>(a);
    auto d = static_q_cast<i32qm2_sat>(a);

    ASSERT_NEAR(realValueA, b.toReal(), u16q6::resolution + i32qm2_sat::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), u16q6::resolution + i32qm2::resolution);
    ASSERT_NEAR(realValueA, d.toReal(), u16q6::resolution + i32qm2_sat::resolution);
    ASSERT_TRUE((std::is_same_v<i32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32qm2, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<i32qm2_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr double realValueA = 498.7;
    auto a = i32q4::fromReal<realValueA>;
    auto b = static_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = static_q_cast<u16q6, Ovf::saturate>(a);
    auto d = static_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(realValueA, b.toReal(), u16q6_sat::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), u16q6::resolution);
    ASSERT_NEAR(realValueA, d.toReal(), u16q6_sat::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed_too_large__smaller_unsigned_type_largerF_value_saturated) {
    constexpr double realValueA = 512.5;
    auto a = i32q4::fromReal<realValueA>;
    auto b = static_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = static_q_cast<u16q6, Ovf::saturate>(a);
    auto d = static_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(u16q6_sat::realVMax, b.toReal(), u16q6_sat::resolution);
    ASSERT_NEAR(u16q6::realVMax, c.toReal(), u16q6::resolution);
    ASSERT_NEAR(u16q6_sat::realVMax, d.toReal(), u16q6_sat::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__negative_real_value__smaller_unsigned_type_largerF_saturated_value) {
    constexpr double realValueA = -498.7;
    auto a = i32q4::fromReal<realValueA>;
    auto b = static_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = static_q_cast<u16q6, Ovf::saturate>(a);
    auto d = static_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(u16q6_sat::realVMin, b.toReal(), u16q6_sat::resolution);
    ASSERT_NEAR(u16q6::realVMin, c.toReal(), u16q6::resolution);
    ASSERT_NEAR(u16q6_sat::realVMin, d.toReal(), u16q6_sat::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__negative_real_value__smaller_unsigned_type_largerF_value_overflow) {
    constexpr double realValueA = -498.7;
    auto a = i32q4::fromReal<realValueA>;
    auto b = static_cast<u16q6_ovf>(a);  // this is possible, however hardly predictable if a is signed
    auto c = static_q_cast<u16q6, Ovf::allowed>(a);
    auto d = static_q_cast<u16q6_ovf>(a);

    constexpr double expectedValue = 525.3125;
    ASSERT_NEAR(expectedValue, b.toReal(), u16q6_ovf::resolution);
    ASSERT_NEAR(expectedValue, c.toReal(), u16q6::resolution);
    ASSERT_NEAR(expectedValue, d.toReal(), u16q6_ovf::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_ovf, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_ovf, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__signed_user_range__unsigned_different_range__expected_real_value) {
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

    using i16qm3 = q<int16_t, -3, -100000., 100000.>;  // i16 -> u32, max delta f is 17
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = i16qm3::fromRealOvf<-110000.>;
    auto b = i16qm3::fromReal<i16qm3::realVMin>;
    auto c = i16qm3::fromReal<50000.>;
    auto d = i16qm3::fromReal<90000.>;
    auto e = i16qm3::fromReal<i16qm3::realVMax>;
    auto f = i16qm3::fromRealOvf<110000.>;
    auto g = i16qm3::fromRealOvf<200000.>;
    auto ac = static_q_cast<u32q14, Ovf::saturate>(a);
    auto bc = static_q_cast<u32q14, Ovf::saturate>(b);
    auto bc_ovf = static_q_cast<u32q14, Ovf::allowed>(b);
    auto cc = static_q_cast<u32q14, Ovf::saturate>(c);
    auto dc = static_q_cast<u32q14, Ovf::saturate>(d);
    auto ec = static_q_cast<u32q14, Ovf::saturate>(e);
    auto fc = static_q_cast<u32q14, Ovf::saturate>(f);
    auto gc = static_q_cast<u32q14, Ovf::saturate>(g);
    auto gc_ovf = static_q_cast<u32q14, Ovf::allowed>(g);

    ASSERT_NEAR(u32q14::realVMin, ac.toReal(), i16qm3::resolution);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, bc.toReal(), i16qm3::resolution);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(162144., bc_ovf.toReal(), i16qm3::resolution);  // (u32::max - 100000*2^14) / 2^14
    ASSERT_NEAR(u32q14::realVMin, cc.toReal(), i16qm3::resolution);  // 50000 saturated to u32q14::realVMin
    ASSERT_NEAR( 90000., dc.toReal(), i16qm3::resolution);
    ASSERT_NEAR(i16qm3::realVMax, ec.toReal(), i16qm3::resolution);
    ASSERT_NEAR(110000., fc.toReal(), i16qm3::resolution);
    ASSERT_NEAR(u32q14::realVMax, gc.toReal(), i16qm3::resolution);
    ASSERT_NEAR(200000., gc_ovf.toReal(), i16qm3::resolution);
}

TEST_F(QTest_Casting, q_static_cast__unsigned_user_range__signed_different_range__expected_real_value) {
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

    using u16qm3 = q<uint16_t, -3, 0., 400000.>;  // u16 -> i32, max delta f is 15
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = u16qm3::fromReal<u16qm3::realVMin>;
    auto b = u16qm3::fromReal<50000.>;
    auto c = u16qm3::fromReal<u16qm3::realVMax>;
    auto d = u16qm3::fromRealOvf<410000.>;
    auto e = u16qm3::fromRealOvf<520000.>;
    auto ac = static_q_cast<i32q12, Ovf::saturate>(a);
    auto bc = static_q_cast<i32q12, Ovf::saturate>(b);
    auto cc = static_q_cast<i32q12, Ovf::saturate>(c);
    auto dc = static_q_cast<i32q12, Ovf::saturate>(d);
    auto ec = static_q_cast<i32q12, Ovf::saturate>(e);
    auto ec_ovf = static_q_cast<i32q12, Ovf::allowed>(e);

    ASSERT_NEAR(u16qm3::realVMin, ac.toReal(), u16qm3::resolution);
    ASSERT_NEAR( 50000., bc.toReal(), u16qm3::resolution);
    ASSERT_NEAR(u16qm3::realVMax, cc.toReal(), u16qm3::resolution);
    ASSERT_NEAR(410000., dc.toReal(), u16qm3::resolution);
    ASSERT_NEAR(i32q12::realVMax, ec.toReal(), u16qm3::resolution);  // 520000 saturated to i32q12::realVMax
    ASSERT_NEAR(520000., ec_ovf.toReal(), u16qm3::resolution);
}

TEST_F(QTest_Casting, q_static_cast__signed_user_range__signed_different_range__expected_real_value) {
    // i16[min,max]:
    // i16::min      min       0       max         i16::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= i16 user value range
    //
    // static_cast< i32[MIN,MAX] >( i16_scaled[min,max] ):
    // i32::min  MIN           0            MAX    i32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled i16 user value range before saturation
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= i32 user value range (saturated value range)

    using i16qm4 = q<int16_t, -4, -400000., 400000.>;  // i16 -> i32, max delta f is 16
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = i16qm4::fromReal<i16qm4::realVMin>;
    auto b = i16qm4::fromReal<-50000.>;
    auto c = i16qm4::fromReal<i16qm4::realVMax>;
    auto d = i16qm4::fromRealOvf<410000.>;
    auto e = i16qm4::fromRealOvf<520000.>;
    auto ac = static_q_cast<i32q12, Ovf::saturate>(a);
    auto ac_ovf = static_q_cast<i32q12, Ovf::allowed>(a);
    auto bc = static_q_cast<i32q12, Ovf::saturate>(b);
    auto cc = static_q_cast<i32q12, Ovf::saturate>(c);
    auto dc = static_q_cast<i32q12, Ovf::saturate>(d);
    auto ec = static_q_cast<i32q12, Ovf::saturate>(e);
    auto ec_ovf = static_q_cast<i32q12, Ovf::allowed>(e);

    ASSERT_NEAR(i32q12::realVMin, ac.toReal(), i16qm4::resolution);  // -400000 saturated to i32q12::realVMin
    ASSERT_NEAR(i16qm4::realVMin, ac_ovf.toReal(), i16qm4::resolution);  // -400000 not saturated
    ASSERT_NEAR(-50000., bc.toReal(), i16qm4::resolution);
    ASSERT_NEAR(i16qm4::realVMax, cc.toReal(), i16qm4::resolution);
    ASSERT_NEAR(410000., dc.toReal(), i16qm4::resolution);
    ASSERT_NEAR(i32q12::realVMax, ec.toReal(), i16qm4::resolution);  // 520000 saturated to i32q12::realVMax
    ASSERT_NEAR(520000., ec_ovf.toReal(), i16qm4::resolution);  // not saturated
}

TEST_F(QTest_Casting, q_static_cast__unsigned_user_range__unsigned_different_range__expected_real_value) {
    // u16[min,max]:
    // 0             min               max         u16::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= u16 user value range
    //
    // static_cast< u32[MIN,MAX] >( u16_scaled[min,max] ):
    // 0         MIN                        MAX    u32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled u16 user value range before saturation
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= u32 user value range (saturated value range)

    using u16qm2 = q<uint16_t, -2, 10000., 100000.>;  // u16 -> u32, max delta f is 16
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = u16qm2::fromRealOvf<0.>;
    auto b = u16qm2::fromReal<u16qm2::realVMin>;
    auto c = u16qm2::fromReal<50000.>;
    auto d = u16qm2::fromReal<90000.>;
    auto e = u16qm2::fromReal<u16qm2::realVMax>;
    auto f = u16qm2::fromRealOvf<210000.>;
    auto ac = static_q_cast<u32q14, Ovf::saturate>(a);
    auto bc = static_q_cast<u32q14, Ovf::saturate>(b);
    auto cc = static_q_cast<u32q14, Ovf::saturate>(c);
    auto dc = static_q_cast<u32q14, Ovf::saturate>(d);
    auto ec = static_q_cast<u32q14, Ovf::saturate>(e);
    auto fc = static_q_cast<u32q14, Ovf::saturate>(f);
    auto fc_ovf = static_q_cast<u32q14, Ovf::allowed>(f);

    ASSERT_NEAR(u32q14::realVMin, ac.toReal(), u16qm2::resolution);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, bc.toReal(), u16qm2::resolution);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, cc.toReal(), u16qm2::resolution);  // 50000 saturated to u32q14::realVMin
    ASSERT_NEAR( 90000., dc.toReal(), u16qm2::resolution);
    ASSERT_NEAR(u16qm2::realVMax, ec.toReal(), u16qm2::resolution);
    ASSERT_NEAR(u32q14::realVMax, fc.toReal(), u16qm2::resolution);  // 210000 saturated to u32q14::realVMax
    ASSERT_NEAR(210000., fc_ovf.toReal(), u16qm2::resolution);  // not saturated
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed__unsigned_type_smallerF_same_value) {
    constexpr double realValueA = 1024.2;
    auto a = i32q4::fromReal<realValueA>;
    auto b = safe_q_cast<u32qm2_sat>(a);  // this would not compile if u32qm2 was used
    auto c = safe_q_cast<u32qm2, Ovf::saturate>(a);

    ASSERT_NEAR(realValueA, b.toReal(), i32q4::resolution + u32qm2_sat::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), i32q4::resolution + u32qm2::resolution);
    ASSERT_TRUE((std::is_same_v<u32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u32qm2, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr double realValueA = 498.7;
    auto a = u16q6::fromReal<realValueA>;
    auto b = safe_q_cast<i32q20_sat>(a);  // this would not compile if i32q20 was used
    auto c = safe_q_cast<i32q20, Ovf::saturate>(a);

    // note: for up-scaling to a larger integral type, the resulting resolution is the resolution
    //       of the source type (because the base type of both target and source is integral)
    ASSERT_NEAR(realValueA, b.toReal(), u16q6::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), u16q6::resolution);
    ASSERT_TRUE((std::is_same_v<i32q20_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32q20, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr double realValueA = 498.7;
    auto a = u16q6::fromReal<realValueA>;
    auto b = safe_q_cast<i32qm2_sat>(a);  // this would not compile if i32qm2 was used
    auto c = safe_q_cast<i32qm2, Ovf::saturate>(a);

    ASSERT_NEAR(realValueA, b.toReal(), u16q6::resolution + i32qm2_sat::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), u16q6::resolution + i32qm2::resolution);
    ASSERT_TRUE((std::is_same_v<i32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32qm2, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr double realValueA = 498.7;
    auto a = i32q4::fromReal<realValueA>;
    auto b = safe_q_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6, Ovf::saturate>(a);

    ASSERT_NEAR(realValueA, b.toReal(), u16q6_sat::resolution);
    ASSERT_NEAR(realValueA, c.toReal(), u16q6::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed_too_large__smaller_unsigned_type_largerF_value_saturated) {
    constexpr double realValueA = 512.5;
    auto a = i32q4::fromReal<realValueA>;
    auto b = safe_q_cast<u16q6, Ovf::saturate>(a);
    auto c = safe_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(u16q6_sat::realVMax, b.toReal(), u16q6_sat::resolution);
    ASSERT_NEAR(u16q6::realVMax, c.toReal(), u16q6::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__negative_real_value__smaller_unsigned_type_largerF_saturated_value) {
    constexpr double realValueA = -498.7;
    auto a = i32q4::fromReal<realValueA>;
    auto b = safe_q_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6, Ovf::saturate>(a);

    ASSERT_NEAR(u16q6_sat::realVMin, b.toReal(), u16q6_sat::resolution);
    ASSERT_NEAR(u16q6::realVMin, c.toReal(), u16q6::resolution);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__signed_user_range__unsigned_different_range__saturated_real_value) {
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

    using i16qm3 = q<int16_t, -3, -100000., 100000.>;  // i16 -> u32, max delta f is 17
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = i16qm3::fromRealOvf<-110000.>;
    auto b = i16qm3::fromReal<i16qm3::realVMin>;
    auto c = i16qm3::fromReal<50000.>;
    auto d = i16qm3::fromReal<90000.>;
    auto e = i16qm3::fromReal<i16qm3::realVMax>;
    auto f = i16qm3::fromRealOvf<110000.>;
    auto ac = safe_q_cast<u32q14, Ovf::saturate>(a);
    auto bc = safe_q_cast<u32q14, Ovf::saturate>(b);
    auto cc = safe_q_cast<u32q14, Ovf::saturate>(c);
    auto dc = safe_q_cast<u32q14, Ovf::saturate>(d);
    auto ec = safe_q_cast<u32q14, Ovf::saturate>(e);
    auto fc = safe_q_cast<u32q14, Ovf::saturate>(f);

    ASSERT_NEAR(u32q14::realVMin, ac.toReal(), i16qm3::resolution);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, bc.toReal(), i16qm3::resolution);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, cc.toReal(), i16qm3::resolution);  // 50000 saturated to u32q14::realVMin
    ASSERT_NEAR( 90000., dc.toReal(), i16qm3::resolution);
    ASSERT_NEAR(i16qm3::realVMax, ec.toReal(), i16qm3::resolution);
    ASSERT_NEAR(110000., fc.toReal(), i16qm3::resolution);
}

TEST_F(QTest_Casting, q_safe_cast__unsigned_user_range__signed_different_range__saturated_real_value) {
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

    using u16qm3 = q<uint16_t, -3, 0., 400000.>;  // u16 -> i32, max delta f is 15
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = u16qm3::fromReal<u16qm3::realVMin>;
    auto b = u16qm3::fromReal<50000.>;
    auto c = u16qm3::fromReal<u16qm3::realVMax>;
    auto d = u16qm3::fromRealOvf<410000.>;
    auto e = u16qm3::fromRealOvf<520000.>;
    auto ac = safe_q_cast<i32q12, Ovf::saturate>(a);
    auto bc = safe_q_cast<i32q12, Ovf::saturate>(b);
    auto cc = safe_q_cast<i32q12, Ovf::saturate>(c);
    auto dc = safe_q_cast<i32q12, Ovf::saturate>(d);
    auto ec = safe_q_cast<i32q12, Ovf::saturate>(e);

    ASSERT_NEAR(u16qm3::realVMin, ac.toReal(), u16qm3::resolution);
    ASSERT_NEAR( 50000., bc.toReal(), u16qm3::resolution);
    ASSERT_NEAR(u16qm3::realVMax, cc.toReal(), u16qm3::resolution);
    ASSERT_NEAR(410000., dc.toReal(), u16qm3::resolution);
    ASSERT_NEAR(i32q12::realVMax, ec.toReal(), u16qm3::resolution);  // 520000 saturated to i32q12::realVMax
}

TEST_F(QTest_Casting, q_safe_cast__signed_user_range__signed_different_range__saturated_real_value) {
    // i16[min,max]:
    // i16::min      min       0       max         i16::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= i16 user value range
    //
    // static_cast< i32[MIN,MAX] >( i16_scaled[min,max] ):
    // i32::min  MIN           0            MAX    i32::max
    // |-----|----|------------|-------------|--|------|
    //      min   |                          | max
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled i16 user value range before saturation
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= i32 user value range (saturated value range)

    using i16qm4 = q<int16_t, -4, -400000., 400000.>;  // i16 -> i32, max delta f is 16
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = i16qm4::fromReal<i16qm4::realVMin>;
    auto b = i16qm4::fromReal<-50000.>;
    auto c = i16qm4::fromReal<i16qm4::realVMax>;
    auto d = i16qm4::fromRealOvf<410000.>;
    auto e = i16qm4::fromRealOvf<520000.>;
    auto ac = safe_q_cast<i32q12, Ovf::saturate>(a);
    auto bc = safe_q_cast<i32q12, Ovf::saturate>(b);
    auto cc = safe_q_cast<i32q12, Ovf::saturate>(c);
    auto dc = safe_q_cast<i32q12, Ovf::saturate>(d);
    auto ec = safe_q_cast<i32q12, Ovf::saturate>(e);

    ASSERT_NEAR(i32q12::realVMin, ac.toReal(), i16qm4::resolution);  // -400000 saturated to i32q12::realVMin
    ASSERT_NEAR(-50000., bc.toReal(), i16qm4::resolution);
    ASSERT_NEAR(i16qm4::realVMax, cc.toReal(), i16qm4::resolution);
    ASSERT_NEAR(410000., dc.toReal(), i16qm4::resolution);
    ASSERT_NEAR(i32q12::realVMax, ec.toReal(), i16qm4::resolution);  // 520000 saturated to i32q12::realVMax
}

TEST_F(QTest_Casting, q_safe_cast__unsigned_user_range__unsigned_different_range__saturated_real_value) {
    // u16[min,max]:
    // 0             min               max         u16::max
    // |--------------|--------|--------|--------------|
    //                ^^^^^^^^^^^^^^^^^^^  <= u16 user value range
    //
    // static_cast< u32[MIN,MAX] >( u16_scaled[min,max] ):
    // 0         MIN                        MAX    u32::max
    // |-----|----|------------|-------------|--|------|
    // |    min   |                          | max     |
    //       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= scaled u16 user value range before saturation
    //            |                          |
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^  <= u32 user value range (saturated value range)

    using u16qm2 = q<uint16_t, -2, 10000., 100000.>;  // u16 -> u32, max delta f is 16
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = u16qm2::fromRealOvf<0.>;
    auto b = u16qm2::fromReal<u16qm2::realVMin>;
    auto c = u16qm2::fromReal<50000.>;
    auto d = u16qm2::fromReal<90000.>;
    auto e = u16qm2::fromReal<u16qm2::realVMax>;
    auto f = u16qm2::fromRealOvf<110000.>;
    auto ac = safe_q_cast<u32q14, Ovf::saturate>(a);
    auto bc = safe_q_cast<u32q14, Ovf::saturate>(b);
    auto cc = safe_q_cast<u32q14, Ovf::saturate>(c);
    auto dc = safe_q_cast<u32q14, Ovf::saturate>(d);
    auto ec = safe_q_cast<u32q14, Ovf::saturate>(e);
    auto fc = safe_q_cast<u32q14, Ovf::saturate>(f);

    ASSERT_NEAR(u32q14::realVMin, ac.toReal(), u16qm2::resolution);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, bc.toReal(), u16qm2::resolution);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::realVMin, cc.toReal(), u16qm2::resolution);  // 50000 saturated to u32q14::realVMin
    ASSERT_NEAR( 90000., dc.toReal(), u16qm2::resolution);
    ASSERT_NEAR(u16qm2::realVMax, ec.toReal(), u16qm2::resolution);
    ASSERT_NEAR(110000., fc.toReal(), u16qm2::resolution);
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_signed__unsigned_type_smallerF_same_value) {
    constexpr int32_t scaledValueA = 1024;
    auto a = i32q4::fromScaled<scaledValueA>;
    auto b = force_q_cast<u32qm2>(a);

    ASSERT_EQ(static_cast<u32qm2::base_t>(scaledValueA), b.reveal());
    ASSERT_TRUE((std::is_same_v<u32qm2, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr uint16_t scaledValueA = 498u;
    auto a = u16q6::fromScaled<scaledValueA>;
    auto b = force_q_cast<i32q20>(a);

    ASSERT_EQ(static_cast<i32q20::base_t>(scaledValueA), b.reveal());
    ASSERT_TRUE((std::is_same_v<i32q20, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr uint16_t scaledValueA = 498u;
    auto a = u16q6::fromScaled<scaledValueA>;
    auto b = force_q_cast<i32qm2>(a);

    ASSERT_EQ(static_cast<i32qm2::base_t>(scaledValueA), b.reveal());
    ASSERT_TRUE((std::is_same_v<i32qm2, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr int32_t scaledValueA = 498;
    auto a = i32q4::fromScaled<scaledValueA>;
    auto b = force_q_cast<u16q6>(a);

    ASSERT_EQ(static_cast<u16q6::base_t>(scaledValueA), b.reveal());  // expect: 498u
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__negative_scaled_value__smaller_unsigned_type_largerF_same_value_saturate_ignored) {
    constexpr int32_t scaledValueA = -498;
    auto a = i32q4::fromScaled<scaledValueA>;
    auto b = force_q_cast<u16q6_sat>(a);

    ASSERT_EQ(static_cast<u16q6_sat::base_t>(scaledValueA), b.reveal());  // expect: 65038u
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__negative_scaled_value__smaller_unsigned_type_largerF_same_value_overflow_ignored) {
    constexpr int32_t scaledValueA = -498;
    auto a = i32q4::fromScaled<scaledValueA>;
    auto b = force_q_cast<u16q6_ovf>(a);

    ASSERT_EQ(static_cast<u16q6::base_t>(scaledValueA), b.reveal());  // expect: 65038u
    ASSERT_TRUE((std::is_same_v<u16q6_ovf, decltype(b)>));
}

// EOF
