/**
 * @file calculatorwindow.h
 * @brief 计算器应用程序的主窗口类声明
 * 
 * 该文件定义了CalculatorWindow类，继承自QMainWindow，实现计算器的图形界面和逻辑。
 * 包含表达式显示、按钮交互和表达式求值功能。
 */

#ifndef CALCULATORWINDOW_H
#define CALCULATORWINDOW_H

// Qt核心类包含
#include <QMainWindow>      // 主窗口基类
#include <QLineEdit>        // 文本输入框
#include <QPushButton>      // 按钮
#include <QLabel>           // 标签
#include <QSpinBox>         // 数字微调框
#include <QVBoxLayout>      // 垂直布局
#include <QHBoxLayout>      // 水平布局
#include <QMessageBox>      // 消息对话框
#include <QString>          // 字符串类
#include <QFont>            // 字体类

// C++标准库包含
#include <random>           // C++11随机数库

/**
 * @class CalculatorWindow
 * @brief 计算器应用程序的主窗口类
 * 
 * 该类负责：
 * 1. 管理计算器界面和用户交互
 * 2. 维护表达式显示和按钮状态
 * 3. 处理按钮点击和表达式求值
 * 4. 提供基本的数学运算（加减乘除）和括号支持
 * 
 * 继承自QMainWindow，使用Qt的信号槽机制处理事件。
 */
class CalculatorWindow : public QMainWindow
{
    Q_OBJECT  // Qt元对象系统宏，启用信号槽机制和反射

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     */
    CalculatorWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~CalculatorWindow();

private slots:
    /**
     * @brief 处理数字按钮点击的槽函数
     * @param digit 点击的数字字符
     */
    void onDigitClicked(const QString &digit);
    
    /**
     * @brief 处理操作符按钮点击的槽函数
     * @param op 点击的操作符字符
     */
    void onOperatorClicked(const QString &op);
    
    /**
     * @brief 处理括号按钮点击的槽函数
     * @param parenthesis 点击的括号字符
     */
    void onParenthesisClicked(const QString &parenthesis);
    
    /**
     * @brief 清除表达式
     */
    void clearExpression();
    
    /**
     * @brief 计算结果
     */
    void evaluateExpression();

private:
    /**
     * @brief 设置用户界面
     * 
     * 创建和布局所有UI组件，连接信号槽。
     */
    void setupUI();

    // ==================== UI组件指针 ====================
    QLineEdit *expressionDisplay;  ///< 表达式显示框
    QPushButton *digitButtons[10]; ///< 数字按钮0-9
    QPushButton *addButton;        ///< 加号按钮
    QPushButton *subtractButton;   ///< 减号按钮
    QPushButton *multiplyButton;   ///< 乘号按钮
    QPushButton *divideButton;     ///< 除号按钮
    QPushButton *leftParenButton;  ///< 左括号按钮
    QPushButton *rightParenButton; ///< 右括号按钮
    QPushButton *clearButton;      ///< 清除按钮
    QPushButton *equalsButton;     ///< 等号按钮
};

#endif // CALCULATORWINDOW_H