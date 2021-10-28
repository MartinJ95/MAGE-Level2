#pragma once
#include <unordered_map>
#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Maths/Vector.h"

struct Input
{
	Input() : m_inputValue(0) {};
	float m_inputValue;
};

class InputManager
{
public:
	InputManager(GLFWwindow *window);
	Input* getAxis(const std::string &input);
	Input* getInput(const std::string &input);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void processInput();
	void checkKey(const std::string &inputName, const int &keyNumber, const int &value);
	void checkKeyAxis(const std::string &inputName, const int &keyNumber, const int &keyNumberOther, const int &value, const int &otherValue);
	void checkMouseButton(const std::string &inputName, const int &keyNumber, const int &value);
	GLFWwindow *m_window;
	bool m_firstMouseMove;
	bool m_mouseActive;
	Mage::Maths::Vector2 m_lastMousePos;
	std::string m_mouseAxis;
	std::string m_mouseAxis1;
	std::unordered_map<std::string, Input> m_axis;
	std::unordered_map<std::string, Input> m_inputs;
	~InputManager();
};

extern InputManager *ActiveInputManager;
