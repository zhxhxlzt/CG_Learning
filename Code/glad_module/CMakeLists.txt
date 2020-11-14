file(GLOB_RECURSE header_files *.h *.hpp)
source_group("Header Files" FILES ${header_files})

add_library(glad_module src/glad.c ${header_files})

target_include_directories(glad_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)