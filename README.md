# Qt Calculator - 跨平台计算器应用程序

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Qt6](https://img.shields.io/badge/Qt-6-green.svg)](https://www.qt.io/)

一个使用Qt6和C++17构建的跨平台桌面计算器应用程序，支持基本数学运算、括号运算和浮点数计算。

## ✨ 功能特性

- **基本运算**：加(+)、减(-)、乘(*)、除(/)四则运算
- **括号支持**：完整括号运算，遵循数学优先级
- **浮点数计算**：支持小数运算，结果精度保留10位有效数字
- **表达式求值**：使用调度场算法（Shunting-yard algorithm）准确求值
- **用户界面**：
  - 清晰的表达式显示框
  - 数字按钮0-9和小数点按钮
  - 操作符按钮和括号按钮
  - 清除(C)和等于(=)按钮
  - 错误提示对话框
- **跨平台支持**：Windows、macOS、Linux
- **单元测试**：完整的表达式求值器测试覆盖

## 🚀 快速开始

### 前提条件

- **CMake 3.16+**
- **vcpkg**（包管理器，需要设置`VCPKG_ROOT`环境变量）
- **C++17兼容编译器**
  - Windows: MSVC
  - macOS: Clang
  - Linux: GCC

### 构建项目

项目使用CMake预设配置，简化跨平台构建：

```bash
# 1. 配置项目（使用macOS预设）
cmake --preset macos-debug

# 2. 构建项目
cmake --build --preset debug-macos

# 3. 运行应用程序
./build/macos-debug/calculator
```

#### 其他平台预设

- **Windows**: `windows-debug`, `windows-release`
- **macOS**: `macos-debug`, `macos-release`
- **Linux**: `linux-debug`, `linux-release`

### 手动构建（不使用预设）

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

## 🧪 运行测试

项目包含使用Google Test框架的单元测试：

```bash
# 构建测试目标
cmake --build --preset debug-macos --target calculator_tests

# 运行测试
./build/macos-debug/bin/tests/calculator_tests

# 使用简洁输出格式
./build/macos-debug/bin/tests/calculator_tests --gtest_brief=1
```

### 测试覆盖范围

- 空表达式和单数字
- 基本运算符和优先级
- 括号运算和复杂表达式
- 小数和浮点精度
- 错误处理（无效字符、括号不匹配、除零等）
- 空格处理

## 🏗️ 项目架构

```
calculator_qt/
├── CMakeLists.txt           # 主构建配置
├── CMakePresets.json        # 跨平台构建预设
├── vcpkg.json              # 依赖管理
├── src/
│   ├── calculator.cpp      # 应用程序入口点
│   ├── calculatorwindow.cpp # Qt主窗口实现
│   └── evaluator.cpp       # 表达式求值器核心逻辑
├── include/
│   ├── calculatorwindow.h  # 主窗口类声明
│   └── evaluator.h         # 表达式求值器类声明
├── tests/
│   └── evaluator_test.cpp  # 单元测试
└── build/                  # 构建输出目录
```

### 核心组件

1. **ExpressionEvaluator类**
   - 实现调度场算法（Shunting-yard algorithm）
   - 中缀表达式转后缀表达式
   - 支持运算符优先级和括号运算
   - 静态方法设计，无状态依赖

2. **CalculatorWindow类**
   - 继承自`QMainWindow`
   - 使用Qt信号槽机制处理用户交互
   - 管理UI布局和组件状态

3. **Main Application**
   - 初始化Qt应用程序
   - 创建和显示主窗口

### 设计模式

- **Model-View分离**：UI（CalculatorWindow）与业务逻辑（ExpressionEvaluator）分离
- **静态工具类**：ExpressionEvaluator使用纯静态方法
- **Qt事件驱动**：基于信号槽的响应式UI

## 🔧 开发指南

### 扩展新功能

#### 添加新运算符
1. 修改`ExpressionEvaluator::isOperator()`识别新运算符
2. 更新`ExpressionEvaluator::precedence()`设置运算符优先级
3. 实现`ExpressionEvaluator::applyOperator()`中的运算逻辑

#### 添加UI组件
1. 在`calculatorwindow.h`中添加组件指针
2. 在`CalculatorWindow::setupUI()`中创建和配置组件
3. 连接信号到相应的槽函数
4. 添加到布局管理器

### 开发工具配置

- **代码格式化**：`.clang-format`配置文件
- **静态分析**：`.clang-tidy`配置规则
- **代码补全**：`.clangd`语言服务器配置
- **IDE配置**：`.vscode/`目录包含VS Code设置

### 调试

- 使用LLDB进行调试（VS Code配置已提供）
- 编译命令数据库位于`build/[preset]/compile_commands.json`
- clangd已配置详细日志记录

## 📝 使用示例

### 支持的表达式

```bash
# 基本运算
"2+3"                     # => 5
"10-4"                    # => 6
"3*4"                     # => 12
"8/2"                     # => 4

# 优先级运算
"1+2*3"                   # => 7 (乘法优先)
"10-6/2"                  # => 7 (除法优先)

# 括号运算
"(1+2)*3"                 # => 9
"(10-4)/2"                # => 3

# 小数运算
"0.5+1.0"                 # => 1.5
"5.0/2.5"                 # => 2.0

# 空格支持
" 2 + 3 "                 # => 5
"( 2 + 3 ) * 2"           # => 10
```

### 错误处理

```bash
# 无效字符
"2 & 3"                   # => 错误：Invalid character
"abc"                     # => 错误：Invalid character

# 括号不匹配
"(2+3"                    # => 错误：Mismatched parentheses
"2+3)"                    # => 错误：Mismatched parentheses

# 除零错误
"5/0"                     # => 错误：Division by zero

# 无效数字格式
"2.3.4"                   # => 错误：Invalid number format
"."                       # => 错误：Invalid number format
```

## 🔍 已知限制

- 不支持负数（一元负号），如"-5"或"3*-4"
- 不支持函数调用（sin、cos、log等）
- 仅支持基本四则运算符
- 无键盘快捷键，仅支持鼠标点击

## 🤝 贡献指南

1. Fork项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开Pull Request

### 开发规范

- 遵循现有的代码风格（`.clang-format`）
- 为新增功能编写单元测试
- 更新相关文档
- 确保所有测试通过

## 📄 许可证

本项目基于MIT许可证 - 查看[LICENSE](LICENSE)文件了解详情。

## 🙏 致谢

- [Qt](https://www.qt.io/) - 跨平台应用程序框架
- [vcpkg](https://vcpkg.io/) - C++包管理器
- [Google Test](https://github.com/google/googletest) - 单元测试框架
- 所有贡献者和使用者

---

**提示**：更详细的开发指南请查看[CLAUDE.md](CLAUDE.md)文件。