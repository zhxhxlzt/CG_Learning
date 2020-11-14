cmake_minimum_required(VERSION 3.10)

project(Tutorial)

add_subdirectory(glfw)
add_subdirectory(glad_module)
list(APPEND EXTRA_LIBS glfw glad_module)
add_executable(Tutorial main.cpp)
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})
