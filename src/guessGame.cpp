// guessGame.cpp: 定义应用程序的入口点。
// 这是猜数字游戏的主程序文件，负责初始化Qt应用程序和主窗口。
//

#include <QApplication>     // 包含Qt应用程序类
#include "mainwindow.h"     // 包含主窗口类头文件

/**
 * @brief 应用程序的主函数，猜数字游戏的入口点
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出码
 * 
 * 该函数执行以下主要步骤：
 * 1. 创建QApplication实例，管理应用程序的控制流和设置
 * 2. 创建MainWindow实例，即游戏的主窗口
 * 3. 设置窗口标题、大小等属性
 * 4. 显示主窗口
 * 5. 进入Qt事件循环，等待用户交互
 */
auto main(int argc, char *argv[]) -> int
{
    // 创建Qt应用程序实例，管理整个应用程序的生命周期
    // argc和argv参数允许从命令行接收参数
    QApplication app(argc, argv);
    
    // 创建游戏主窗口实例
    MainWindow window;
    
    // 设置窗口标题，显示游戏名称和使用的Qt版本
    window.setWindowTitle("猜数字游戏 - Qt6版");
    
    // 设置窗口初始大小：宽度500像素，高度300像素
    window.resize(500, 300);
    
    // 显示主窗口，使其可见并可交互
    window.show();
    
    // 进入Qt事件循环，等待并处理用户事件（如鼠标点击、键盘输入等）
    // 当用户关闭窗口时，事件循环结束，程序退出
    return app.exec();
}
