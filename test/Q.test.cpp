/* \file
 * Tests for q.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/q.hpp>
using namespace fpm;


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ Q Test -------------------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class QTest : public ::testing::Test {
protected:
    using i16q6 = q<int16_t, 6, -500., 500.>;
    using i16q6_sat = q<int16_t, 6, -500., 500., overflow::SATURATE>;
    using i16q6_ovf = q<int16_t, 6, -500., 500., overflow::ALLOWED>;
    using i32q20 = q<int32_t, 20, -500., 500.>;
    using i32q20_sat = q<int32_t, 20, -500., 500., overflow::SATURATE>;
    using i32q20_ovf = q<int32_t, 20, -500., 500., overflow::ALLOWED>;
    using i16sq6 = sq<int16_t, 6, -500., 500.>;
    using i32sq20 = sq<int32_t, 20, -500., 500.>;
    using i32sq20_l = sq<int32_t, 20, -1000., 1000.>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest, q_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double REAL_VALUE = -495.1;
    auto qValue = i16q6::from_real<REAL_VALUE>();  // this does not perform overflow checks

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.to_real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(qValue.to_real())>));

    constexpr int16_t RESULT_MEM_VALUE = -31686;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_EQ(-495, qValue.to_real<int>());  // test this once to make sure it works as expected
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
}

TEST_F(QTest, q_from_real__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = -511.;
    auto qValue = i16q6::from_real<REAL_VALUE, overflow::SATURATE>();
    auto qValueSat = i16q6_sat::from_real<REAL_VALUE>();

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_from_real__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = +511.;
    auto qValue = i16q6::from_real<REAL_VALUE, overflow::SATURATE>();
    auto qValueSat = i16q6_sat::from_real<REAL_VALUE>();

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_from_real__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = -511.;
    auto qValue = i16q6::from_real<REAL_VALUE, overflow::ALLOWED>();
    auto qValueOvf = i16q6_ovf::from_real<REAL_VALUE>();

    constexpr int16_t RESULT_MEM_VALUE = -32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_from_real__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = +511.;
    auto qValue = i16q6::from_real<REAL_VALUE, overflow::ALLOWED>();
    auto qValueOvf = i16q6_ovf::from_real<REAL_VALUE>();

    constexpr int16_t RESULT_MEM_VALUE = +32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t MEM_VALUE = -31686;
    auto qValue = i16q6::from_scaled<MEM_VALUE>();  // this does not perform overflow checks

    constexpr double RESULT_REAL_VALUE = -495.1;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
}

TEST_F(QTest, q_from_scaled__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, overflow::SATURATE>();
    auto qValueSat = i16q6_sat::from_scaled<MEM_VALUE>();

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_from_scaled__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = +32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, overflow::SATURATE>();
    auto qValueSat = i16q6_sat::from_scaled<MEM_VALUE>();

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_from_scaled__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, overflow::ALLOWED>();
    auto qValueOvf = i16q6_ovf::from_scaled<MEM_VALUE>();

    constexpr double RESULT_REAL_VALUE = -511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_from_scaled__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = +32704;
    auto qValue = i16q6::from_scaled<MEM_VALUE, overflow::ALLOWED>();
    auto qValueOvf = i16q6_ovf::from_scaled<MEM_VALUE>();

    constexpr double RESULT_REAL_VALUE = +511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_construct__constexpr_negative_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::construct<overflow::SATURATE>(MEM_VALUE);
    auto qValueSat = i16q6_sat::construct(MEM_VALUE);

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_construct__constexpr_positive_int16_positiveF_saturate__expected_value) {
    constexpr int16_t MEM_VALUE = 32704;
    auto qValue = i16q6::construct<overflow::SATURATE>(MEM_VALUE);
    auto qValueSat = i16q6_sat::construct(MEM_VALUE);

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_construct__constexpr_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = -32704;
    auto qValue = i16q6::construct<overflow::ALLOWED>(MEM_VALUE);
    auto qValueOvf = i16q6_ovf::construct(MEM_VALUE);

    constexpr double RESULT_REAL_VALUE = -511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_construct__constexpr_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr int16_t MEM_VALUE = +32704;
    auto qValue = i16q6::construct<overflow::ALLOWED>(MEM_VALUE);
    auto qValueOvf = i16q6_ovf::construct(MEM_VALUE);

    constexpr double RESULT_REAL_VALUE = +511.;
    ASSERT_EQ(MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(RESULT_REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_construct__variable_negative_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = -511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<overflow::SATURATE>(value);
    auto qValueSat = i16q6_sat::construct(value);

    ASSERT_EQ(i16q6::V_MIN, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MIN, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MIN, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MIN, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_construct__variable_positive_int16_positiveF_saturate__expected_value) {
    constexpr double REAL_VALUE = +511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<overflow::SATURATE>(value);
    auto qValueSat = i16q6_sat::construct(value);

    ASSERT_EQ(i16q6::V_MAX, qValue.reveal());
    ASSERT_NEAR(i16q6::REAL_V_MAX, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(i16q6_sat::V_MAX, qValueSat.reveal());
    ASSERT_NEAR(i16q6_sat::REAL_V_MAX, qValueSat.to_real(), i16q6_sat::RESOLUTION);
}

TEST_F(QTest, q_construct__variable_negative_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = -511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<overflow::ALLOWED>(value);
    auto qValueOvf = i16q6_ovf::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = -32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_construct__variable_positive_int16_positiveF_overflow_allowed__expected_value) {
    constexpr double REAL_VALUE = +511.;
    int16_t volatile value = v2s<int16_t,6>(REAL_VALUE);
    auto qValue = i16q6::construct<overflow::ALLOWED>(value);
    auto qValueOvf = i16q6_ovf::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = +32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i16q6::RESOLUTION);
    ASSERT_EQ(RESULT_MEM_VALUE, qValueOvf.reveal());
    ASSERT_NEAR(REAL_VALUE, qValueOvf.to_real(), i16q6_ovf::RESOLUTION);
}

TEST_F(QTest, q_embedded_sq__from_real__sq_value_is_correct) {
    constexpr double REAL_SQ_VALUE = +356.;
    auto sqValue = i32q20::sq<>::from_real<REAL_SQ_VALUE>();

    ASSERT_NEAR(REAL_SQ_VALUE, sqValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest, q_embedded_sq__smaller_value_range_from_real__sq_value_is_correct) {
    constexpr double REAL_SQ_VALUE = -400.;
    auto sqValue = i32q20::sq<-400., 400.>::from_real<REAL_SQ_VALUE>();

    ASSERT_NEAR(REAL_SQ_VALUE, sqValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest, q_to_sq__same_value_range__no_overflow_check_performed) {
    constexpr double REAL_VALUE = -400.;
    auto sqValue = i32q20::from_real<REAL_VALUE>().to_sq();  // does not include a range check

    ASSERT_NEAR(REAL_VALUE, sqValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest, q_to_sq__different_value_range_overflow_forbidden__does_not_compile) {
    // this must not compile!
    // note: although the value is inside the value range in this case, the compiler will add
    //       overflow checks as soon as the value range for sq is smaller (because the value of q
    //       might be changed before to_sq() is called).
    //auto sqValue = i32q20::from_real<-400.>().to_sq<-400., 400., overflow::FORBIDDEN>();
}

TEST_F(QTest, q_to_sq__different_value_range_overflow_asserted__does_not_compile) {
    // this must trigger an assertion trap at runtime
    //auto sqValue = i32q20::from_real<-410.>().to_sq<-400., 400., overflow::ASSERT>();
}

TEST_F(QTest, q_to_sq__different_value_range_overflow_saturated__value_is_saturated) {
    constexpr double SQ_RANGE_N = -400., SQ_RANGE_P = +400.;
    auto sqValueN = i32q20::from_real<-450.>().to_sq<SQ_RANGE_N, SQ_RANGE_P, overflow::SATURATE>();
    auto sqValueP = i32q20::from_real<+450.>().to_sq<SQ_RANGE_N, SQ_RANGE_P, overflow::SATURATE>();
    auto sqValueNSat = i32q20_sat::from_real<-450.>().to_sq<SQ_RANGE_N, SQ_RANGE_P>();
    auto sqValuePSat = i32q20_sat::from_real<+450.>().to_sq<SQ_RANGE_N, SQ_RANGE_P>();

    ASSERT_NEAR(SQ_RANGE_N, sqValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(SQ_RANGE_P, sqValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(SQ_RANGE_N, sqValueNSat.to_real(), i32q20_sat::RESOLUTION);
    ASSERT_NEAR(SQ_RANGE_P, sqValuePSat.to_real(), i32q20_sat::RESOLUTION);
}

TEST_F(QTest, q_to_sq__different_value_range_overflow_allowed__value_can_overflow) {
    constexpr double REAL_VALUE = 450.;
    auto sqValueN = i32q20::from_real<-REAL_VALUE>().to_sq<-400., +400., overflow::ALLOWED>();
    auto sqValueP = i32q20::from_real<+REAL_VALUE>().to_sq<-400., +400., overflow::ALLOWED>();
    auto sqValueNOvf = i32q20_ovf::from_real<-REAL_VALUE>().to_sq<-400., +400.>();
    auto sqValuePOvf = i32q20_ovf::from_real<+REAL_VALUE>().to_sq<-400., +400.>();

    ASSERT_NEAR(-REAL_VALUE, sqValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, sqValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(-REAL_VALUE, sqValueNOvf.to_real(), i32q20_ovf::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, sqValuePOvf.to_real(), i32q20_ovf::RESOLUTION);
}

TEST_F(QTest, q_from_sq__same_value_range_value_within_limits__q_value_equal_no_overflow_check) {
    constexpr double REAL_VALUE = -234.5;
    auto sqValue = i32sq20::from_real<REAL_VALUE>();
    auto qValue = i32q20::from_sq(sqValue);  // does not perform overflow checks in this case

    ASSERT_NEAR(REAL_VALUE, qValue.to_real(), i32q20::RESOLUTION);
}

TEST_F(QTest, q_from_sq__different_value_range_default__does_not_compile) {
    // this must not compile!
    //auto sqValue = i32sq20_l::from_real<-654.>();
    //auto qValue = i32q20::from_sq(sqValue);
}

TEST_F(QTest, q_from_sq__different_value_range_saturate__q_value_is_saturated) {
    auto sqValueN = i32sq20_l::from_real<-654.>();
    auto sqValueP = i32sq20_l::from_real<+654.>();
    auto qValueN = i32q20::from_sq<overflow::SATURATE>(sqValueN);
    auto qValueP = i32q20::from_sq<overflow::SATURATE>(sqValueP);
    auto qValueNSat = i32q20_sat::from_sq(sqValueN);
    auto qValuePSat = i32q20_sat::from_sq(sqValueP);

    ASSERT_NEAR(i32q20::REAL_V_MIN, qValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(i32q20::REAL_V_MAX, qValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(i32q20_sat::REAL_V_MIN, qValueNSat.to_real(), i32q20_sat::RESOLUTION);
    ASSERT_NEAR(i32q20_sat::REAL_V_MAX, qValuePSat.to_real(), i32q20_sat::RESOLUTION);
}

TEST_F(QTest, q_from_sq__different_value_range_overflow_allowed__q_value_can_overflow) {
    constexpr double REAL_VALUE = 654.;
    auto sqValueN = i32sq20_l::from_real<-REAL_VALUE>();
    auto sqValueP = i32sq20_l::from_real<+REAL_VALUE>();
    auto qValueN = i32q20::from_sq<overflow::ALLOWED>(sqValueN);
    auto qValueP = i32q20::from_sq<overflow::ALLOWED>(sqValueP);
    auto qValueNOvf = i32q20_ovf::from_sq(sqValueN);
    auto qValuePOvf = i32q20_ovf::from_sq(sqValueP);

    ASSERT_NEAR(-REAL_VALUE, qValueN.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, qValueP.to_real(), i32q20::RESOLUTION);
    ASSERT_NEAR(-REAL_VALUE, qValueNOvf.to_real(), i32q20_ovf::RESOLUTION);
    ASSERT_NEAR(+REAL_VALUE, qValuePOvf.to_real(), i32q20_ovf::RESOLUTION);
}

// EOF
