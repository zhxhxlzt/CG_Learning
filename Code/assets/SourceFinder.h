#include <string>

class CSourceFinder
{
public:
	static std::string FindShaderFullPath(std::string relative_shader_path);
	static std::string FindTexFullPath(std::string relative_tex_path);
	static std::string FindModelFullPath(std::string relative_model_path);
};