#include "SourceFinder.h"
#include <direct.h>
#include <stdio.h>


#define ROOT(x) "E:/CG_Learning/Code/assets/"#x"/"

std::string CSourceFinder::FindShaderFullPath(std::string relative_shader_path)
{
	return ROOT(ShaderSource) + relative_shader_path;
}

std::string CSourceFinder::FindTexFullPath(std::string relative_tex_path)
{
	return ROOT(TexSource) + relative_tex_path;
}

std::string CSourceFinder::FindModelFullPath(std::string relative_model_path)
{
	return ROOT(ModelSource) + relative_model_path;
}
