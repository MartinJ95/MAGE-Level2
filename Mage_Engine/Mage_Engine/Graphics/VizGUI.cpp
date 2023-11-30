#include "VizGUI.h"

VizGUI::VizGUI()
{}

bool VizGUI::InitialiseGUI(GLFWwindow *window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
	return true;
}

void VizGUI::GUIBegin(const std::string& windowLabel)
{
	ImGui::Begin(windowLabel.c_str());
}

void VizGUI::GUIText(const std::string& text)
{
	ImGui::Text(text.c_str());
}

void VizGUI::GUIEditText(const std::string& label, std::string& str)
{
	ImGui::InputText(label.c_str(), &str);
}

bool VizGUI::GUIButton(const std::string& label)
{
	return ImGui::Button(label.c_str());
}

void VizGUI::GUIButton(const std::string& label, VoidFunctionCallback callback, Application* app)
{
	if (ImGui::Button(label.c_str())) { (app->*callback)(); }
}

void VizGUI::GUIButton(const std::string& label, VoidFunctionCallbackString callback, std::string& str, Application* app)
{
	if (ImGui::Button(label.c_str())) { (app->*callback)(str); }
}

void VizGUI::GUIButton(const std::string& label, VoidFunctionCallbackInt callback, int& value, Application* app)
{
	if (ImGui::Button(label.c_str())) { (app->*callback)(value); }
}

void VizGUI::GUICheckbox(const std::string& label, bool& value)
{
	ImGui::Checkbox(label.c_str(), &value);
}

void VizGUI::GUISliderBox(const std::string& label, float& value, const float& min, const float& max)
{
	ImGui::SliderFloat(label.c_str(), &value, min, max);
}

void VizGUI::GUIEditFloat(const std::string& label, float& value)
{
	ImGui::InputFloat(label.c_str(), &value);
}

void VizGUI::GUIEditInt(const std::string& label, int& value)
{
	ImGui::InputInt(label.c_str(), &value);
}

void VizGUI::GUIVector3(const std::string& label, Mage::Maths::Vector3& vec)
{
	ImGui::InputFloat3(label.c_str(), (float*)&vec);
}

void VizGUI::GUIEnd()
{
	ImGui::End();
}