/**
 * @file evaluator_test.cpp
 * @brief ExpressionEvaluator 单元测试
 */

#include <gtest/gtest.h>
#include "evaluator.h"
#include <cmath>
#include <limits>

TEST(ExpressionEvaluatorTest, HandlesEmptyExpression) {
    // 空表达式应返回 0.0
    EXPECT_DOUBLE_EQ(0.0, ExpressionEvaluator::evaluate(""));
}

TEST(ExpressionEvaluatorTest, HandlesSingleNumber) {
    EXPECT_DOUBLE_EQ(42.0, ExpressionEvaluator::evaluate("42"));
    EXPECT_DOUBLE_EQ(3.14, ExpressionEvaluator::evaluate("3.14"));
    EXPECT_DOUBLE_EQ(0.5, ExpressionEvaluator::evaluate("0.5"));
    EXPECT_DOUBLE_EQ(100.0, ExpressionEvaluator::evaluate("100"));
}

TEST(ExpressionEvaluatorTest, HandlesBasicOperations) {
    EXPECT_DOUBLE_EQ(5.0, ExpressionEvaluator::evaluate("2+3"));
    EXPECT_DOUBLE_EQ(1.0, ExpressionEvaluator::evaluate("3-2"));
    EXPECT_DOUBLE_EQ(6.0, ExpressionEvaluator::evaluate("2*3"));
    EXPECT_DOUBLE_EQ(2.0, ExpressionEvaluator::evaluate("6/3"));
}

TEST(ExpressionEvaluatorTest, HandlesOperatorPrecedence) {
    // 乘除优先于加减
    EXPECT_DOUBLE_EQ(7.0, ExpressionEvaluator::evaluate("1+2*3"));
    EXPECT_DOUBLE_EQ(9.0, ExpressionEvaluator::evaluate("3*2+3"));
    EXPECT_DOUBLE_EQ(7.0, ExpressionEvaluator::evaluate("10-6/2"));
    EXPECT_DOUBLE_EQ(2.0, ExpressionEvaluator::evaluate("8/2-2"));
}

TEST(ExpressionEvaluatorTest, HandlesParentheses) {
    EXPECT_DOUBLE_EQ(9.0, ExpressionEvaluator::evaluate("(1+2)*3"));
    EXPECT_DOUBLE_EQ(3.0, ExpressionEvaluator::evaluate("(10-4)/2"));
    EXPECT_DOUBLE_EQ(16.0, ExpressionEvaluator::evaluate("(2+2)*(2+2)"));
    EXPECT_DOUBLE_EQ(2.0, ExpressionEvaluator::evaluate("((1+1))"));
}

TEST(ExpressionEvaluatorTest, HandlesDecimalNumbers) {
    EXPECT_DOUBLE_EQ(1.5, ExpressionEvaluator::evaluate("0.5+1.0"));
    EXPECT_DOUBLE_EQ(0.3, ExpressionEvaluator::evaluate("0.5-0.2"));
    EXPECT_DOUBLE_EQ(1.5, ExpressionEvaluator::evaluate("0.5*3.0"));
    EXPECT_DOUBLE_EQ(2.0, ExpressionEvaluator::evaluate("5.0/2.5"));
}

TEST(ExpressionEvaluatorTest, HandlesWhitespace) {
    EXPECT_DOUBLE_EQ(5.0, ExpressionEvaluator::evaluate(" 2 + 3 "));
    EXPECT_DOUBLE_EQ(10.0, ExpressionEvaluator::evaluate("( 2 + 3 ) * 2"));
    EXPECT_DOUBLE_EQ(6.0, ExpressionEvaluator::evaluate("2 * 3"));
}

TEST(ExpressionEvaluatorTest, ThrowsOnInvalidCharacters) {
    EXPECT_THROW(ExpressionEvaluator::evaluate("2 & 3"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("abc"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("2 + a"), std::invalid_argument);
}

TEST(ExpressionEvaluatorTest, ThrowsOnMismatchedParentheses) {
    EXPECT_THROW(ExpressionEvaluator::evaluate("(2+3"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("2+3)"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("((2+3)"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("(2+3))"), std::invalid_argument);
}

TEST(ExpressionEvaluatorTest, ThrowsOnDivisionByZero) {
    EXPECT_THROW(ExpressionEvaluator::evaluate("5/0"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("10/(5-5)"), std::invalid_argument);
}

TEST(ExpressionEvaluatorTest, ThrowsOnInvalidNumberFormat) {
    // 多个小数点
    EXPECT_THROW(ExpressionEvaluator::evaluate("2.3.4"), std::invalid_argument);
    // 只有小数点
    EXPECT_THROW(ExpressionEvaluator::evaluate("."), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("+.2"), std::invalid_argument);
}

TEST(ExpressionEvaluatorTest, ThrowsOnInvalidExpressionStructure) {
    EXPECT_THROW(ExpressionEvaluator::evaluate("2+"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("*3"), std::invalid_argument);
    EXPECT_THROW(ExpressionEvaluator::evaluate("2 3"), std::invalid_argument);
}

TEST(ExpressionEvaluatorTest, HandlesComplexExpressions) {
    EXPECT_DOUBLE_EQ(23.0, ExpressionEvaluator::evaluate("2+3*7"));
    EXPECT_DOUBLE_EQ(17.0, ExpressionEvaluator::evaluate("(2+3)*3+2"));
    EXPECT_DOUBLE_EQ(2.5, ExpressionEvaluator::evaluate("(3.5+1.5)/2"));
    EXPECT_DOUBLE_EQ(0.5, ExpressionEvaluator::evaluate("1/(1+1)"));
}

// 测试浮点数精度（允许微小误差）
TEST(ExpressionEvaluatorTest, HandlesFloatingPointPrecision) {
    double result = ExpressionEvaluator::evaluate("1.0/3.0");
    EXPECT_NEAR(0.3333333333333333, result, 1e-12);

    result = ExpressionEvaluator::evaluate("0.1+0.2");
    EXPECT_NEAR(0.3, result, 1e-12);
}