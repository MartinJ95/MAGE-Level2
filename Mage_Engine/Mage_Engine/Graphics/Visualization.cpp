#include "Visualization.h"
#include "Core/Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "Camera.h"
#include "Core/Entity.h"
#include <queue>

Visualization::Visualization(const int screenWidth, const int screenHeight, const std::string &windowName) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_window(nullptr),
	m_windowName(windowName),
	m_meshGeneration(),
	m_textures(),
	m_shaderPrograms()
{
	//m_meshes.reserve(10);
}

bool Visualization::initialise()
{
	/* Initialize the library GLFW */
	if (!glfwInit())
	{
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, m_windowName.c_str(), NULL, NULL);
	if (m_window == NULL)
	{
		glfwTerminate();
		{
			return false;
		}
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD failed to initualize" << std::endl;
		return false;
	}
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glEnable(GL_DEPTH_TEST);
	return true;
}

VizGUI& Visualization::GetGUI()
{
	return m_vizGUI;
}

void Visualization::generateShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string &shaderName)
{
	GLuint shaderProgramID;

	int success;
	char infoLog[512];

	GLuint vertex = compileShader("VERTEX", vertexShader);
	GLuint fragment = compileShader("FRAGMENT", fragmentShader);

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertex);
	glAttachShader(shaderProgramID, fragment);
	glLinkProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	m_shaderPrograms.emplace(shaderName, shaderProgramID);
}

bool Visualization::isOpen() const
{
	if (!glfwWindowShouldClose(m_window))
	{
		return true;
	}
	return false;
}

GLFWwindow * Visualization::getWindow() const
{
	if (m_window != nullptr)
	{
		return m_window;
	}
	return nullptr;
}

void Visualization::clear()
{
	if (!glfwWindowShouldClose(m_window))
	{
		glClearColor(0.f, 0.25f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		return;
	}
	return;
}

void Visualization::render2D(const std::string & meshName, const std::string & textureName, const std::string & shaderName, const glm::mat4 transformMatrix)
{
	useShader(shaderName);
	setShaderUniformMatrix4f(shaderName, "model_xform", transformMatrix);
	setShaderTexture("Texture", textureName, shaderName, 1);
	if (GetMeshGeneration().GetAllMeshes().find(meshName) == GetMeshGeneration().GetAllMeshes().end()) { return; }
	m_meshGeneration.GetAllMeshes().find(meshName)->second->render();
}

void Visualization::render3D(const std::string & meshName, const std::string & textureName, const std::string & shaderName, const glm::mat4 transformMatrix, Camera & camera, Mage::Maths::Vector3 &worldUp, Application &app)
{
	useShader(shaderName);
	setShaderUniformVector3(shaderName, "cameraPosition", app.m_currentLevel->m_data.m_mainCamera->m_entity->getComponent<Transform>()->m_position);
	setShaderUniformVector3(shaderName, "ambientLighting", app.m_ambientLighting);
	int numPointLights = 0;
	for (int i = 0; i < app.m_currentLevel->m_data.m_pointLights.size(); i++)
	{
		if (app.m_currentLevel->m_data.m_pointLights[i]->m_entity->m_active == true)
		{
			setShaderUniformVector3(shaderName, "pointLights[" + std::to_string(numPointLights) + "].m_position", app.m_currentLevel->m_data.m_pointLights[i]->m_entity->getComponent<Transform>()->worldPosition());
			setShaderUniformFloat(shaderName, "pointLights[" + std::to_string(numPointLights) + "].m_radius", app.m_currentLevel->m_data.m_pointLights[i]->m_radius);
			setShaderUniformVector3(shaderName, "pointLights[" + std::to_string(numPointLights) + "].m_intensity", app.m_currentLevel->m_data.m_pointLights[i]->m_intensity);
			numPointLights++;
		}
	}
	int numSpotLights = 0;
	for (int i = 0; i < app.m_currentLevel->m_data.m_spotLights.size(); i++)
	{
		if (app.m_currentLevel->m_data.m_spotLights[i]->m_entity->m_active == true)
		{
			setShaderUniformVector3(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_position", app.m_currentLevel->m_data.m_spotLights[i]->m_entity->getComponent<Transform>()->worldPosition());
			setShaderUniformVector3(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_intensity", app.m_currentLevel->m_data.m_spotLights[i]->m_intensity);
			setShaderUniformVector3(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_direction", app.m_currentLevel->m_data.m_spotLights[i]->m_entity->getComponent<Transform>()->worldForward());
			setShaderUniformFloat(shaderName, "spotlights[" + std::to_string(numSpotLights) + "].m_range", app.m_currentLevel->m_data.m_spotLights[i]->m_range);
			setShaderUniformFloat(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_fieldOfView", app.m_currentLevel->m_data.m_spotLights[i]->m_fieldOfView);
			numSpotLights++;
		}
	}
	setShaderUniformInt(shaderName, "numPointLights", numPointLights);
	setShaderUniformInt(shaderName, "numSpotLights", numSpotLights);
	setShaderUniformMatrix4f(shaderName, "model_xform", transformMatrix);
	glm::mat4 projection = glm::perspective(glm::radians(camera.m_fieldOfView), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
	setShaderUniformMatrix4f(shaderName, "projection", projection);
	Mage::Maths::Vector3 cameraPosition(camera.m_entity->getComponent<Transform>()->worldPosition());
	Mage::Maths::Vector3 cameraDirection(camera.m_entity->getComponent<Transform>()->worldForward());
	glm::vec3 camPos(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glm::vec3 camDir(cameraDirection.x, cameraDirection.y, cameraDirection.z);
	glm::vec3 up(worldUp.x, worldUp.y, worldUp.z);
	glm::mat4 view = glm::lookAt(camPos, camPos + camDir, up);
	setShaderUniformMatrix4f(shaderName, "view", view);
	setShaderTexture("Texture", textureName, shaderName, 1);
	if (GetMeshGeneration().GetAllMeshes().find(meshName) == GetMeshGeneration().GetAllMeshes().end()) { return; }
	GetMeshGeneration().GetAllMeshes().find(meshName)->second->render();
}

void Visualization::display()
{
	if (!glfwWindowShouldClose(m_window))
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window);
		return;
	}
	return;
}

void Visualization::generateTexture(const std::string & textureFilePath, const std::string & textureName)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	m_textures.emplace(textureName, texture);
}

void Visualization::useShader(const std::string & shaderName)
{
	if (m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		glUseProgram(m_shaderPrograms.find(shaderName)->second);
	}
}

void Visualization::setShaderTexture(const std::string &textureUniform, const std::string & textureName, const std::string & shaderName, const int textureIndex)
{
	if (m_textures.find(textureName) != m_textures.end()
		&& m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, m_textures.find(textureName)->second);
		setShaderUniformInt(shaderName, textureUniform, 1);
	}
	
}

void Visualization::setShaderUniformFloat(const std::string & shaderName, const std::string & uniformName, const float value)
{
	if (m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		glUniform1f(glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()), value);
	}
}

void Visualization::setShaderUniformInt(const std::string & shaderName, const std::string & uniformName, const int value)
{
	if (m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		glUniform1i(glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()), value);
	}
}

void Visualization::setShaderUniformBool(const std::string & shaderName, const std::string & uniformName, const bool value)
{
	if (m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		glUniform1i(glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()), (int)value);
	}
}

void Visualization::setShaderUniformMatrix4f(const std::string & shaderName, const std::string & uniformName, const glm::mat4 &matrix)
{
	if (m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		if (glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()) == -1)
		{
			std::cout << "error finding uniform location" << std::endl;
		}
		glUniformMatrix4fv(glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

void Visualization::setShaderUniformVector3(const std::string &shaderName, const std::string &uniformName, const Mage::Maths::Vector3 &vector)
{
	if (m_shaderPrograms.find(shaderName) != m_shaderPrograms.end())
	{
		if (glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()) == -1)
		{
			std::cout << "error finding uniform location" << std::endl;
		}
		glUniform3fv(glGetUniformLocation(m_shaderPrograms.find(shaderName)->second, uniformName.c_str()), 1, &vector.x);
	}
}

void Visualization::LockCursor(bool toggle)
{
	if (toggle)
	{
		GLFW_CURSOR_DISABLED;
		return;
	}
	GLFW_CURSOR_NORMAL;
	return;
}

Visualization::~Visualization()
{
	for (auto i : m_shaderPrograms)
	{
		glDeleteProgram(i.second);
	}
	/*for (auto i : m_meshes)
	{
		delete i.second;
	}*/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

GLuint Visualization::compileShader(const std::string &shaderType, const std::string &shaderFileName)
{

	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	shaderFile.open(shaderFileName);
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	shaderCode = shaderStream.str();

	const char* cShaderCode = shaderCode.c_str();

	GLuint shader{ 0 };
	int success;
	char infoLog[512];
	if (shaderType == "VERTEX")
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderType == "FRAGMENT")
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glShaderSource(shader, 1, &cShaderCode, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" + shaderType + "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

void frameBufferSizeCallback(GLFWwindow * window, int screenWidth, int screenHeight)
{
	glViewport(0, 0, screenWidth, screenHeight);
}
