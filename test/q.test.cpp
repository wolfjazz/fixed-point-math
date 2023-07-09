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
    using i16q6_sat = q<int16_t, 6, -500., 500., ovf::SATURATE>;
    using i16q6_ovf = q<int16_t, 6, -500., 500., ovf::ALLOWED>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Construct, q_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double REAL_VALUE = -495.1;
    auto qValue = i16q6::from_real<REAL_VALUE>;  // this does not perform overflow checks
    auto qValue2 = i16q6_sat::from_real_novf<REAL_VALUE>;  // overflow check is forbidden

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.to_real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(qValue.to_real())>));

    constexpr int16_t RESULT_MEM_VALUE = -31686;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_EQ(-495, qValue.to_real<int>());  // test this once to make sure it works as expected
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValue2.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue2.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_real__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = -511.;
    auto qValue = i16q6::from_real<REAL_VALUE, ovf::SATURATE>;
    auto qValueSat1 = i16q6::from_real_sat<REAL_VALUE>;
    auto qValueSat2 = i16q6_sat::from_real<REAL_VALUE>;

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6::V_MIN, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValueSat1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat2.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_real__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = +511.;
    auto qValue = i16q6::from_real<REAL_VALUE, ovf::SATURATE>;
    auto qValueSat1 = i16q6::from_real_sat<REAL_VALUE>;
    auto qValueSat2 = i16q6_sat::from_real<REAL_VALUE>;

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6::V_MAX, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValueSat1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat2.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_real__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = -511.;
    auto qValue = i16q6::from_real<REAL_VALUE, ovf::ALLOWED>;
    auto qValueOvf1 = i16q6::from_real_ovf<REAL_VALUE>;
    auto qValueOvf2 = i16q6_ovf::from_real<REAL_VALUE>;

    constexpr int16_t RESULT_MEM_VALUE = -32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf1.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf2.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf2.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_real__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = +511.;
    auto qValue = i16q6::from_real<REAL_VALUE, ovf::ALLOWED>;
    auto qValueOvf1 = i16q6::from_real_ovf<REAL_VALUE>;
    auto qValueOvf2 = i16q6_ovf::from_real<REAL_VALUE>;

    constexpr int16_t RESULT_MEM_VALUE = +32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf1.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf2.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf2.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t MEM_VALUE = -31686;
    auto qValue = i16q6::from_scaled<MEM_VALUE>;  // this does not perform overflow checks
    auto qValue2 = i16q6_sat::from_scaled_novf<MEM_VALUE>;  // overflow check is forbidden

    constexpr double RESULT_REAL_VALUE = -495.1;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValue2.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue2.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, ovf::SATURATE>;
    auto qValueSat1 = i16q6::from_scaled_sat<MEM_VALUE>;
    auto qValueSat2 = i16q6_sat::from_scaled<MEM_VALUE>;

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6::V_MIN, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValueSat1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat2.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = +32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, ovf::SATURATE>;
    auto qValueSat1 = i16q6::from_scaled_sat<MEM_VALUE>;
    auto qValueSat2 = i16q6_sat::from_scaled<MEM_VALUE>;

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6::V_MAX, qValueSat1.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValueSat1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat2.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat2.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, ovf::ALLOWED>;
    auto qValueOvf1 = i16q6::from_scaled_ovf<MEM_VALUE>;
    auto qValueOvf2 = i16q6_ovf::from_scaled<MEM_VALUE>;

    constexpr double RESULT_REAL_VALUE = -511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf1.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf2.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf2.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_from_scaled__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = +32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, ovf::ALLOWED>;
    auto qValueOvf1 = i16q6::from_scaled_ovf<MEM_VALUE>;
    auto qValueOvf2 = i16q6_ovf::from_scaled<MEM_VALUE>;

    constexpr double RESULT_REAL_VALUE = +511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf1.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf1.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf2.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf2.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::construct<ovf::SATURATE>(MEM_VALUE);
    auto qValueSat = i16q6_sat::construct(MEM_VALUE);

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = 32704;
    auto qValue = i16q6::construct<ovf::SATURATE>(MEM_VALUE);
    auto qValueSat = i16q6_sat::construct(MEM_VALUE);

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::construct<ovf::ALLOWED>(MEM_VALUE);
    auto qValueOvf = i16q6_ovf::construct(MEM_VALUE);

    constexpr double RESULT_REAL_VALUE = -511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = +32704;
    auto qValue = i16q6::construct<ovf::ALLOWED>(MEM_VALUE);
    auto qValueOvf = i16q6_ovf::construct(MEM_VALUE);

    constexpr double RESULT_REAL_VALUE = +511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__variable_negative_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = -511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<ovf::SATURATE>(value);
    auto qValueSat = i16q6_sat::construct(value);

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__variable_positive_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = +511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<ovf::SATURATE>(value);
    auto qValueSat = i16q6_sat::construct(value);

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__variable_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = -511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<ovf::ALLOWED>(value);
    auto qValueOvf = i16q6_ovf::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = -32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest_Construct, q_construct__variable_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = +511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<ovf::ALLOWED>(value);
    auto qValueOvf = i16q6_ovf::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = +32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Related Sq -------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Sq : public ::testing::Test {
protected:
    using i32q20 = q<int32_t, 20, -500., 500.>;
    using i32q20_sat = q<int32_t, 20, -500., 500., ovf::SATURATE>;
    using i32q20_ovf = q<int32_t, 20, -500., 500., ovf::ALLOWED>;
    using i32q20_l = q<int32_t, 20, -1000., 1000.>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Sq, q_embedded_sq__from_real__sq_value_is_correct) {
    constexpr double REAL_SQ_VALUE = +356.;
    auto sqValue = i32q20::sq<>::from_real<REAL_SQ_VALUE>;

    ASSERT_NEAR(REAL_SQ_VALUE, sqValue.to_real(), i32q20::sq<>::RESOLUTION);
}

TEST_F(QTest_Sq, q_embedded_sq__smaller_value_range_from_real__sq_value_is_correct) {
    constexpr double REAL_SQ_VALUE = -400.;
    auto sqValue = i32q20::sq<-400., 400.>::from_real<REAL_SQ_VALUE>;

    ASSERT_NEAR(REAL_SQ_VALUE, sqValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest_Sq, q_to_sq__same_value_range__no_overflow_check_performed) {
    constexpr double REAL_VALUE = -400.;
    auto sqValue = i32q20::from_real<REAL_VALUE>.to_sq();  // does not include a range check

    ASSERT_NEAR(REAL_VALUE, sqValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_forbidden__does_not_compile) {
    // this must not compile!
    // note: although the value is inside the value range in this case, the compiler will add
    //       overflow checks as soon as the value range for sq is smaller (because the value of q
    //       might be changed before to_sq() is called).
    //auto sqValue = i32q20::from_real<-400.>.to_sq<-400., 400., ovf::FORBIDDEN>();
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_asserted__does_not_compile) {
    // this must trigger an assertion trap at runtime
    //auto sqValue = i32q20::from_real<-410.>.to_sq<-400., 400., ovf::ASSERT>();
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_saturated__value_is_saturated) {
    constexpr double REAL_VALUE = 450.;
    constexpr double SQ_RANGE_N = -400., SQ_RANGE_P = +400.;
    auto sqValueN = i32q20::from_real<-REAL_VALUE>.to_sq<SQ_RANGE_N, SQ_RANGE_P, ovf::SATURATE>();
    auto sqValueP = i32q20::from_real<+REAL_VALUE>.to_sq<SQ_RANGE_N, SQ_RANGE_P, ovf::SATURATE>();
    auto sqValueNSat = i32q20_sat::from_real<-REAL_VALUE>.to_sq<SQ_RANGE_N, SQ_RANGE_P>();
    auto sqValuePSat = i32q20_sat::from_real<+REAL_VALUE>.to_sq<SQ_RANGE_N, SQ_RANGE_P>();

    ASSERT_NEAR(SQ_RANGE_N, sqValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(SQ_RANGE_P, sqValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(SQ_RANGE_N, sqValueNSat.to_real(), i32q20_sat::RESOLUTION);
    ASSERT_NEAR(SQ_RANGE_P, sqValuePSat.to_real(), i32q20_sat::RESOLUTION);
}

TEST_F(QTest_Sq, q_to_sq__different_value_range_overflow_allowed__value_can_overflow) {
    constexpr double REAL_VALUE = 450.;
    auto sqValueN = i32q20::from_real<-REAL_VALUE>.to_sq<-400., +400., ovf::ALLOWED>();
    auto sqValueP = i32q20::from_real<+REAL_VALUE>.to_sq<-400., +400., ovf::ALLOWED>();
    auto sqValueNOvf = i32q20_ovf::from_real<-REAL_VALUE>.to_sq<-400., +400.>();
    auto sqValuePOvf = i32q20_ovf::from_real<+REAL_VALUE>.to_sq<-400., +400.>();

    ASSERT_NEAR(-REAL_VALUE, sqValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, sqValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(-REAL_VALUE, sqValueNOvf.to_real(), i32q20_ovf::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, sqValuePOvf.to_real(), i32q20_ovf::RESOLUTION);
}

TEST_F(QTest_Sq, q_from_sq__same_value_range_value_within_limits__q_value_equal_no_overflow_check) {
    constexpr double REAL_VALUE = -234.5;
    auto sqValue = i32q20::from_real<REAL_VALUE>.to_sq();
    auto qValue = i32q20::from_sq(sqValue);  // does not perform overflow checks in this case

    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_default__does_not_compile) {
    // this must not compile!
    //auto sqValue = i32sq20_l::from_real<-654.>;
    //auto qValue = i32q20::from_sq(sqValue);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_saturate__q_value_is_saturated) {
    constexpr double REAL_VALUE = 654.;
    auto sqValueN = i32q20_l::from_real<-REAL_VALUE>.to_sq();
    auto sqValueP = i32q20_l::from_real<+REAL_VALUE>.to_sq();
    auto qValueN = i32q20::from_sq<ovf::SATURATE>(sqValueN);
    auto qValueP = i32q20::from_sq<ovf::SATURATE>(sqValueP);
    auto qValueNSat = i32q20_sat::from_sq(sqValueN);
    auto qValuePSat = i32q20_sat::from_sq(sqValueP);

    ASSERT_NEAR(i32q20::REAL_V_MIN, qValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(i32q20::REAL_V_MAX, qValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(i32q20_sat::REAL_V_MIN, qValueNSat.to_real(), i32q20_sat::RESOLUTION);
    ASSERT_NEAR(i32q20_sat::REAL_V_MAX, qValuePSat.to_real(), i32q20_sat::RESOLUTION);
}

TEST_F(QTest_Sq, q_from_sq__different_value_range_overflow_allowed__q_value_can_overflow) {
    constexpr double REAL_VALUE = 654.;
    auto sqValueN = i32q20_l::from_real<-REAL_VALUE>.to_sq();
    auto sqValueP = i32q20_l::from_real<+REAL_VALUE>.to_sq();
    auto qValueN = i32q20::from_sq<ovf::ALLOWED>(sqValueN);
    auto qValueP = i32q20::from_sq<ovf::ALLOWED>(sqValueP);
    auto qValueNOvf = i32q20_ovf::from_sq(sqValueN);
    auto qValuePOvf = i32q20_ovf::from_sq(sqValueP);

    ASSERT_NEAR(-REAL_VALUE, qValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, qValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(-REAL_VALUE, qValueNOvf.to_real(), i32q20_ovf::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, qValuePOvf.to_real(), i32q20_ovf::RESOLUTION);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ---------------------------------- Q Test: Copy & Scale -------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_CopyScale : public ::testing::Test {
protected:
    using i16q4 = q<int16_t, 4, -2048., 2047.9>;
    using i32q4 = q<int32_t, 4, -2048., 2048.>;
    using i32q4_sat = q<int32_t, 4, -2048., 2048., ovf::SATURATE>;
    using i32q4_ovf = q<int32_t, 4, -2048., 2048., ovf::ALLOWED>;
    using i32qm2 = q<int32_t, -2, -2048., 2048.>;
    using i32qm2_sat = q<int32_t, -2, -2048., 2048., ovf::SATURATE>;
    using i32qm2_ovf = q<int32_t, -2, -2048., 2048., ovf::ALLOWED>;
    using i32q8 = q<int32_t, 8, -2048.1, 2048.1>;
    using i32q8_sat = q<int32_t, 8, -2048.1, 2048.1, ovf::SATURATE>;
    using i32q8_ovf = q<int32_t, 8, -2048.1, 2048.1, ovf::ALLOWED>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_CopyScale, q_relimit__some_q_type__relimited_q_type) {
    constexpr double RESTRICTED_LIMIT = 1024.;
    constexpr double EXTENDED_LIMIT = 4096.;
    using restricted_q_t   = i32q4::relimit_t<-RESTRICTED_LIMIT, +RESTRICTED_LIMIT, ovf::SATURATE>;
    using restricted_l_q_t = i32q4::relimit_t<-RESTRICTED_LIMIT, i32q4::REAL_V_MAX, ovf::SATURATE>;
    using restricted_r_q_t = i32q4::relimit_t<i32q4::REAL_V_MIN, +RESTRICTED_LIMIT, ovf::SATURATE>;
    using extended_q_t     = i32q4::relimit_t<-EXTENDED_LIMIT,   +EXTENDED_LIMIT,   ovf::SATURATE>;
    using extended_l_q_t   = i32q4::relimit_t<-EXTENDED_LIMIT,   i32q4::REAL_V_MAX, ovf::SATURATE>;
    using extended_r_q_t   = i32q4::relimit_t<i32q4::REAL_V_MIN, +EXTENDED_LIMIT,   ovf::SATURATE>;
    using shifted_q_l_t    = i32q4::relimit_t<-EXTENDED_LIMIT,   +RESTRICTED_LIMIT, ovf::SATURATE>;
    using shifted_q_r_t    = i32q4::relimit_t<-RESTRICTED_LIMIT, +EXTENDED_LIMIT,   ovf::SATURATE>;

    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -RESTRICTED_LIMIT, +RESTRICTED_LIMIT, ovf::SATURATE>, restricted_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -RESTRICTED_LIMIT, i32q4::REAL_V_MAX, ovf::SATURATE>, restricted_l_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, i32q4::REAL_V_MIN, +RESTRICTED_LIMIT, ovf::SATURATE>, restricted_r_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -EXTENDED_LIMIT,   +EXTENDED_LIMIT,   ovf::SATURATE>, extended_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -EXTENDED_LIMIT,   i32q4::REAL_V_MAX, ovf::SATURATE>, extended_l_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, i32q4::REAL_V_MIN, +EXTENDED_LIMIT,   ovf::SATURATE>, extended_r_q_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -EXTENDED_LIMIT,   +RESTRICTED_LIMIT, ovf::SATURATE>, shifted_q_l_t >));
    ASSERT_TRUE((std::is_same_v< q<int32_t, 4, -RESTRICTED_LIMIT, +EXTENDED_LIMIT,   ovf::SATURATE>, shifted_q_r_t >));
}

TEST_F(QTest_CopyScale, q_copy_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16q4::from_real<REAL_VALUE_A>;
    auto b = i16q4(a);  // explicit copy-constructor from same q-type
    auto c = i16q4::from_q(a);  // copy-construction via named constructor
    i16q4 d = b;  // implicit copy-construction

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16q4::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i16q4::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, d.to_real(), i16q4::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_move_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16q4::from_real<REAL_VALUE_A>;
    i16q4 b = std::move(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16q4::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = i32q8::from_q(a);  // no overflow check needed here

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       q4 type and does not change if the value is up-scaled to a larger F
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_saturate__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -2060.;
    auto a = i32q4_ovf::from_real<REAL_VALUE_A>;
    auto b = i32q8::from_q<ovf::SATURATE>(a);
    auto c = i32q8_sat::from_q(a);

    ASSERT_NEAR(i32q8_sat::REAL_V_MIN, b.to_real(), i32q4_ovf::RESOLUTION);
    ASSERT_NEAR(i32q8_sat::REAL_V_MIN, c.to_real(), i32q4_ovf::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_upscale_copy_constructor_overflow_allowed__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -2060.;
    auto a = i32q4_ovf::from_real<REAL_VALUE_A>;
    auto b = i32q8::from_q<ovf::ALLOWED>(a);
    auto c = i32q8_ovf::from_q(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4_ovf::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32q4_ovf::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = i32qm2::from_q(a);  // no overflow check needed here

    // note: for down-scaling to a smaller F, the representation error is at most the sum of the two
    //       resolutions before and after the scaling operation
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::RESOLUTION + i32qm2::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_saturate__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -2060.;
    auto a = i32q4_ovf::from_real<REAL_VALUE_A>;
    auto b = i32qm2::from_q<ovf::SATURATE>(a);
    auto c = i32qm2_sat::from_q(a);

    ASSERT_NEAR(i32qm2_sat::REAL_V_MIN, b.to_real(), i32q4_ovf::RESOLUTION + i32qm2::RESOLUTION);
    ASSERT_NEAR(i32qm2_sat::REAL_V_MIN, c.to_real(), i32q4_ovf::RESOLUTION + i32qm2_sat::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_downscale_copy_constructor_overflow_allowed__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -2060.;
    auto a = i32q4_ovf::from_real<REAL_VALUE_A>;
    auto b = i32qm2::from_q<ovf::ALLOWED>(a);
    auto c = i32qm2_ovf::from_q(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4_ovf::RESOLUTION + i32qm2::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32q4_ovf::RESOLUTION + i32qm2_ovf::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_assignment__different_q_type__value_is_scaled_and_assigned_only_downscale) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = i32qm2::from_scaled<0>;
    auto c = i32q8::from_scaled<0>;
    b = a;  // downscale-assignment (no runtime checks needed here)
    c = a;  // upscale-assignment (no runtime checks needed here)

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::RESOLUTION + i32qm2::RESOLUTION);  // downscale decreases resolution
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32q4::RESOLUTION);  // upscale does not change initial resolution
}

TEST_F(QTest_CopyScale, q_assignment_saturate__different_q_type__value_is_scaled_and_assigned) {
    constexpr double REAL_VALUE_A = -2060.;
    auto a = i32q4_ovf::from_real<REAL_VALUE_A>;
    auto b = i32qm2_sat::from_scaled<0>;
    auto c = i32q8_sat::from_scaled<0>;
    b = a;  // assignment from different type
    c = a;

    ASSERT_NEAR(i32qm2_sat::REAL_V_MIN, b.to_real(), i32q4_ovf::RESOLUTION + i32qm2_sat::RESOLUTION);
    ASSERT_NEAR(i32q8_sat::REAL_V_MIN, c.to_real(), i32q4_ovf::RESOLUTION);
}

TEST_F(QTest_CopyScale, q_assignment_overflow__different_q_type__value_is_scaled_and_assigned) {
    constexpr double REAL_VALUE_A = -2060.;
    auto a = i32q4_ovf::from_real<REAL_VALUE_A>;
    auto b = i32qm2_ovf::from_scaled<0>;
    auto c = i32q8_ovf::from_scaled<0>;
    b = a;  // assignment from different type
    c = a;

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4_ovf::RESOLUTION + i32qm2_ovf::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32q4_ovf::RESOLUTION);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test: Casting ----------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest_Casting : public ::testing::Test {
protected:
    using i32q20 = q<int32_t, 20, -500., 500.>;
    using i32q20_sat = q<int32_t, 20, -500., 500., ovf::SATURATE>;
    using i32q20_ovf = q<int32_t, 20, -500., 500., ovf::ALLOWED>;

    using i32q4 = q<int32_t, 4, -2048., 2048.>;
    using i32q4_sat = q<int32_t, 4, -2048., 2048., ovf::SATURATE>;
    using i32q4_ovf = q<int32_t, 4, -2048., 2048., ovf::ALLOWED>;
    using i32qm2 = q<int32_t, -2, -2048., 2048.>;
    using i32qm2_sat = q<int32_t, -2, -2048., 2048., ovf::SATURATE>;
    using i32qm2_ovf = q<int32_t, -2, -2048., 2048., ovf::ALLOWED>;

    using u16q6 = q<uint16_t, 6, 0., 500.>;
    using u16q6_sat = q<uint16_t, 6, 0., 500., ovf::SATURATE>;
    using u16q6_ovf = q<uint16_t, 6, 0., 500., ovf::ALLOWED>;
    using u32qm2 = q<uint32_t, -2, 0., 2048.>;
    using u32qm2_sat = q<uint32_t, -2, 0., 2048., ovf::SATURATE>;
    using u32qm2_ovf = q<uint32_t, -2, 0., 2048., ovf::ALLOWED>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed__unsigned_type_smallerF_same_value) {
    constexpr double REAL_VALUE_A = 1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = static_cast<u32qm2_sat>(a);  // this would not compile if u32qm2 was used
    auto c = static_q_cast<u32qm2, ovf::SATURATE>(a);
    auto d = static_q_cast<u32qm2_sat>(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::RESOLUTION + u32qm2_sat::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32q4::RESOLUTION + u32qm2::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, d.to_real(), i32q4::RESOLUTION + u32qm2_sat::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u32qm2, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u32qm2_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr double REAL_VALUE_A = 498.7;
    auto a = u16q6::from_real<REAL_VALUE_A>;
    auto b = static_cast<i32q20_sat>(a);  // this would not compile if i32q20 was used
    auto c = static_q_cast<i32q20, ovf::SATURATE>(a);
    auto d = static_q_cast<i32q20_sat>(a);

    // note: for up-scaling to a larger integral type, the resulting resolution is the resolution
    //       of the source type (because the base type of both target and source is integral)
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, d.to_real(), u16q6::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<i32q20_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32q20, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<i32q20_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr double REAL_VALUE_A = 498.7;
    auto a = u16q6::from_real<REAL_VALUE_A>;
    auto b = static_cast<i32qm2_sat>(a);  // this would not compile if i32qm2 was used
    auto c = static_q_cast<i32qm2, ovf::SATURATE>(a);
    auto d = static_q_cast<i32qm2_sat>(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), u16q6::RESOLUTION + i32qm2_sat::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), u16q6::RESOLUTION + i32qm2::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, d.to_real(), u16q6::RESOLUTION + i32qm2_sat::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<i32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32qm2, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<i32qm2_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr double REAL_VALUE_A = 498.7;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = static_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = static_q_cast<u16q6, ovf::SATURATE>(a);
    auto d = static_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, d.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__positive_real_value_signed_too_large__smaller_unsigned_type_largerF_value_saturated) {
    constexpr double REAL_VALUE_A = 512.5;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = static_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = static_q_cast<u16q6, ovf::SATURATE>(a);
    auto d = static_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(u16q6_sat::REAL_V_MAX, b.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_NEAR(u16q6::REAL_V_MAX, c.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(u16q6_sat::REAL_V_MAX, d.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__negative_real_value__smaller_unsigned_type_largerF_saturated_value) {
    constexpr double REAL_VALUE_A = -498.7;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = static_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = static_q_cast<u16q6, ovf::SATURATE>(a);
    auto d = static_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(u16q6_sat::REAL_V_MIN, b.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_NEAR(u16q6::REAL_V_MIN, c.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(u16q6_sat::REAL_V_MIN, d.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(d)>));
}

TEST_F(QTest_Casting, q_static_cast__negative_real_value__smaller_unsigned_type_largerF_value_overflow) {
    constexpr double REAL_VALUE_A = -498.7;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = static_cast<u16q6_ovf>(a);  // this is possible, however hardly predictable if a is signed
    auto c = static_q_cast<u16q6, ovf::ALLOWED>(a);
    auto d = static_q_cast<u16q6_ovf>(a);

    constexpr double EXPECTED_VALUE = 525.3125;
    ASSERT_NEAR(EXPECTED_VALUE, b.to_real(), u16q6_ovf::RESOLUTION);
    ASSERT_NEAR(EXPECTED_VALUE, c.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(EXPECTED_VALUE, d.to_real(), u16q6_ovf::RESOLUTION);
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

    using i16qm3 = q<int16_t, -3, -100000., 100000.>;  // i16 -> u32, max delta F is 17
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = i16qm3::from_real_ovf<-110000.>;
    auto b = i16qm3::from_real<i16qm3::REAL_V_MIN>;
    auto c = i16qm3::from_real<50000.>;
    auto d = i16qm3::from_real<90000.>;
    auto e = i16qm3::from_real<i16qm3::REAL_V_MAX>;
    auto f = i16qm3::from_real_ovf<110000.>;
    auto g = i16qm3::from_real_ovf<200000.>;
    auto ac = static_q_cast<u32q14, ovf::SATURATE>(a);
    auto bc = static_q_cast<u32q14, ovf::SATURATE>(b);
    auto bc_ovf = static_q_cast<u32q14, ovf::ALLOWED>(b);
    auto cc = static_q_cast<u32q14, ovf::SATURATE>(c);
    auto dc = static_q_cast<u32q14, ovf::SATURATE>(d);
    auto ec = static_q_cast<u32q14, ovf::SATURATE>(e);
    auto fc = static_q_cast<u32q14, ovf::SATURATE>(f);
    auto gc = static_q_cast<u32q14, ovf::SATURATE>(g);
    auto gc_ovf = static_q_cast<u32q14, ovf::ALLOWED>(g);

    ASSERT_NEAR(u32q14::REAL_V_MIN, ac.to_real(), i16qm3::RESOLUTION);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, bc.to_real(), i16qm3::RESOLUTION);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(162144., bc_ovf.to_real(), i16qm3::RESOLUTION);  // (u32::max - 100000*2^14) / 2^14
    ASSERT_NEAR(u32q14::REAL_V_MIN, cc.to_real(), i16qm3::RESOLUTION);  // 50000 saturated to u32q14::REAL_V_MIN
    ASSERT_NEAR( 90000., dc.to_real(), i16qm3::RESOLUTION);
    ASSERT_NEAR(i16qm3::REAL_V_MAX, ec.to_real(), i16qm3::RESOLUTION);
    ASSERT_NEAR(110000., fc.to_real(), i16qm3::RESOLUTION);
    ASSERT_NEAR(u32q14::REAL_V_MAX, gc.to_real(), i16qm3::RESOLUTION);
    ASSERT_NEAR(200000., gc_ovf.to_real(), i16qm3::RESOLUTION);
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

    using u16qm3 = q<uint16_t, -3, 0., 400000.>;  // u16 -> i32, max delta F is 15
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = u16qm3::from_real<u16qm3::REAL_V_MIN>;
    auto b = u16qm3::from_real<50000.>;
    auto c = u16qm3::from_real<u16qm3::REAL_V_MAX>;
    auto d = u16qm3::from_real_ovf<410000.>;
    auto e = u16qm3::from_real_ovf<520000.>;
    auto ac = static_q_cast<i32q12, ovf::SATURATE>(a);
    auto bc = static_q_cast<i32q12, ovf::SATURATE>(b);
    auto cc = static_q_cast<i32q12, ovf::SATURATE>(c);
    auto dc = static_q_cast<i32q12, ovf::SATURATE>(d);
    auto ec = static_q_cast<i32q12, ovf::SATURATE>(e);
    auto ec_ovf = static_q_cast<i32q12, ovf::ALLOWED>(e);

    ASSERT_NEAR(u16qm3::REAL_V_MIN, ac.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR( 50000., bc.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR(u16qm3::REAL_V_MAX, cc.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR(410000., dc.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR(i32q12::REAL_V_MAX, ec.to_real(), u16qm3::RESOLUTION);  // 520000 saturated to i32q12::REAL_V_MAX
    ASSERT_NEAR(520000., ec_ovf.to_real(), u16qm3::RESOLUTION);
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

    using i16qm4 = q<int16_t, -4, -400000., 400000.>;  // i16 -> i32, max delta F is 16
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = i16qm4::from_real<i16qm4::REAL_V_MIN>;
    auto b = i16qm4::from_real<-50000.>;
    auto c = i16qm4::from_real<i16qm4::REAL_V_MAX>;
    auto d = i16qm4::from_real_ovf<410000.>;
    auto e = i16qm4::from_real_ovf<520000.>;
    auto ac = static_q_cast<i32q12, ovf::SATURATE>(a);
    auto ac_ovf = static_q_cast<i32q12, ovf::ALLOWED>(a);
    auto bc = static_q_cast<i32q12, ovf::SATURATE>(b);
    auto cc = static_q_cast<i32q12, ovf::SATURATE>(c);
    auto dc = static_q_cast<i32q12, ovf::SATURATE>(d);
    auto ec = static_q_cast<i32q12, ovf::SATURATE>(e);
    auto ec_ovf = static_q_cast<i32q12, ovf::ALLOWED>(e);

    ASSERT_NEAR(i32q12::REAL_V_MIN, ac.to_real(), i16qm4::RESOLUTION);  // -400000 saturated to i32q12::REAL_V_MIN
    ASSERT_NEAR(i16qm4::REAL_V_MIN, ac_ovf.to_real(), i16qm4::RESOLUTION);  // -400000 not saturated
    ASSERT_NEAR(-50000., bc.to_real(), i16qm4::RESOLUTION);
    ASSERT_NEAR(i16qm4::REAL_V_MAX, cc.to_real(), i16qm4::RESOLUTION);
    ASSERT_NEAR(410000., dc.to_real(), i16qm4::RESOLUTION);
    ASSERT_NEAR(i32q12::REAL_V_MAX, ec.to_real(), i16qm4::RESOLUTION);  // 520000 saturated to i32q12::REAL_V_MAX
    ASSERT_NEAR(520000., ec_ovf.to_real(), i16qm4::RESOLUTION);  // not saturated
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

    using u16qm2 = q<uint16_t, -2, 10000., 100000.>;  // u16 -> u32, max delta F is 16
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = u16qm2::from_real_ovf<0.>;
    auto b = u16qm2::from_real<u16qm2::REAL_V_MIN>;
    auto c = u16qm2::from_real<50000.>;
    auto d = u16qm2::from_real<90000.>;
    auto e = u16qm2::from_real<u16qm2::REAL_V_MAX>;
    auto f = u16qm2::from_real_ovf<210000.>;
    auto ac = static_q_cast<u32q14, ovf::SATURATE>(a);
    auto bc = static_q_cast<u32q14, ovf::SATURATE>(b);
    auto cc = static_q_cast<u32q14, ovf::SATURATE>(c);
    auto dc = static_q_cast<u32q14, ovf::SATURATE>(d);
    auto ec = static_q_cast<u32q14, ovf::SATURATE>(e);
    auto fc = static_q_cast<u32q14, ovf::SATURATE>(f);
    auto fc_ovf = static_q_cast<u32q14, ovf::ALLOWED>(f);

    ASSERT_NEAR(u32q14::REAL_V_MIN, ac.to_real(), u16qm2::RESOLUTION);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, bc.to_real(), u16qm2::RESOLUTION);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, cc.to_real(), u16qm2::RESOLUTION);  // 50000 saturated to u32q14::REAL_V_MIN
    ASSERT_NEAR( 90000., dc.to_real(), u16qm2::RESOLUTION);
    ASSERT_NEAR(u16qm2::REAL_V_MAX, ec.to_real(), u16qm2::RESOLUTION);
    ASSERT_NEAR(u32q14::REAL_V_MAX, fc.to_real(), u16qm2::RESOLUTION);  // 210000 saturated to u32q14::REAL_V_MAX
    ASSERT_NEAR(210000., fc_ovf.to_real(), u16qm2::RESOLUTION);  // not saturated
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed__unsigned_type_smallerF_same_value) {
    constexpr double REAL_VALUE_A = 1024.2;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = safe_q_cast<u32qm2_sat>(a);  // this would not compile if u32qm2 was used
    auto c = safe_q_cast<u32qm2, ovf::SATURATE>(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32q4::RESOLUTION + u32qm2_sat::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), i32q4::RESOLUTION + u32qm2::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u32qm2, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr double REAL_VALUE_A = 498.7;
    auto a = u16q6::from_real<REAL_VALUE_A>;
    auto b = safe_q_cast<i32q20_sat>(a);  // this would not compile if i32q20 was used
    auto c = safe_q_cast<i32q20, ovf::SATURATE>(a);

    // note: for up-scaling to a larger integral type, the resulting resolution is the resolution
    //       of the source type (because the base type of both target and source is integral)
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), u16q6::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), u16q6::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<i32q20_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32q20, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr double REAL_VALUE_A = 498.7;
    auto a = u16q6::from_real<REAL_VALUE_A>;
    auto b = safe_q_cast<i32qm2_sat>(a);  // this would not compile if i32qm2 was used
    auto c = safe_q_cast<i32qm2, ovf::SATURATE>(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), u16q6::RESOLUTION + i32qm2_sat::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), u16q6::RESOLUTION + i32qm2::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<i32qm2_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<i32qm2, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr double REAL_VALUE_A = 498.7;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = safe_q_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6, ovf::SATURATE>(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_NEAR(REAL_VALUE_A, c.to_real(), u16q6::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__positive_real_value_signed_too_large__smaller_unsigned_type_largerF_value_saturated) {
    constexpr double REAL_VALUE_A = 512.5;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = safe_q_cast<u16q6, ovf::SATURATE>(a);
    auto c = safe_q_cast<u16q6_sat>(a);

    ASSERT_NEAR(u16q6_sat::REAL_V_MAX, b.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_NEAR(u16q6::REAL_V_MAX, c.to_real(), u16q6::RESOLUTION);
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(b)>));
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(c)>));
}

TEST_F(QTest_Casting, q_safe_cast__negative_real_value__smaller_unsigned_type_largerF_saturated_value) {
    constexpr double REAL_VALUE_A = -498.7;
    auto a = i32q4::from_real<REAL_VALUE_A>;
    auto b = safe_q_cast<u16q6_sat>(a);  // this would not compile if u16q6 was used
    auto c = safe_q_cast<u16q6, ovf::SATURATE>(a);

    ASSERT_NEAR(u16q6_sat::REAL_V_MIN, b.to_real(), u16q6_sat::RESOLUTION);
    ASSERT_NEAR(u16q6::REAL_V_MIN, c.to_real(), u16q6::RESOLUTION);
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

    using i16qm3 = q<int16_t, -3, -100000., 100000.>;  // i16 -> u32, max delta F is 17
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = i16qm3::from_real_ovf<-110000.>;
    auto b = i16qm3::from_real<i16qm3::REAL_V_MIN>;
    auto c = i16qm3::from_real<50000.>;
    auto d = i16qm3::from_real<90000.>;
    auto e = i16qm3::from_real<i16qm3::REAL_V_MAX>;
    auto f = i16qm3::from_real_ovf<110000.>;
    auto ac = safe_q_cast<u32q14, ovf::SATURATE>(a);
    auto bc = safe_q_cast<u32q14, ovf::SATURATE>(b);
    auto cc = safe_q_cast<u32q14, ovf::SATURATE>(c);
    auto dc = safe_q_cast<u32q14, ovf::SATURATE>(d);
    auto ec = safe_q_cast<u32q14, ovf::SATURATE>(e);
    auto fc = safe_q_cast<u32q14, ovf::SATURATE>(f);

    ASSERT_NEAR(u32q14::REAL_V_MIN, ac.to_real(), i16qm3::RESOLUTION);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, bc.to_real(), i16qm3::RESOLUTION);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, cc.to_real(), i16qm3::RESOLUTION);  // 50000 saturated to u32q14::REAL_V_MIN
    ASSERT_NEAR( 90000., dc.to_real(), i16qm3::RESOLUTION);
    ASSERT_NEAR(i16qm3::REAL_V_MAX, ec.to_real(), i16qm3::RESOLUTION);
    ASSERT_NEAR(110000., fc.to_real(), i16qm3::RESOLUTION);
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

    using u16qm3 = q<uint16_t, -3, 0., 400000.>;  // u16 -> i32, max delta F is 15
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = u16qm3::from_real<u16qm3::REAL_V_MIN>;
    auto b = u16qm3::from_real<50000.>;
    auto c = u16qm3::from_real<u16qm3::REAL_V_MAX>;
    auto d = u16qm3::from_real_ovf<410000.>;
    auto e = u16qm3::from_real_ovf<520000.>;
    auto ac = safe_q_cast<i32q12, ovf::SATURATE>(a);
    auto bc = safe_q_cast<i32q12, ovf::SATURATE>(b);
    auto cc = safe_q_cast<i32q12, ovf::SATURATE>(c);
    auto dc = safe_q_cast<i32q12, ovf::SATURATE>(d);
    auto ec = safe_q_cast<i32q12, ovf::SATURATE>(e);

    ASSERT_NEAR(u16qm3::REAL_V_MIN, ac.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR( 50000., bc.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR(u16qm3::REAL_V_MAX, cc.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR(410000., dc.to_real(), u16qm3::RESOLUTION);
    ASSERT_NEAR(i32q12::REAL_V_MAX, ec.to_real(), u16qm3::RESOLUTION);  // 520000 saturated to i32q12::REAL_V_MAX
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

    using i16qm4 = q<int16_t, -4, -400000., 400000.>;  // i16 -> i32, max delta F is 16
    using i32q12 = q<int32_t, 12, -80000., 500000.>;
    auto a = i16qm4::from_real<i16qm4::REAL_V_MIN>;
    auto b = i16qm4::from_real<-50000.>;
    auto c = i16qm4::from_real<i16qm4::REAL_V_MAX>;
    auto d = i16qm4::from_real_ovf<410000.>;
    auto e = i16qm4::from_real_ovf<520000.>;
    auto ac = safe_q_cast<i32q12, ovf::SATURATE>(a);
    auto bc = safe_q_cast<i32q12, ovf::SATURATE>(b);
    auto cc = safe_q_cast<i32q12, ovf::SATURATE>(c);
    auto dc = safe_q_cast<i32q12, ovf::SATURATE>(d);
    auto ec = safe_q_cast<i32q12, ovf::SATURATE>(e);

    ASSERT_NEAR(i32q12::REAL_V_MIN, ac.to_real(), i16qm4::RESOLUTION);  // -400000 saturated to i32q12::REAL_V_MIN
    ASSERT_NEAR(-50000., bc.to_real(), i16qm4::RESOLUTION);
    ASSERT_NEAR(i16qm4::REAL_V_MAX, cc.to_real(), i16qm4::RESOLUTION);
    ASSERT_NEAR(410000., dc.to_real(), i16qm4::RESOLUTION);
    ASSERT_NEAR(i32q12::REAL_V_MAX, ec.to_real(), i16qm4::RESOLUTION);  // 520000 saturated to i32q12::REAL_V_MAX
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

    using u16qm2 = q<uint16_t, -2, 10000., 100000.>;  // u16 -> u32, max delta F is 16
    using u32q14 = q<uint32_t, 14, 80000., 160000.>;
    auto a = u16qm2::from_real_ovf<0.>;
    auto b = u16qm2::from_real<u16qm2::REAL_V_MIN>;
    auto c = u16qm2::from_real<50000.>;
    auto d = u16qm2::from_real<90000.>;
    auto e = u16qm2::from_real<u16qm2::REAL_V_MAX>;
    auto f = u16qm2::from_real_ovf<110000.>;
    auto ac = safe_q_cast<u32q14, ovf::SATURATE>(a);
    auto bc = safe_q_cast<u32q14, ovf::SATURATE>(b);
    auto cc = safe_q_cast<u32q14, ovf::SATURATE>(c);
    auto dc = safe_q_cast<u32q14, ovf::SATURATE>(d);
    auto ec = safe_q_cast<u32q14, ovf::SATURATE>(e);
    auto fc = safe_q_cast<u32q14, ovf::SATURATE>(f);

    ASSERT_NEAR(u32q14::REAL_V_MIN, ac.to_real(), u16qm2::RESOLUTION);  // (u32::max - 110000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, bc.to_real(), u16qm2::RESOLUTION);  // (u32::max - 100000*2^14) / 2^14 saturated
    ASSERT_NEAR(u32q14::REAL_V_MIN, cc.to_real(), u16qm2::RESOLUTION);  // 50000 saturated to u32q14::REAL_V_MIN
    ASSERT_NEAR( 90000., dc.to_real(), u16qm2::RESOLUTION);
    ASSERT_NEAR(u16qm2::REAL_V_MAX, ec.to_real(), u16qm2::RESOLUTION);
    ASSERT_NEAR(110000., fc.to_real(), u16qm2::RESOLUTION);
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_signed__unsigned_type_smallerF_same_value) {
    constexpr int32_t SCALED_VALUE_A = 1024;
    auto a = i32q4::from_scaled<SCALED_VALUE_A>;
    auto b = force_q_cast<u32qm2>(a);

    ASSERT_EQ(static_cast<u32qm2::base_t>(SCALED_VALUE_A), b.reveal());
    ASSERT_TRUE((std::is_same_v<u32qm2, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_unsigned__larger_signed_type_largerF_same_value) {
    constexpr uint16_t SCALED_VALUE_A = 498u;
    auto a = u16q6::from_scaled<SCALED_VALUE_A>;
    auto b = force_q_cast<i32q20>(a);

    ASSERT_EQ(static_cast<i32q20::base_t>(SCALED_VALUE_A), b.reveal());
    ASSERT_TRUE((std::is_same_v<i32q20, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_unsigned__larger_signed_type_smallerF_same_value) {
    constexpr uint16_t SCALED_VALUE_A = 498u;
    auto a = u16q6::from_scaled<SCALED_VALUE_A>;
    auto b = force_q_cast<i32qm2>(a);

    ASSERT_EQ(static_cast<i32qm2::base_t>(SCALED_VALUE_A), b.reveal());
    ASSERT_TRUE((std::is_same_v<i32qm2, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__positive_scaled_value_signed__smaller_unsigned_type_largerF_same_value) {
    constexpr int32_t SCALED_VALUE_A = 498;
    auto a = i32q4::from_scaled<SCALED_VALUE_A>;
    auto b = force_q_cast<u16q6>(a);

    ASSERT_EQ(static_cast<u16q6::base_t>(SCALED_VALUE_A), b.reveal());  // expect: 498u
    ASSERT_TRUE((std::is_same_v<u16q6, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__negative_scaled_value__smaller_unsigned_type_largerF_same_value_saturate_ignored) {
    constexpr int32_t SCALED_VALUE_A = -498;
    auto a = i32q4::from_scaled<SCALED_VALUE_A>;
    auto b = force_q_cast<u16q6_sat>(a);

    ASSERT_EQ(static_cast<u16q6_sat::base_t>(SCALED_VALUE_A), b.reveal());  // expect: 65038u
    ASSERT_TRUE((std::is_same_v<u16q6_sat, decltype(b)>));
}

TEST_F(QTest_Casting, q_force_cast__negative_scaled_value__smaller_unsigned_type_largerF_same_value_overflow_ignored) {
    constexpr int32_t SCALED_VALUE_A = -498;
    auto a = i32q4::from_scaled<SCALED_VALUE_A>;
    auto b = force_q_cast<u16q6_ovf>(a);

    ASSERT_EQ(static_cast<u16q6::base_t>(SCALED_VALUE_A), b.reveal());  // expect: 65038u
    ASSERT_TRUE((std::is_same_v<u16q6_ovf, decltype(b)>));
}

// EOF
