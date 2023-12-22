#pragma once
#include <unordered_map>
#include "Graphics/VizGUI.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "MeshGL.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <cctype>
#include <functional>
#include "MeshGeneration.h"

#define PI 3.14159265

class Camera;
class Application;

typedef void(Application::*VoidFunctionCallback)(void);
typedef void(Application::*VoidFunctionCallbackString)(const std::string&);
typedef void(Application::*VoidFunctionCallbackInt)(const int&);

void frameBufferSizeCallback(GLFWwindow *window, int screenWidth, int screenHeight);

class Visualization
{
public:
	Visualization(const int screenWidth, const int screenHeight, const std::string &windowName);
	bool initialise();
	VizGUI& GetGUI();
	void generateShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string &shaderName);
	bool isOpen() const;
	GLFWwindow* getWindow() const;
	void clear();
	void render2D(const std::string &meshName, const std::string &textureName, const std::string &shaderName, const glm::mat4 transformMatrix);
	void render3D(const std::string &meshName, const std::string &textureName, const std::string &shaderName, const glm::mat4 transformMatrix, Camera &camera, Mage::Maths::Vector3 &worldUp, Application &app);
	void display();
	void generateTexture(const std::string &textureFilePath, const std::string &textureName);
	MeshGeneration& GetMeshGeneration() { return m_meshGeneration; }
	void LockCursor(bool toggle);
	~Visualization();
	int m_screenWidth, m_screenHeight;
private:
	void useShader(const std::string &shaderName);
	void setShaderTexture(const std::string &textureUniform, const std::string &textureName, const std::string &shaderName, const int textureIndex);
	void setShaderUniformFloat(const std::string &shaderName, const std::string &uniformName, const float value);
	void setShaderUniformInt(const std::string &shaderName, const std::string &uniformName, const int value);
	void setShaderUniformBool(const std::string &shaderName, const std::string &uniformName, const bool value);
	void setShaderUniformMatrix4f(const std::string &shaderName, const std::string &uniformName, const glm::mat4 &matrix);
	void setShaderUniformVector3(const std::string &shaderName, const std::string &uniformName, const Mage::Maths::Vector3 &vector);
	GLuint compileShader(const std::string &shaderType, const std::string &shaderFileName);
	VizGUI m_vizGUI;
	GLFWwindow* m_window;
	std::string m_windowName;
	MeshGeneration m_meshGeneration;
	std::unordered_map <std::string, GLuint> m_textures;
	std::unordered_map <std::string, GLuint> m_shaderPrograms;
};

