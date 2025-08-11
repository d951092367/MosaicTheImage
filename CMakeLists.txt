# 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MosaicTheImage)

# 启用自动 MOC 处理
set(CMAKE_AUTOMOC ON)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 Qt5 模块
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# 查找 OpenCV 模块
find_package(OpenCV REQUIRED)

# 包含 Qt 和 OpenCV 的头文件目录
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

# 包含自定义头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 包含生成的 UI 文件目录
include_directories(${CMAKE_BINARY_DIR})

# 添加 Qt 的 UI 文件和资源文件
qt5_wrap_ui(MOSAIC_UI_FILES ${CMAKE_SOURCE_DIR}/resources/MosaicTheImage.ui)
qt5_add_resources(MOSAIC_QRC_FILES ${CMAKE_SOURCE_DIR}/resources/MosaicTheImage.qrc)

# 添加源文件
set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/MosaicTheImage.cpp
    ${CMAKE_SOURCE_DIR}/src/CanvasLabel.cpp
)

# 添加头文件
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/include/MosaicTheImage.h
    ${CMAKE_SOURCE_DIR}/include/CanvasLabel.h
    ${CMAKE_SOURCE_DIR}/include/Tools.hpp
)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${MOSAIC_UI_FILES} ${MOSAIC_QRC_FILES})

# 链接 Qt 和 OpenCV 库
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} ${OpenCV_LIBS})