# CMakeList.txt: calculator 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.16)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 禁用 vcpkg 的外部包含隔离（解决 type_traits 找不到的问题）
set(VCPKG_USE_EXTERNAL_I OFF)

project ("calculator" VERSION 1.0.0 LANGUAGES CXX)

# 查找 Qt6 库
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 启用自动生成 MOC、UIC 和 RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找 vcpkg 根目录
if(NOT DEFINED VCPKG_ROOT)
    if(DEFINED ENV{VCPKG_ROOT})
        set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE PATH "Vcpkg root directory")
    elseif(CMAKE_TOOLCHAIN_FILE AND EXISTS "${CMAKE_TOOLCHAIN_FILE}")
        # 从工具链文件路径推断 vcpkg 根目录
        get_filename_component(VCPKG_TOOLCHAIN_DIR "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
        get_filename_component(VCPKG_ROOT "${VCPKG_TOOLCHAIN_DIR}/../.." ABSOLUTE)
        set(VCPKG_ROOT "${VCPKG_ROOT}" CACHE PATH "Vcpkg root directory")
    endif()
endif()

# 创建核心求值器库
add_library(evaluator_lib STATIC
    "src/evaluator.cpp"
    "${CMAKE_SOURCE_DIR}/include/evaluator.h"
)

# 添加 include 目录
target_include_directories(evaluator_lib PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 设置 C++ 标准
set_target_properties(evaluator_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 将源代码添加到此项目的可执行文件。
add_executable (calculator WIN32
    "src/calculator.cpp"
    "src/calculatorwindow.cpp"
    "${CMAKE_SOURCE_DIR}/include/calculatorwindow.h"
)

# 链接求值器库到可执行文件
target_link_libraries(calculator PRIVATE evaluator_lib Qt6::Widgets)

# 添加 include 目录
target_include_directories(calculator PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 设置 C++ 标准
set_target_properties(calculator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Windows 平台：使用 windeployqt 自动部署 Qt 运行时文件
if(WIN32)
    # 查找 windeployqt 工具（调试版使用 .bat 脚本，发布版使用 .exe）
    find_program(WINDEPLOYQT_DEBUG 
        NAMES windeployqt.debug.bat
        HINTS "${VCPKG_ROOT}/tools/Qt6/bin"
              "${VCPKG_ROOT}/installed/x64-windows/tools/Qt6/bin"
        DOC "Qt Windows deployment tool (Debug)"
    )
    
    find_program(WINDEPLOYQT_RELEASE 
        NAMES windeployqt windeployqt6
        HINTS "${VCPKG_ROOT}/tools/Qt6/bin"
              "${VCPKG_ROOT}/installed/x64-windows/tools/Qt6/bin"
        DOC "Qt Windows deployment tool (Release)"
    )
    
    if(WINDEPLOYQT_DEBUG AND WINDEPLOYQT_RELEASE)
        message(STATUS "Found windeployqt debug: ${WINDEPLOYQT_DEBUG}")
        message(STATUS "Found windeployqt release: ${WINDEPLOYQT_RELEASE}")
        
        # 使用 windeployqt 自动部署（根据配置选择正确的工具）
        add_custom_command(TARGET calculator POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E echo "Deploying Qt runtime..."
            COMMAND "$<$<CONFIG:Debug>:${WINDEPLOYQT_DEBUG}>$<$<NOT:$<CONFIG:Debug>>:${WINDEPLOYQT_RELEASE}>"
                --no-compiler-runtime
                --no-system-d3d-compiler
                --no-opengl-sw
                --verbose 0
                "$<TARGET_FILE:calculator>"
            COMMENT "Deploying Qt runtime with windeployqt..."
        )
    else()
        message(WARNING "windeployqt not found or incomplete, Qt deployment will not be automatic")
        # 回退到手动复制（如果 vcpkg 可用）
        if(VCPKG_ROOT)
            set(VCPKG_INSTALLED_DIR "${VCPKG_ROOT}/installed/x64-windows")
            
            if(EXISTS "${VCPKG_INSTALLED_DIR}")
                set(VCPKG_DEBUG_BIN_DIR "${VCPKG_INSTALLED_DIR}/debug/bin")
                set(VCPKG_DEBUG_PLUGINS_DIR "${VCPKG_INSTALLED_DIR}/debug/Qt6/plugins")
                set(VCPKG_RELEASE_BIN_DIR "${VCPKG_INSTALLED_DIR}/bin")
                set(VCPKG_RELEASE_PLUGINS_DIR "${VCPKG_INSTALLED_DIR}/Qt6/plugins")
                
                add_custom_command(TARGET calculator POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "$<$<CONFIG:Debug>:${VCPKG_DEBUG_BIN_DIR}/Qt6Cored.dll>$<$<NOT:$<CONFIG:Debug>>:${VCPKG_RELEASE_BIN_DIR}/Qt6Core.dll>"
                        "$<TARGET_FILE_DIR:calculator>"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "$<$<CONFIG:Debug>:${VCPKG_DEBUG_BIN_DIR}/Qt6Guid.dll>$<$<NOT:$<CONFIG:Debug>>:${VCPKG_RELEASE_BIN_DIR}/Qt6Gui.dll>"
                        "$<TARGET_FILE_DIR:calculator>"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "$<$<CONFIG:Debug>:${VCPKG_DEBUG_BIN_DIR}/Qt6Widgetsd.dll>$<$<NOT:$<CONFIG:Debug>>:${VCPKG_RELEASE_BIN_DIR}/Qt6Widgets.dll>"
                        "$<TARGET_FILE_DIR:calculator>"
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        "$<TARGET_FILE_DIR:calculator>/platforms"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "$<$<CONFIG:Debug>:${VCPKG_DEBUG_PLUGINS_DIR}/platforms/qwindowsd.dll>$<$<NOT:$<CONFIG:Debug>>:${VCPKG_RELEASE_PLUGINS_DIR}/platforms/qwindows.dll>"
                        "$<TARGET_FILE_DIR:calculator>/platforms"
                    COMMENT "Manually copying Qt runtime libraries..."
                )
                message(STATUS "Manual Qt DLL copy enabled (fallback)")
            endif()
        endif()
    endif()
endif()

# TODO: 如有需要，请添加测试并安装目标。

# 启用测试
enable_testing()

# 使用 FetchContent 获取 Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
# 设置 gtest 为不构建 samples 和 tests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 添加测试可执行文件
add_executable(calculator_tests
    tests/evaluator_test.cpp
)

# 链接测试目标
target_link_libraries(calculator_tests
    PRIVATE
        gtest_main
        evaluator_lib
)

# 添加 include 目录
target_include_directories(calculator_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 添加测试运行
add_test(NAME calculator_tests COMMAND calculator_tests)

# 设置测试可执行文件输出目录
set_target_properties(calculator_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
)

# 安装配置
install(TARGETS calculator
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
    COMPONENT Runtime
)

# 安装头文件（可选）
install(DIRECTORY include/
    DESTINATION include
    COMPONENT Development
    FILES_MATCHING PATTERN "*.h"
)

# CPack 打包配置
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "calculator")
set(CPACK_PACKAGE_VENDOR "Unknown")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A number guessing game")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")

# 设置组件
set(CPACK_COMPONENTS_ALL Runtime Development)

# Windows 特定配置
if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "calculator")
    set(CPACK_NSIS_PACKAGE_NAME "calculator")
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://example.com")
    set(CPACK_NSIS_CONTACT "unknown@example.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
else()
    set(CPACK_GENERATOR "TGZ;ZIP")
endif()

include(CPack)
