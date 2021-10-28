#include "pch.h"
#include "Editor.h"


Editor::Editor()
{
}

void Editor::Initialization()
{
}

void Editor::OnGUI()
{
	m_viz->GUIBegin("testWindow");
	m_viz->GUIText("description");
	m_viz->GUIEnd();
}

void Editor::OnUpdate()
{
	m_viz->clear();
	glfwPollEvents();
	OnGUI();

	m_viz->display();
}


Editor::~Editor()
{
}
