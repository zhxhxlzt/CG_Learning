
file(GLOB header_files *.h)
source_group("Header Files" FILES ${header_files})
file(GLOB cpp_files *.cpp)

add_library(Reflection ${cpp_files} ${header_files})
