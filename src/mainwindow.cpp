// mainwindow.cpp: 实现猜数字游戏的主窗口逻辑
// 包含游戏UI的构建、随机数生成、猜测验证等核心功能

#include "mainwindow.h"           // 主窗口类声明
#include <QMessageBox>            // 消息弹窗
#include <QIntValidator>          // 整数输入验证器
#include <QGridLayout>            // 网格布局
#include <QGroupBox>              // 分组框

/**
 * @brief MainWindow类的构造函数
 * @param parent 父窗口指针，默认为nullptr
 * 
 * 构造函数初始化游戏状态并设置用户界面：
 * 1. 初始化成员变量（秘密数字、尝试次数、范围等）
 * 2. 调用setupUI()创建游戏界面
 * 3. 调用generateRandomNumber()生成第一个随机数
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)         // 调用基类QMainWindow构造函数
    , secretNumber(0)             // 初始化秘密数字为0（后续会重新生成）
    , attemptsLeft(10)            // 初始化剩余尝试次数为10
    , maxAttempts(10)             // 初始化最大尝试次数为10
    , minRange(1)                 // 初始化最小范围为1
    , maxRange(100)               // 初始化最大范围为100
{
    setupUI();                    // 创建和设置用户界面
    generateRandomNumber();       // 生成第一个随机数字
}

/**
 * @brief MainWindow类的析构函数
 * 
 * 目前没有需要手动释放的资源，因为Qt的对象树会自动管理子对象的内存
 */
MainWindow::~MainWindow()
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
void MainWindow::setupUI()
{
    // 创建中心部件，作为主窗口的内容容器
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建垂直布局作为主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // ==================== 游戏设置区域 ====================
    // 创建"游戏设置"分组框
    QGroupBox *rangeGroup = new QGroupBox("游戏设置", centralWidget);
    QHBoxLayout *rangeLayout = new QHBoxLayout(rangeGroup);
    
    // 添加范围标签
    rangeLayout->addWidget(new QLabel("范围:"));
    
    // 创建范围选择微调框，允许选择1-1000的范围
    rangeSpinBox = new QSpinBox(rangeGroup);
    rangeSpinBox->setRange(1, 1000);      // 设置可选范围
    rangeSpinBox->setValue(100);          // 设置默认值为100
    rangeSpinBox->setSuffix(" 以内");     // 添加单位后缀
    rangeLayout->addWidget(rangeSpinBox);
    
    // 创建"新游戏"按钮
    newGameButton = new QPushButton("新游戏", rangeGroup);
    rangeLayout->addWidget(newGameButton);
    
    // 添加伸缩空间，使组件靠左对齐
    rangeLayout->addStretch();
    
    // 将设置分组框添加到主布局
    mainLayout->addWidget(rangeGroup);

    // ==================== 游戏区域 ====================
    // 创建"猜数字游戏"分组框
    QGroupBox *gameGroup = new QGroupBox("猜数字游戏", centralWidget);
    QGridLayout *gameLayout = new QGridLayout(gameGroup);

    // 第一行：输入标签和输入框
    gameLayout->addWidget(new QLabel("输入猜测:"), 0, 0);
    guessInput = new QLineEdit(gameGroup);
    // 设置输入验证器，只允许输入1-1000的整数
    guessInput->setValidator(new QIntValidator(1, 1000, guessInput));
    gameLayout->addWidget(guessInput, 0, 1);
    
    // 第一行：提交按钮
    submitButton = new QPushButton("提交猜测", gameGroup);
    gameLayout->addWidget(submitButton, 0, 2);

    // 第二行：提示标签（跨3列）
    hintLabel = new QLabel("猜一个 1 到 100 之间的数字。", gameGroup);
    hintLabel->setAlignment(Qt::AlignCenter);  // 文字居中对齐
    gameLayout->addWidget(hintLabel, 1, 0, 1, 3);

    // 第三行：尝试次数标签（跨3列）
    attemptsLabel = new QLabel("剩余尝试次数: 10", gameGroup);
    attemptsLabel->setAlignment(Qt::AlignCenter);  // 文字居中对齐
    gameLayout->addWidget(attemptsLabel, 2, 0, 1, 3);

    // 将游戏分组框添加到主布局
    mainLayout->addWidget(gameGroup);
    
    // 添加伸缩空间，使内容靠上显示
    mainLayout->addStretch();

    // ==================== 信号槽连接 ====================
    // 连接提交按钮的点击信号到checkGuess槽函数
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::checkGuess);
    
    // 连接新游戏按钮的点击信号到startNewGame槽函数
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::startNewGame);
    
    // 连接范围选择框的值改变信号到lambda表达式
    // 当范围改变时，更新最大范围并开始新游戏
    connect(rangeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                maxRange = value;     // 更新最大范围
                startNewGame();       // 使用新范围开始游戏
            });

    // ==================== 其他设置 ====================
    // 设置初始焦点到输入框，方便用户直接输入
    guessInput->setFocus();
    
    // 连接输入框的回车键到提交按钮的点击信号
    // 这样用户可以直接按Enter键提交猜测
    guessInput->connect(guessInput, &QLineEdit::returnPressed, submitButton, &QPushButton::click);
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
void MainWindow::generateRandomNumber()
{
    // 创建随机设备，用于生成随机种子
    std::random_device rd;
    
    // 使用Mersenne Twister算法作为随机数引擎，用随机设备初始化
    std::mt19937 gen(rd());
    
    // 创建均匀整数分布，范围从minRange到maxRange
    std::uniform_int_distribution<> distrib(minRange, maxRange);
    
    // 生成秘密数字
    secretNumber = distrib(gen);
    
    // 重置剩余尝试次数为最大值
    attemptsLeft = maxAttempts;
    
    // 更新尝试次数显示
    attemptsLabel->setText(QString("剩余尝试次数: %1").arg(attemptsLeft));
    
    // 更新提示信息，显示当前数字范围
    hintLabel->setText(QString("猜一个 %1 到 %2 之间的数字。").arg(minRange).arg(maxRange));
    
    // 清空输入框内容
    guessInput->clear();
    
    // 将焦点设置回输入框，方便用户继续输入
    guessInput->setFocus();
}

/**
 * @brief 检查用户的猜测
 * 
 * 处理用户提交的猜测数字：
 * 1. 验证输入的有效性
 * 2. 减少剩余尝试次数
 * 3. 判断猜测是否正确
 * 4. 判断是否用尽尝试次数
 * 5. 提供猜测结果的反馈（太大/太小）
 * 6. 处理游戏胜利或失败的情况
 */
void MainWindow::checkGuess()
{
    bool ok;  // 用于检查转换是否成功
    // 将输入框文本转换为整数
    int guess = guessInput->text().toInt(&ok);
    
    // 验证输入：必须转换成功且在有效范围内
    if (!ok || guess < minRange || guess > maxRange) {
        // 显示错误提示
        QMessageBox::warning(this, "输入无效", "请输入有效的数字。");
        
        // 清空输入框并设置焦点，方便用户重新输入
        guessInput->clear();
        guessInput->setFocus();
        return;  // 结束函数，不处理无效输入
    }

    // 减少剩余尝试次数（每次猜测消耗一次机会）
    attemptsLeft--;
    
    // 更新尝试次数显示
    attemptsLabel->setText(QString("剩余尝试次数: %1").arg(attemptsLeft));

    // 情况1：猜测正确
    if (guess == secretNumber) {
        // 显示胜利消息
        QMessageBox::information(this, "恭喜", QString("正确！答案是 %1。").arg(secretNumber));
        
        // 开始新游戏
        startNewGame();
        return;  // 结束函数
    }

    // 情况2：尝试次数用尽
    if (attemptsLeft <= 0) {
        // 显示失败消息
        QMessageBox::information(this, "游戏结束", QString("没有尝试次数了。答案是 %1。").arg(secretNumber));
        
        // 开始新游戏
        startNewGame();
        return;  // 结束函数
    }

    // 情况3：猜测错误但还有尝试次数
    if (guess < secretNumber) {
        // 提示猜测太小
        hintLabel->setText(QString("太小了！猜一个更大的数字。"));
    } else {
        // 提示猜测太大
        hintLabel->setText(QString("太大了！猜一个更小的数字。"));
    }

    // 清空输入框，准备下一次猜测
    guessInput->clear();
    
    // 将焦点设置回输入框
    guessInput->setFocus();
}

/**
 * @brief 开始新游戏
 * 
 * 重置游戏状态，生成新的随机数字
 * 当用户点击"新游戏"按钮、更改范围或完成一轮游戏时调用
 */
void MainWindow::startNewGame()
{
    // 调用generateRandomNumber()生成新的随机数字并重置游戏状态
    generateRandomNumber();
}