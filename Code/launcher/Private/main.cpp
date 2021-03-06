#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "SourceFinder.h"
#include "string"
#define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "sstream"
#include <iostream>
#include <array>
#include <assimp/Importer.hpp>

#include "Model.h"
#include "Camera.h"
#include "AABoundingBox.h"
#include "utils.h"
#include "RenderTexture.h"


class CPostProcessShader : public CShader
{
public:
	CPostProcessShader(const char* fragPath);
	CPostProcessShader() : CPostProcessShader(CSourceFinder::FindShaderFullPath("post.frag").data()) {}
};



ostream& operator<<(ostream& os, glm::vec3 val)
{
	os << "(" << val.x << ", " << val.y << ", " << val.z << ")";
	return os;
}

void TestAABB()
{
	std::vector<glm::vec3> vertices = {
		{-1, -1, -1},
		{1, 1, 1},
	};

	CAABoundingBox aabb;
	aabb.Build(vertices);

	CRay ray;
	ray.position = { 3.0f, 0, 3 };
	ray.direction = glm::normalize(glm::vec3(0, 0.5f, 0) - ray.position);

	CRayCastInfo info;
	if (aabb.RayCast(ray, info))
	{
		cout << "hit pos: " << info.position << "hit normal: " << info.normal << endl;
	}
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void processModelInput(Model& model, GLFWwindow* window);
void processCameraTransform(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float mouseOffsetX = 0.0f;
float mouseOffsetY = 0.0f;


int CreateTexture2D(std::string tex_path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int rgb_type;
	if (tex_path.find(".jpg") != std::string::npos)
	{
		rgb_type = GL_RGB;
	}
	else if (tex_path.find(".png") != std::string::npos)
	{
		rgb_type = GL_RGBA;
	}
	else
	{
		return -1;
	}

	// 加载并生成纹理
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tex_path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, rgb_type, width, height, 0, rgb_type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
#include "MyClass.h"

int CreateSampler()
{
	GLuint samplerId;
	glGenSamplers(1, &samplerId);

	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return samplerId;
}

#include "TaskGraph.h"
void TestTaskGraph()
{
	CTaskGraph g;
	auto f = []() {};
#define CreateTask(TaskNo) auto task##TaskNo = g.CreateTask("task "#TaskNo, f)
#define ConnectTask(Prev, Next) g.Connect((task##Prev), (task##Next))
	CreateTask(0);
	CreateTask(1);
	CreateTask(2);
	CreateTask(3);
	CreateTask(4);
	CreateTask(5);

	ConnectTask(0, 1);
	ConnectTask(0, 2);
	ConnectTask(0, 3);
	ConnectTask(1, 3);
	ConnectTask(1, 5);
	ConnectTask(3, 5);
	ConnectTask(2, 3);
	ConnectTask(2, 4);
	ConnectTask(4, 3);

	//ConnectTask(5, 4);
#undef CreateTask
#undef ConnectTask

	for(int i = 6; i < 1000; i++)
	{
		auto t = g.CreateTask((std::stringstream() << "task " << i).str(), f);
		auto prev_task_no = std::max(0, i % 6);
		auto prev_task = g.GetTask((std::stringstream() << "task " << prev_task_no).str());
		g.Connect(prev_task, t);
	}

	
	g.Run();
	getchar();

}
int main()
{
	TestTaskGraph();
	return 0;
	TestRapidJson();
	return 0;
	//TestAABB();
	using namespace glm;
	using namespace std;
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Model person(CSourceFinder::FindModelFullPath("nanosuit/nanosuit.obj"));

	auto vs_path = CSourceFinder::FindShaderFullPath("model.vert");
	auto fs_path = CSourceFinder::FindShaderFullPath("model.frag");
	CShader shader(vs_path.c_str(), fs_path.c_str());

	CShader baseShader = GetBaseShader();
	Model baseModel(CSourceFinder::FindModelFullPath("base/cube.fbx"));

	CShader bboxShader(CSourceFinder::FindShaderFullPath("bbox.vert").data(), CSourceFinder::FindShaderFullPath("bbox.frag").data());

	auto container_path = CSourceFinder::FindTexFullPath("container.jpg");

	auto texture1 = CreateTexture2D(container_path);
	auto awesomeface_path = CSourceFinder::FindTexFullPath("awesomeface.png");
	auto texture2 = CreateTexture2D(awesomeface_path);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//



	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),  (void*)offsetof(VertexData, color));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));
	glEnableVertexAttribArray(2);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	struct SCamMat
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::vec3 viewPos;
		float fill;
		glm::vec3 lightPos;
	};
	unsigned int camMatBuffer;
	glGenBuffers(1, &camMatBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, camMatBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SCamMat), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, camMatBuffer);

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);

	CShader shadowShader(CSourceFinder::FindShaderFullPath("shadow.vert").data(), CSourceFinder::FindShaderFullPath("shadow.frag").data());


	struct LightInfo {
		mat4 view;
		mat4 projection;

	} light_info;
	unsigned int lightInfoBuffer;
	glGenBuffers(1, &lightInfoBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, lightInfoBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightInfo), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightInfoBuffer);
	


	CRenderTexture rt(SCR_WIDTH, SCR_HEIGHT);
	CPostProcessShader postShader;
	
	CTriangleTest triangle;


	int texId = CreateTexture2D(CSourceFinder::FindTexFullPath("awesomeface.png"));
	
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.Aspect = (float)SCR_WIDTH / SCR_HEIGHT;
		// input
		processInput(window);
		processModelInput(person, window);

		if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			processCameraTransform(window);
			camera.ProcessMouseMovement(mouseOffsetX, mouseOffsetY);
			mouseOffsetX = mouseOffsetY = 0;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			

		}

		if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
		{
			CRay ray = camera.GetRay(vec2(lastX / SCR_WIDTH, 1 - lastY / SCR_HEIGHT));
			CRayCastInfo info;
			if (person.bounding_box.RayCast(ray, info))
			{
				cout << "intersect bbox, pos:" << info.position << endl;
			}
			if (triangle.RayCast(ray, info))
			{
				cout << "intersect triangle, pos:" << info.position << endl;
			}
			if (person.RayCast(ray, info))
			{
				cout << "intersect model, pos:" << info.position << endl;
			}
		}


		vec3 lightPos = glm::vec3(5 * cos(glfwGetTime()), 3, 5 * sin(glfwGetTime()));
		{
			mat4 light_view = glm::lookAt(lightPos, vec3(0, 0, 0), vec3(0, 1, 0));
			mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
			light_info.view = light_view;
			light_info.projection = light_projection;
			glBindBuffer(GL_UNIFORM_BUFFER, lightInfoBuffer);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(light_info), &light_info, GL_STATIC_DRAW);
		}

		// 渲染光源深度到framebuffer深度附件
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader.use();
		{
			glDisable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glm::mat4 model = person.transform.matrix();
			shadowShader.setMat4("model", model);
			person.Draw(shadowShader);

			model = mat4(1.0f);
			shadowShader.setMat4("model", model);
			baseModel.Draw(shadowShader);
		}
		

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetPerspectiveProjectionMatrix();

		SCamMat camMat;
		camMat.view = view;
		camMat.proj = projection;
		camMat.viewPos = camera.Position;
		camMat.lightPos = lightPos;
		glBindBuffer(GL_UNIFORM_BUFFER, camMatBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(camMat), &camMat);



		// render
		// ------

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		rt.BindFrameBuffer();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		


		// draw our first triangle

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		baseShader.use();
		{
			glDisable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glm::mat4 model(1.0f);
			baseShader.setMat4("model", model);
			// 绑定深度texture
			int shadow_idx =  8;
			glActiveTexture(GL_TEXTURE0 + shadow_idx);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			baseShader.setInt("texture_shadowmap", shadow_idx);
			baseModel.Draw(baseShader);
		}
		

		shader.use();
		{
			glDisable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glm::mat4 model = person.transform.matrix();
			shader.setMat4("model", model);
		}

		person.Draw(shader);

		bboxShader.use();
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(2);
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glm::mat4 model = person.transform.matrix();
			bboxShader.setMat4("model", model);
			person.bounding_box.Draw(bboxShader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		postShader.use();
		{
			glDisable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		rt.Draw(postShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void processCameraTransform(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		cout << "camera position: " << camera.Position << endl;
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void processModelInput(Model& model, GLFWwindow* window)
{
	using namespace glm;
	vec3 position(0);
	quat rotation(1, 0, 0, 0);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		position += camera.Front;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		position -= camera.Front;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			rotation = angleAxis(deltaTime, camera.Up) * rotation;
		}
		else
		{
			position += camera.Right;

		}
	}
		
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		{
			rotation = angleAxis(-deltaTime, camera.Up) * rotation;
		}
		else
		{
			position -= camera.Right;
		}
	}

	if (position != vec3(0))
	{
		position = normalize(position);
		model.transform.m_position += position * deltaTime * 5.0f;
	}

	if (rotation != quat(1, 0, 0, 0))
	{
		model.transform.m_rotataion = rotation * model.transform.m_rotataion;
	}
		
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	mouseOffsetX = xpos - lastX;
	mouseOffsetY = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

CPostProcessShader::CPostProcessShader(const char* fragPath) : CShader(CSourceFinder::FindShaderFullPath("post.vert").data(), fragPath)
{
}
