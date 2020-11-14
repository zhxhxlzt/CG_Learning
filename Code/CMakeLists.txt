cmake_minimum_required(VERSION 3.10)

#project(launcher)

add_subdirectory(glfw)
add_subdirectory(glad_module)
list(APPEND EXTRA_LIBS glfw glad_module)

file(GLOB header_files main/Public/*.h)
source_group("Header Files" FILES ${header_files})

project(launcher)
file(GLOB cpp_files main/Private/*.cpp)

add_executable(launcher ${cpp_files} ${header_files})
target_link_libraries(launcher PUBLIC ${EXTRA_LIBS})
