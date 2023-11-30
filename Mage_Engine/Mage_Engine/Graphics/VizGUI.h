#pragma once
#include <iostream>
#include <MageMaths.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

class Visualization;
class Application;

typedef void(Application::* VoidFunctionCallback)(void);
typedef void(Application::* VoidFunctionCallbackString)(const std::string&);
typedef void(Application::* VoidFunctionCallbackInt)(const int&);


class VizGUI
{
public:
	VizGUI();
	bool InitialiseGUI(GLFWwindow* window);
	void GUIBegin(const std::string& windowLabel);
	void GUIText(const std::string& text);
	void GUIEditText(const std::string& label, std::string& str);
	bool GUIButton(const std::string& label);
	void GUIButton(const std::string& label, VoidFunctionCallback callback, Application* app);
	void GUIButton(const std::string& label, VoidFunctionCallbackString callback, std::string& str, Application* app);
	void GUIButton(const std::string& label, VoidFunctionCallbackInt, int& value, Application* app);
	void GUICheckbox(const std::string& label, bool& value);
	void GUISliderBox(const std::string& label, float& value, const float& min, const float& max);
	void GUIEditFloat(const std::string& label, float& value);
	void GUIEditInt(const std::string& label, int& value);
	void GUIVector3(const std::string& label, Mage::Maths::Vector3& vec);
	void GUIEnd();
};

