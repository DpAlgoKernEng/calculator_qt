/**
 * @file evaluator.cpp
 * @brief 数学表达式求值器实现
 */

#include "evaluator.h"
#include <cmath>
#include <sstream>
#include <iostream>

using namespace std;

namespace {
    bool isValidNumber(const string& str) {
        if (str.empty()) return false;

        int dotCount = 0;
        bool hasDigit = false;

        for (size_t i = 0; i < str.length(); ++i) {
            char ch = str[i];
            if (isdigit(ch)) {
                hasDigit = true;
            } else if (ch == '.') {
                dotCount++;
                if (dotCount > 1) {
                    return false; // 多个小数点
                }
            } else {
                return false; // 无效字符
            }
        }

        return hasDigit; // 必须至少有一个数字
    }

}

double ExpressionEvaluator::evaluate(const string& expression) {
    if (expression.empty()) {
        return 0.0;
    }
    
    try {
        vector<string> postfix = infixToPostfix(expression);
        stack<double> values;
        
        for (const string& token : postfix) {
            if (token.length() == 1 && isOperator(token[0])) {
                if (values.size() < 2) {
                    throw invalid_argument("Invalid expression: insufficient operands");
                }
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                values.push(applyOperator(a, b, token[0]));
            } else {
                // 数字
                double val = stod(token);
                values.push(val);
            }
        }
        
        if (values.size() != 1) {
            throw invalid_argument("Invalid expression: too many operands");
        }
        
        return values.top();
    } catch (const exception& e) {
        throw invalid_argument(string("Evaluation error: ") + e.what());
    }
}

int ExpressionEvaluator::precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

bool ExpressionEvaluator::isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

double ExpressionEvaluator::applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0.0) {
                throw invalid_argument("Division by zero");
            }
            return a / b;
        default:
            throw invalid_argument("Unknown operator");
    }
}

vector<string> ExpressionEvaluator::infixToPostfix(const string& expression) {
    stack<char> ops;
    vector<string> output;
    string number;
    
    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];
        
        if (isspace(ch)) {
            if (!number.empty()) {
                if (!isValidNumber(number)) {
                    throw invalid_argument("Invalid number format: " + number);
                }
                output.push_back(number);
                number.clear();
            }
            continue;
        }
        
        if (isdigit(ch) || ch == '.') {
            number += ch;
        } else {
            if (!number.empty()) {
                if (!isValidNumber(number)) {
                    throw invalid_argument("Invalid number format: " + number);
                }
                output.push_back(number);
                number.clear();
            }
            
            if (ch == '(') {
                ops.push(ch);
            } else if (ch == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    output.push_back(string(1, ops.top()));
                    ops.pop();
                }
                if (ops.empty()) {
                    throw invalid_argument("Mismatched parentheses");
                }
                ops.pop(); // 弹出 '('
            } else if (isOperator(ch)) {
                while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(ch)) {
                    output.push_back(string(1, ops.top()));
                    ops.pop();
                }
                ops.push(ch);
            } else {
                throw invalid_argument("Invalid character in expression");
            }
        }
    }
    
    if (!number.empty()) {
        if (!isValidNumber(number)) {
            throw invalid_argument("Invalid number format: " + number);
        }
        output.push_back(number);
    }
    
    while (!ops.empty()) {
        if (ops.top() == '(') {
            throw invalid_argument("Mismatched parentheses");
        }
        output.push_back(string(1, ops.top()));
        ops.pop();
    }
    
    return output;
}