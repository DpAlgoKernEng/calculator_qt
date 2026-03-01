# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a calculator application built with Qt6 and C++17. It features a graphical user interface for basic arithmetic operations (addition, subtraction, multiplication, division) with parenthesis support. The project uses CMake for build configuration and vcpkg for dependency management.

## Building and Running

### Prerequisites
- CMake 3.16+
- vcpkg (installed and `VCPKG_ROOT` environment variable set)
- Qt6 dependencies (automatically managed by vcpkg)

### Building with CMake Presets
The project uses CMake presets for cross-platform builds. Available presets include:

**Configure presets:**
- `windows-debug`, `windows-release` (MSVC on Windows)
- `linux-debug`, `linux-release` (GCC on Linux)
- `macos-debug`, `macos-release` (Clang on macOS)

**Build presets:**
- `debug-windows`, `release-windows`
- `debug-linux`, `release-linux`
- `debug-macos`, `release-macos`

**Common commands:**

1. Configure with a preset:
   ```bash
   cmake --preset macos-debug
   ```

2. Build with a preset:
   ```bash
   cmake --build --preset debug-macos
   ```

3. Configure and build in one command:
   ```bash
   cmake --preset macos-debug && cmake --build --preset debug-macos
   ```

### Building without Presets
If you prefer not to use presets:

1. Create a build directory:
   ```bash
   mkdir -p build && cd build
   ```

2. Configure with CMake (adjust toolchain path as needed):
   ```bash
   cmake .. -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Debug
   ```

3. Build:
   ```bash
   cmake --build .
   ```

### Running the Application
After building, the executable is located at:
- `build/macos-debug/bin/calculator` (when using preset)
- `build/bin/calculator` (when building without preset)

Run directly:
```bash
./build/macos-debug/bin/calculator
```

### Development Dependencies
vcpkg manages Qt dependencies as specified in `vcpkg.json`:
- `qtbase` (core Qt libraries)
- `qttools` (Qt tools)
- `qtdeclarative` (Qt Declarative module)

## Architecture

### Core Components

1. **Main Application Entry (`src/calculator.cpp`)**
   - Initializes Qt application
   - Creates and displays the main window

2. **Calculator Window (`include/calculatorwindow.h`, `src/calculatorwindow.cpp`)**
   - `CalculatorWindow` class inherits from `QMainWindow`
   - Manages UI layout and user interaction
   - Contains digit buttons (0-9), operator buttons (+, -, *, /), parenthesis buttons, clear, and equals buttons
   - Uses Qt signal-slot mechanism for event handling

3. **Expression Evaluator (`include/evaluator.h`, `src/evaluator.cpp`)**
   - `ExpressionEvaluator` class with static `evaluate()` method
   - Implements Shunting-yard algorithm to parse and evaluate mathematical expressions
   - Supports integer and floating-point arithmetic with operator precedence
   - Throws `std::invalid_argument` on invalid expressions or division by zero

### Key Design Patterns
- **Model-View Separation**: UI (`CalculatorWindow`) is separate from business logic (`ExpressionEvaluator`)
- **Static Utility Class**: `ExpressionEvaluator` uses static methods for pure computational logic
- **Qt Signal-Slot**: Event-driven architecture for UI interactions

## Development Tools and Configuration

### Code Formatting and Linting
- `.clang-format`: Code formatting rules
- `.clang-tidy`: Static analysis configuration
- `.clangd`: Language server configuration with clang-tidy integration

### VS Code Configuration
- `.vscode/settings.json`: Disables C/C++ IntelliSense in favor of clangd
- `.vscode/launch.json`: Debug configuration using LLDB
- clangd arguments point to compile commands in build directory

### Git Submodules
- `third/cmdline_parser/gflags`: Command-line argument parsing library
- `third/lib_manager/vcpkg`: Package manager (though vcpkg is typically installed separately)

### Build Output
- Executable outputs to `bin/` directory within the build tree
- On Windows, automatic Qt deployment via `windeployqt` (or manual DLL copy as fallback)

## Common Tasks

### Adding New UI Components
1. Add widget pointer as private member in `calculatorwindow.h`
2. Create and configure widget in `CalculatorWindow::setupUI()`
3. Connect signals to appropriate slot functions
4. Add to layout for proper positioning

### Extending Expression Evaluation
1. Modify `ExpressionEvaluator::isOperator()` to recognize new operators
2. Update `ExpressionEvaluator::precedence()` for operator priority
3. Implement new operation logic in `ExpressionEvaluator::applyOperator()`

### Debugging
- Use LLDB via VS Code launch configuration
- Compile commands are generated at `build/[preset]/compile_commands.json` for clangd
- Enable verbose logging in clangd via `--log=verbose` (already configured)

### Testing
- Unit tests are available using Google Test framework
- Build tests: `cmake --build --preset debug-macos --target calculator_tests`
- Run tests: `./build/macos-debug/bin/tests/calculator_tests`
- Test source: `tests/evaluator_test.cpp` (covers ExpressionEvaluator)
- Tests are automatically downloaded via CMake FetchContent

### Cross-Platform Considerations
- The project uses Qt abstractions for platform-specific UI
- Windows deployment includes automatic Qt DLL copying
- CMake presets handle compiler differences across platforms