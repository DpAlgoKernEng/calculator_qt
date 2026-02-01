/**
 * @file mainwindow.h
 * @brief 猜数字游戏的主窗口类声明
 * 
 * 该文件定义了MainWindow类，继承自QMainWindow，实现猜数字游戏的图形界面和逻辑。
 * 包含游戏状态管理、UI组件声明和用户交互处理函数。
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt核心类包含
#include <QMainWindow>      // 主窗口基类
#include <QLineEdit>        // 文本输入框
#include <QPushButton>      // 按钮
#include <QLabel>           // 标签
#include <QSpinBox>         // 数字微调框
#include <QVBoxLayout>      // 垂直布局
#include <QHBoxLayout>      // 水平布局
#include <QMessageBox>      // 消息对话框

// C++标准库包含
#include <random>           // C++11随机数库

/**
 * @class MainWindow
 * @brief 猜数字游戏的主窗口类
 * 
 * 该类负责：
 * 1. 管理游戏界面和用户交互
 * 2. 维护游戏状态（秘密数字、尝试次数、范围等）
 * 3. 处理猜测验证和游戏逻辑
 * 4. 提供游戏设置和新游戏功能
 * 
 * 继承自QMainWindow，使用Qt的信号槽机制处理事件。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT  // Qt元对象系统宏，启用信号槽机制和反射

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为nullptr
     */
    MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~MainWindow();

private slots:
    /**
     * @brief 检查用户猜测的槽函数
     * 
     * 当用户点击"提交猜测"按钮或按Enter键时调用，
     * 验证输入并给出反馈。
     */
    void checkGuess();
    
    /**
     * @brief 开始新游戏的槽函数
     * 
     * 当用户点击"新游戏"按钮或更改游戏范围时调用，
     * 重置游戏状态并生成新的随机数字。
     */
    void startNewGame();

private:
    /**
     * @brief 设置用户界面
     * 
     * 创建和布局所有UI组件，连接信号槽。
     */
    void setupUI();
    
    /**
     * @brief 生成随机秘密数字
     * 
     * 使用C++11随机数库生成指定范围内的随机整数，
     * 并重置游戏状态。
     */
    void generateRandomNumber();

    // ==================== UI组件指针 ====================
    QLineEdit *guessInput;      ///< 猜测输入框，用户在此输入猜测数字
    QPushButton *submitButton;  ///< 提交猜测按钮
    QPushButton *newGameButton; ///< 新游戏按钮
    QLabel *hintLabel;          ///< 提示标签，显示游戏提示信息
    QLabel *attemptsLabel;      ///< 尝试次数标签，显示剩余尝试次数
    QSpinBox *rangeSpinBox;     ///< 范围选择微调框，设置数字范围上限

    // ==================== 游戏状态变量 ====================
    int secretNumber;  ///< 当前游戏的秘密数字，用户需要猜测的数字
    int attemptsLeft;  ///< 剩余尝试次数，每次错误猜测减1
    int maxAttempts;   ///< 最大尝试次数，默认10次
    int minRange;      ///< 最小数字范围，固定为1
    int maxRange;      ///< 最大数字范围，用户可通过rangeSpinBox调整
};

#endif // MAINWINDOW_H