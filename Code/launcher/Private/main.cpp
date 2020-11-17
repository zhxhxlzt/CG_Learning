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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 15.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



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

int main()
{
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glm::mat4 view(1.0f);
	// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	//view = glm::translate(view, glm::vec3(0.0f, 2, -3.0f));
	view = glm::lookAt(vec3(0.0f, 1.5f, 10.0f), vec3(0.0f), vec3(0, 1, 0));
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

	int texture_arr[4];
	for (int i = 0; i < 4; i++)
	{
		std::stringstream convert;
		convert << i;
		std::string num;
		convert >> num;
		auto pos = awesomeface_path.find(".png");
		std::string png_path = awesomeface_path.substr(0, pos) + num + ".png";
		cout << "png path:" << png_path << endl;
		texture_arr[i] = CreateTexture2D(png_path);
	}
	glEnable(GL_DEPTH_TEST);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// input
		processInput(window);
		processModelInput(person, window);

		

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();



		// render
		// ------

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
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			baseShader.setMat4("model", model);
			baseShader.setMat4("view", view);
			baseShader.setMat4("projection", projection);
			baseModel.Draw(baseShader);
		}
		

		shader.use();
		{
			glDisable(GL_BLEND);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glm::mat4 model = person.transform.matrix();
			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
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
			bboxShader.setMat4("view", view);
			bboxShader.setMat4("projection", projection);
			person.bounding_box.Draw(bboxShader);
		}

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}