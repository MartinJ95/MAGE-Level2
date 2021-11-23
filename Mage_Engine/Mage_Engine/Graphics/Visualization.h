#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
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
	bool InitialiseGUI();
	void GUIBegin(const std::string &windowLabel);
	void GUIText(const std::string &text);
	void GUIEditText(const std::string &label, std::string &str);
	bool GUIButton(const std::string &label);
	void GUIButton(const std::string &label, VoidFunctionCallback callback, Application *app);
	void GUIButton(const std::string &label, VoidFunctionCallbackString callback, std::string &str, Application *app);
	void GUIButton(const std::string &label, VoidFunctionCallbackInt, int &value, Application *app);
	void GUICheckbox(const std::string &label, bool &value);
	void GUISliderBox(const std::string &label, float &value, const float &min, const float &max);
	void GUIEditFloat(const std::string &label, float &value);
	void GUIVector3(const std::string &label, Mage::Maths::Vector3 &vec);
	void GUIEnd();
	void generateShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string &shaderName);
	bool isOpen() const;
	GLFWwindow* getWindow() const;
	void clear();
	void render2D(const std::string &meshName, const std::string &textureName, const std::string &shaderName, const glm::mat4 transformMatrix);
	void render3D(const std::string &meshName, const std::string &textureName, const std::string &shaderName, const glm::mat4 transformMatrix, Camera &camera, Mage::Maths::Vector3 &worldUp, Application &app);
	void display();
	void generateTexture(const std::string &textureFilePath, const std::string &textureName);
	void generateFace(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, const Mage::Maths::Vector3 &minSize, const Mage::Maths::Vector3 &maxSize, const Mage::Maths::Vector2 &minTexcoord, const Mage::Maths::Vector2 &maxTexcoord, const Mage::Maths::Vector3 &normal, const int &offset);
	void generateMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::string &meshName);
	void generateSquareMesh(const int &minSize, const int &maxSize, const int &minTexCoord, const int &maxTexCoord, const std::string &meshName);
	void generateBoxMesh(const int &minSize, const int &maxSize, const int &minTexCoord, const int &maxTexCoord, const std::string &meshName);
	void generateSphereMesh(const Mage::Maths::Vector3 &center, const float &radius, const int &details, const std::string &meshName);
	void generateSphereVertices(std::vector<Vertex>& vertices, const Mage::Maths::Vector3 &center, const int & details, Mage::Maths::Vector3 &newPos, float &theta, float &theta1, float &cs, float &sn, float &cs1, float &sn1);
	void generateSphereColumn(std::vector<Vertex>& vertices, const Mage::Maths::Vector3 &center, const int & details, Mage::Maths::Vector3 &newPos, float &theta, float &theta1, float &cs, float &sn, float &cs1, float &sn1);
	void generateSphereIndices(std::vector<unsigned int> &indices, const int &details);
	void loadObject(const std::string &filePath, const std::string &fileName, const std::string &fileType = ".obj");
	void useShader(const std::string &shaderName);
	void setShaderTexture(const std::string &textureUniform, const std::string &textureName, const std::string &shaderName, const int textureIndex);
	void setShaderUniformFloat(const std::string &shaderName, const std::string &uniformName, const float value);
	void setShaderUniformInt(const std::string &shaderName, const std::string &uniformName, const int value);
	void setShaderUniformBool(const std::string &shaderName, const std::string &uniformName, const bool value);
	void setShaderUniformMatrix4f(const std::string &shaderName, const std::string &uniformName, const glm::mat4 &matrix);
	void setShaderUniformVector3(const std::string &shaderName, const std::string &uniformName, const Mage::Maths::Vector3 &vector);
	~Visualization();
	int m_screenWidth, m_screenHeight;
private:
	GLuint compileShader(const std::string &shaderType, const std::string &shaderFileName);
	GLFWwindow* m_window;
	std::string m_windowName;
	std::unordered_map <std::string, MeshGL*> m_meshes;
	std::unordered_map <std::string, GLuint> m_textures;
	std::unordered_map <std::string, GLuint> m_shaderPrograms;
};

