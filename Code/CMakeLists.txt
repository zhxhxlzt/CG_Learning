cmake_minimum_required(VERSION 3.10)

project(launcher)

add_subdirectory(glfw)
add_subdirectory(assimp)
add_subdirectory(glad_module)
add_subdirectory(engine)
add_subdirectory(assets)


target_include_directories(engine PUBLIC third_party)
file(GLOB header_files launcher/Public/*.h)
source_group("Header Files" FILES ${header_files})

file(GLOB cpp_files launcher/Private/*.cpp)

add_executable(launcher ${cpp_files} ${header_files})
target_include_directories(launcher PUBLIC launcher/Public third_party)
list(APPEND EXTRA_LIBS glfw assimp glad_module engine assets)
target_link_libraries(launcher PUBLIC ${EXTRA_LIBS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
