# CMakeList.txt: MMOpengl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
include_directories(../../../3rdpart/includes)
include_directories(../../../3rdpart/glfw/include)
include_directories(../../../3rdpart/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#遍历当前文件夹下所有cpp和c
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES_MAIN)

link_directories("../../../3rdpart/glfw/lib")
add_executable (MMOpengl ${SOURCES_MAIN} ${GLAD_SOURCES})
target_link_libraries(MMOpengl MMGL)
