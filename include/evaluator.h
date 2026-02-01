/**
 * @file evaluator.h
 * @brief 数学表达式求值器
 * 
 * 该文件定义了ExpressionEvaluator类，用于解析和求值数学表达式。
 * 支持加减乘除和括号运算，遵循运算符优先级。
 */

#pragma once

#include <string>
#include <stack>
#include <vector>
#include <cctype>
#include <stdexcept>

/**
 * @class ExpressionEvaluator
 * @brief 数学表达式求值器类
 * 
 * 使用调度场算法（Shunting-yard algorithm）将中缀表达式转换为后缀表达式，
 * 然后求值。支持整数和浮点数运算。
 */
class ExpressionEvaluator {
public:
    /**
     * @brief 求值数学表达式
     * @param expression 数学表达式字符串
     * @return 计算结果（双精度浮点数）
     * @throws std::invalid_argument 如果表达式无效
     */
    static double evaluate(const std::string& expression);

private:
    /**
     * @brief 获取运算符优先级
     * @param op 运算符字符
     * @return 优先级数值（数值越高优先级越高）
     */
    static int precedence(char op);

    /**
     * @brief 判断字符是否为运算符
     * @param ch 字符
     * @return 如果是运算符返回true
     */
    static bool isOperator(char ch);

    /**
     * @brief 应用运算符到两个操作数
     * @param a 左操作数
     * @param b 右操作数
     * @param op 运算符
     * @return 运算结果
     * @throws std::invalid_argument 如果除零或无效运算符
     */
    static double applyOperator(double a, double b, char op);

    /**
     * @brief 将中缀表达式转换为后缀表达式（逆波兰表示法）
     * @param expression 中缀表达式
     * @return 后缀表达式标记向量
     */
    static std::vector<std::string> infixToPostfix(const std::string& expression);
};