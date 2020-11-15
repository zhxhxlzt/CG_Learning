file(GLOB_RECURSE header_files Public/*.h)
source_group("Header Files" FILES ${header_files})
file(GLOB_RECURSE cpp_files Private/*.cpp)
add_library(engine ${cpp_files} ${header_files})
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public )

target_link_libraries(engine glad_module assimp)

