#include "ShaderSource.h"
#include <direct.h>
#include <stdio.h>

std::string CShaderSource::FindShaderFullPath(std::string relative_path)
{
	return "E:/CG_Learning/Code/shader/ShaderSource/" + relative_path;
}