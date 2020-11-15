cmake_minimum_required(VERSION 3.10)

project(launcher)

add_subdirectory(glfw)
add_subdirectory(glad_module)
add_subdirectory(engine)
add_subdirectory(shader)

file(GLOB header_files launcher/Public/*.h)
source_group("Header Files" FILES ${header_files})

file(GLOB cpp_files launcher/Private/*.cpp)

add_executable(launcher ${cpp_files} ${header_files})

list(APPEND EXTRA_LIBS glfw glad_module engine shader)
target_link_libraries(launcher PUBLIC ${EXTRA_LIBS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
