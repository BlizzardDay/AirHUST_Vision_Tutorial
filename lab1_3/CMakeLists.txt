cmake_minimum_required(VERSION 3.10)
project(lab1_3)

set(CMAKE_CXX_STANDARD 11)

# 查找OpenCV
find_package(OpenCV REQUIRED)

# 包含头文件
include_directories(${OpenCV_INCLUDE_DIRS})

# 创建可执行文件
add_executable(lab1_3 src/main.cpp)

# 链接OpenCV库
target_link_libraries(lab1_3 ${OpenCV_LIBS})