
cmake_minimum_required (VERSION 3.8)
include_directories(../../../3rdpart/glfw/include)
include_directories(../../../3rdpart/glad/include)

file(GLOB_RECURSE GLAD_SOURCES "../../../3rdpart/glad/src/*.c")
include_directories(./)
file(GLOB_RECURSE MY_SOURCES "./*.cpp")

link_directories("../../../3rdpart/glfw/lib")

add_library (MMGL ${MY_SOURCES} ${GLAD_SOURCES} "mmprogram.cpp")
target_link_libraries(MMGL glfw3)
# TODO: 如有需要，请添加测试并安装目标。
