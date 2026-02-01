// calculatorwindow.cpp: 实现计算器应用程序的主窗口逻辑
// 包含计算器UI的构建、按钮交互、表达式求值等核心功能

#include "calculatorwindow.h"           // 计算器主窗口类声明
#include "evaluator.h"            // 表达式求值器
#include <QMessageBox>            // 消息弹窗
#include <QIntValidator>          // 整数输入验证器
#include <QGridLayout>            // 网格布局
#include <QGroupBox>              // 分组框
#include <QFont>                  // 字体

/**
 * @brief CalculatorWindow类的构造函数
 * @param parent 父窗口指针，默认为nullptr
 * 
 * 构造函数初始化计算器状态并设置用户界面：
 * 1. 初始化成员变量（表达式显示、按钮等）
 * 2. 调用setupUI()创建计算器界面
 * 3. 连接信号槽
 */
CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QMainWindow(parent)         // 调用基类QMainWindow构造函数
{
    setupUI();                    // 创建和设置计算器界面
}

/**
 * @brief MainWindow类的析构函数
 * 
 * 目前没有需要手动释放的资源，因为Qt的对象树会自动管理子对象的内存
 */
CalculatorWindow::~CalculatorWindow()
{
}

/**
 * @brief 设置游戏用户界面
 * 
 * 创建游戏的所有UI组件，包括：
 * 1. 游戏设置区域（范围选择、新游戏按钮）
 * 2. 游戏区域（输入框、提交按钮、提示标签、尝试次数显示）
 * 3. 信号槽连接，处理用户交互
 * 4. 设置初始焦点到输入框
 */
void CalculatorWindow::setupUI()
{
    // 创建中心部件，作为主窗口的内容容器
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建垂直布局作为主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // ==================== 表达式显示区域 ====================
    expressionDisplay = new QLineEdit(centralWidget);
    expressionDisplay->setReadOnly(true);
    expressionDisplay->setAlignment(Qt::AlignRight);
    expressionDisplay->setFont(QFont("Arial", 16));
    mainLayout->addWidget(expressionDisplay);

    // ==================== 按钮网格区域 ====================
    QGridLayout *buttonLayout = new QGridLayout();
    
    // 数字按钮 0-9
    for (int i = 0; i < 10; ++i) {
        digitButtons[i] = new QPushButton(QString::number(i), centralWidget);
        digitButtons[i]->setFont(QFont("Arial", 12));
        connect(digitButtons[i], &QPushButton::clicked, this, [this, i]() {
            onDigitClicked(QString::number(i));
        });
    }
    
    // 操作符按钮
    addButton = new QPushButton("+", centralWidget);
    subtractButton = new QPushButton("-", centralWidget);
    multiplyButton = new QPushButton("*", centralWidget);
    divideButton = new QPushButton("/", centralWidget);
    leftParenButton = new QPushButton("(", centralWidget);
    rightParenButton = new QPushButton(")", centralWidget);
    clearButton = new QPushButton("C", centralWidget);
    equalsButton = new QPushButton("=", centralWidget);
    
    // 设置按钮字体
    QFont operatorFont("Arial", 12);
    addButton->setFont(operatorFont);
    subtractButton->setFont(operatorFont);
    multiplyButton->setFont(operatorFont);
    divideButton->setFont(operatorFont);
    leftParenButton->setFont(operatorFont);
    rightParenButton->setFont(operatorFont);
    clearButton->setFont(operatorFont);
    equalsButton->setFont(operatorFont);
    
    // 连接操作符按钮信号
    connect(addButton, &QPushButton::clicked, this, [this]() { onOperatorClicked("+"); });
    connect(subtractButton, &QPushButton::clicked, this, [this]() { onOperatorClicked("-"); });
    connect(multiplyButton, &QPushButton::clicked, this, [this]() { onOperatorClicked("*"); });
    connect(divideButton, &QPushButton::clicked, this, [this]() { onOperatorClicked("/"); });
    connect(leftParenButton, &QPushButton::clicked, this, [this]() { onParenthesisClicked("("); });
    connect(rightParenButton, &QPushButton::clicked, this, [this]() { onParenthesisClicked(")"); });
    connect(clearButton, &QPushButton::clicked, this, &CalculatorWindow::clearExpression);
    connect(equalsButton, &QPushButton::clicked, this, &CalculatorWindow::evaluateExpression);
    
    // 布局按钮
    // 第一行：7 8 9 + (
    buttonLayout->addWidget(digitButtons[7], 0, 0);
    buttonLayout->addWidget(digitButtons[8], 0, 1);
    buttonLayout->addWidget(digitButtons[9], 0, 2);
    buttonLayout->addWidget(addButton, 0, 3);
    buttonLayout->addWidget(leftParenButton, 0, 4);
    
    // 第二行：4 5 6 - )
    buttonLayout->addWidget(digitButtons[4], 1, 0);
    buttonLayout->addWidget(digitButtons[5], 1, 1);
    buttonLayout->addWidget(digitButtons[6], 1, 2);
    buttonLayout->addWidget(subtractButton, 1, 3);
    buttonLayout->addWidget(rightParenButton, 1, 4);
    
    // 第三行：1 2 3 * C
    buttonLayout->addWidget(digitButtons[1], 2, 0);
    buttonLayout->addWidget(digitButtons[2], 2, 1);
    buttonLayout->addWidget(digitButtons[3], 2, 2);
    buttonLayout->addWidget(multiplyButton, 2, 3);
    buttonLayout->addWidget(clearButton, 2, 4);
    
    // 第四行：0 . = / (空)
    buttonLayout->addWidget(digitButtons[0], 3, 0);
    buttonLayout->addWidget(divideButton, 3, 3);
    buttonLayout->addWidget(equalsButton, 3, 4);
    
    // 将按钮布局添加到主布局
    mainLayout->addLayout(buttonLayout);
    
    // 设置窗口大小
    resize(400, 300);
}

/**
 * @brief 生成随机秘密数字
 * 
 * 使用C++11的随机数库生成指定范围内的随机整数：
 * 1. 使用std::random_device获取真随机种子
 * 2. 使用std::mt19937作为随机数引擎
 * 3. 使用std::uniform_int_distribution生成均匀分布的整数
 * 4. 重置游戏状态（尝试次数、提示信息等）
 */
void CalculatorWindow::onDigitClicked(const QString &digit)
{
    QString current = expressionDisplay->text();
    expressionDisplay->setText(current + digit);
}

void CalculatorWindow::onOperatorClicked(const QString &op)
{
    QString current = expressionDisplay->text();
    expressionDisplay->setText(current + op);
}

void CalculatorWindow::onParenthesisClicked(const QString &parenthesis)
{
    QString current = expressionDisplay->text();
    expressionDisplay->setText(current + parenthesis);
}

void CalculatorWindow::clearExpression()
{
    expressionDisplay->clear();
}

void CalculatorWindow::evaluateExpression()
{
    QString expr = expressionDisplay->text();
    if (expr.isEmpty()) {
        return;
    }
    
    try {
        double result = ExpressionEvaluator::evaluate(expr.toStdString());
        expressionDisplay->setText(QString::number(result, 'g', 10));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "计算错误", QString("表达式错误: %1").arg(e.what()));
    }
}