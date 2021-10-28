#pragma once
#include "Core\Application.h"

class Editor :
	public Application
{
public:
	Editor();
	void Initialization() override;
	void OnGUI() override;
	void OnUpdate() override;

	~Editor();
};