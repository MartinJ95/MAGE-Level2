#include "InputManager.h"

InputManager *ActiveInputManager{ nullptr };

InputManager::InputManager(GLFWwindow * window) :
	m_window(window), m_firstMouseMove(true), m_mouseActive(false), m_lastMousePos(0.f, 0.f), m_mouseAxis("vertical1"), m_mouseAxis1("horizontal1"), m_axis(), m_inputs()
{
	m_inputs.emplace(std::pair<std::string, Input>("jump", Input()));
	m_inputs.emplace(std::pair<std::string, Input>("fire", Input()));
	m_inputs.emplace(std::pair<std::string, Input>("fire1", Input()));
	m_axis.emplace(std::pair<std::string, Input>("vertical", Input()));
	m_axis.emplace(std::pair<std::string, Input>("horizontal", Input()));
	m_axis.emplace(std::pair<std::string, Input>("vertical1", Input()));
	m_axis.emplace(std::pair<std::string, Input>("horizontal1", Input()));
}

Input* InputManager::getAxis(const std::string & input)
{
	if (m_axis.find(input) != m_axis.end())
	{
		return &m_axis.find(input)->second;
	}
	return nullptr;
}

Input* InputManager::getInput(const std::string & input)
{
	if (m_inputs.find(input) != m_inputs.end())
	{
		return &m_inputs.find(input)->second;
	}
	return nullptr;
}

void InputManager::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (ActiveInputManager == nullptr)
	{
		std::cout << "no active input manager" << std::endl;
		return;
	}
	if (ActiveInputManager->m_firstMouseMove == true)
	{
		ActiveInputManager->m_firstMouseMove = false;
		ActiveInputManager->m_lastMousePos = Mage::Maths::Vector2(xpos, ypos);
	}
	else if(ActiveInputManager->m_lastMousePos != Mage::Maths::Vector2(xpos, ypos))
	{
		Mage::Maths::Vector2 mouseMove = (Mage::Maths::Vector2(xpos, ypos) - ActiveInputManager->m_lastMousePos).Normalized();
		//std::cout << mouseMove.x << " - " << mouseMove.y << std::endl;
		ActiveInputManager->m_lastMousePos = Mage::Maths::Vector2(xpos, ypos);
		ActiveInputManager->getAxis(ActiveInputManager->m_mouseAxis)->m_inputValue = -mouseMove.y;
		ActiveInputManager->getAxis(ActiveInputManager->m_mouseAxis1)->m_inputValue = mouseMove.x;
	}
	else
	{
		ActiveInputManager->getAxis(ActiveInputManager->m_mouseAxis)->m_inputValue = 0;
		ActiveInputManager->getAxis(ActiveInputManager->m_mouseAxis1)->m_inputValue = 0;
	}
}

void InputManager::processInput()
{
	checkKeyAxis("vertical", GLFW_KEY_W, GLFW_KEY_S, 1, -1);
	checkKeyAxis("horizontal", GLFW_KEY_D, GLFW_KEY_A, 1, -1);
	checkKey("jump", GLFW_KEY_SPACE, 1);
	checkMouseButton("fire", GLFW_MOUSE_BUTTON_1, 1);
	checkMouseButton("fire1", GLFW_MOUSE_BUTTON_2, 1);
}

void InputManager::checkKey(const std::string & inputName, const int & keyNumber, const int & value)
{
	if (glfwGetKey(m_window, keyNumber) == GLFW_PRESS)
	{
		getInput(inputName)->m_inputValue = value;
	}
	else if (glfwGetKey(m_window, keyNumber) == GLFW_RELEASE)
	{
		getInput(inputName)->m_inputValue = 0;
	}
}

void InputManager::checkKeyAxis(const std::string &inputName, const int &keyNumber, const int &keyNumberOther, const int &value, const int &otherValue)
{
	if (glfwGetKey(m_window, keyNumber) == GLFW_PRESS)
	{
		getAxis(inputName)->m_inputValue = value;
	}
	else if (glfwGetKey(m_window, keyNumberOther) == GLFW_PRESS)
	{
		getAxis(inputName)->m_inputValue = otherValue;
	}
	else
	{
		getAxis(inputName)->m_inputValue = 0;
	}
}

void InputManager::checkMouseButton(const std::string & inputName, const int & keyNumber, const int & value)
{
	if (glfwGetMouseButton(m_window, keyNumber) == GLFW_PRESS)
	{
		getInput(inputName)->m_inputValue = 1;
	}
	else if (glfwGetMouseButton(m_window, keyNumber) == GLFW_RELEASE)
	{
		getInput(inputName)->m_inputValue = 0;
	}
}

InputManager::~InputManager()
{
}
