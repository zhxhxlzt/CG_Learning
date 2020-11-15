
file(GLOB header_files ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
source_group("Header Files" FILES ${header_files})


file(GLOB cpp_files ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

file(GLOB_RECURSE shader_files 
${CMAKE_CURRENT_SOURCE_DIR}/ShaderSource/*.vert 
${CMAKE_CURRENT_SOURCE_DIR}/ShaderSource/*.frag)
source_group("Shader Files" FILES ${shader_files})

file(GLOB_RECURSE tex_files
${CMAKE_CURRENT_SOURCE_DIR}/TexSource/*.*
)
source_group("Tex Files" FILES ${tex_files})

add_library(assets ${cpp_files} ${header_files} ${shader_files} ${tex_files})

target_include_directories(assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
