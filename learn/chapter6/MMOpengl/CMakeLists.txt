# CMakeList.txt: MMOpengl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
include_directories(../../../3rdpart/glfw/include)
include_directories(../../../3rdpart/glad/include)

link_directories("../../../3rdpart/glfw/lib")

add_executable (MMOpengl "MMOpengl.cpp" "MMOpengl.h" ${GLAD_SOURCES})
target_link_libraries(MMOpengl MMGL)
# TODO: 如有需要，请添加测试并安装目标。
