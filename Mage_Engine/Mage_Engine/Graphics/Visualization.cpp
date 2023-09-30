#include "Visualization.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "Camera.h"
#include "Core/Entity.h"
#include "Core/Application.h"

Visualization::Visualization(const int screenWidth, const int screenHeight, const std::string &windowName) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_window(nullptr),
	m_windowName(windowName),
	m_meshes(),
	m_textures(),
	m_shaderPrograms()
{
	m_meshes.reserve(10);
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

bool Visualization::InitialiseGUI()
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
	return true;
}

void Visualization::GUIBegin(const std::string & windowLabel)
{
	ImGui::Begin(windowLabel.c_str());
}

void Visualization::GUIText(const std::string & text)
{
	ImGui::Text(text.c_str());
}

void Visualization::GUIEditText(const std::string &label, std::string &str)
{
	ImGui::InputText(label.c_str(), &str);
}

bool Visualization::GUIButton(const std::string & label)
{
	return ImGui::Button(label.c_str());
}

void Visualization::GUIButton(const std::string & label, VoidFunctionCallback callback, Application *app)
{
	if (ImGui::Button(label.c_str())) { (app->*callback)();}
}

void Visualization::GUIButton(const std::string & label, VoidFunctionCallbackString callback, std::string &str, Application *app)
{
	if (ImGui::Button(label.c_str())) { (app->*callback)(str); }
}

void Visualization::GUIButton(const std::string & label, VoidFunctionCallbackInt callback, int &value, Application *app)
{
	if (ImGui::Button(label.c_str())) { (app->*callback)(value); }
}

void Visualization::GUICheckbox(const std::string & label, bool & value)
{
	ImGui::Checkbox(label.c_str(), &value);
}

void Visualization::GUISliderBox(const std::string & label, float & value, const float & min, const float & max)
{
	ImGui::SliderFloat(label.c_str(), &value, min, max);
}

void Visualization::GUIEditFloat(const std::string & label, float & value)
{
	ImGui::InputFloat(label.c_str(), &value);
}

void Visualization::GUIEditInt(const std::string& label, int& value)
{
	ImGui::InputInt(label.c_str(), &value);
}

void Visualization::GUIVector3(const std::string & label, Mage::Maths::Vector3 &vec)
{
	ImGui::InputFloat3(label.c_str(), (float*)&vec);
}

void Visualization::GUIEnd()
{
	ImGui::End();
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
	if (m_meshes.find(meshName) == m_meshes.end()) { return; }
	m_meshes.find(meshName)->second->render();
}

void Visualization::render3D(const std::string & meshName, const std::string & textureName, const std::string & shaderName, const glm::mat4 transformMatrix, Camera & camera, Mage::Maths::Vector3 &worldUp, Application &app)
{
	useShader(shaderName);
	setShaderUniformVector3(shaderName, "cameraPosition", app.m_currentLevel->m_data.m_mainCamera->m_entity.getComponent<Transform>()->m_position);
	setShaderUniformVector3(shaderName, "ambientLighting", app.m_ambientLighting);
	int numPointLights = 0;
	for (int i = 0; i < app.m_currentLevel->m_data.m_pointLights.size(); i++)
	{
		if (app.m_currentLevel->m_data.m_pointLights[i]->m_entity.m_active == true)
		{
			setShaderUniformVector3(shaderName, "pointLights[" + std::to_string(numPointLights) + "].m_position", app.m_currentLevel->m_data.m_pointLights[i]->m_entity.getComponent<Transform>()->worldPosition());
			setShaderUniformFloat(shaderName, "pointLights[" + std::to_string(numPointLights) + "].m_radius", app.m_currentLevel->m_data.m_pointLights[i]->m_radius);
			setShaderUniformVector3(shaderName, "pointLights[" + std::to_string(numPointLights) + "].m_intensity", app.m_currentLevel->m_data.m_pointLights[i]->m_intensity);
			numPointLights++;
		}
	}
	int numSpotLights = 0;
	for (int i = 0; i < app.m_currentLevel->m_data.m_spotLights.size(); i++)
	{
		if (app.m_currentLevel->m_data.m_spotLights[i]->m_entity.m_active == true)
		{
			setShaderUniformVector3(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_position", app.m_currentLevel->m_data.m_spotLights[i]->m_entity.getComponent<Transform>()->worldPosition());
			setShaderUniformVector3(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_intensity", app.m_currentLevel->m_data.m_spotLights[i]->m_intensity);
			setShaderUniformVector3(shaderName, "spotLights[" + std::to_string(numSpotLights) + "].m_direction", app.m_currentLevel->m_data.m_spotLights[i]->m_entity.getComponent<Transform>()->worldForward());
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
	Mage::Maths::Vector3 cameraPosition(camera.m_entity.getComponent<Transform>()->worldPosition());
	Mage::Maths::Vector3 cameraDirection(camera.m_entity.getComponent<Transform>()->worldForward());
	glm::vec3 camPos(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glm::vec3 camDir(cameraDirection.x, cameraDirection.y, cameraDirection.z);
	glm::vec3 up(worldUp.x, worldUp.y, worldUp.z);
	glm::mat4 view = glm::lookAt(camPos, camPos + camDir, up);
	setShaderUniformMatrix4f(shaderName, "view", view);
	setShaderTexture("Texture", textureName, shaderName, 1);
	if (m_meshes.find(meshName) == m_meshes.end()) { return; }
	m_meshes.find(meshName)->second->render();
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

void Visualization::generateFace(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const Mage::Maths::Vector3 & minSize, const Mage::Maths::Vector3 & maxSize, const Mage::Maths::Vector2 & minTexcoord, const Mage::Maths::Vector2 & maxTexcoord, const Mage::Maths::Vector3 & normal, const int & offset)
{
	unsigned int newIndices[]{ 0, 1, 2, 2, 1, 3 };

	for (auto i : newIndices)
	{
		indices.emplace_back(i + offset);
	}

	for (int i = 0; i < 4; i++)
	{
		vertices.emplace_back(Vertex());
		if (minSize.x != maxSize.x)
		{
			vertices[i + offset].position = Mage::Maths::Vector3(
				(i % 2 == 0) ? minSize.x : maxSize.x,
				(i < 2) ? minSize.y : maxSize.y,
				(i < 2) ? minSize.z : maxSize.z);
		}
		else
		{
			vertices[i + offset].position = Mage::Maths::Vector3(
				minSize.x,
				(i < 2) ? minSize.y : maxSize.y,
				(i % 2 == 0) ? minSize.z : maxSize.z);
		}

		vertices[i + offset].normal = normal;
		vertices[i + offset].color = Mage::Maths::Vector3(0, 0, 0);

		vertices[i + offset].texCoords = Mage::Maths::Vector2(
			(i % 2 == 0) ? minTexcoord.x : maxTexcoord.x,
			(i < 2) ? minTexcoord.y : maxTexcoord.y);
	}
	/*for (int i = 0; i < 4; i++)
	{
		vertices.emplace_back(Vertex());
		vertices[i].position = Mage::Maths::Vector3(0, 0, 0);
		vertices[i].position.x = (i % 2 == 0) ? minSize : maxSize;
		vertices[i].position.y = (i < 2) ? minSize : maxSize;

		vertices[i].color = Mage::Maths::Vector3(0, 0, 0);

		vertices[i].normal = Mage::Maths::Vector3(0, 0, 1);

		vertices[i].texCoords = Mage::Maths::Vector2(0, 0);
		vertices[i].texCoords.x = (i % 2 == 0) ? minTexCoord : maxTexCoord;
		vertices[i].texCoords.y = (i < 2) ? minTexCoord : maxTexCoord;
	}*/
}

void Visualization::degenerateMesh(const std::string& meshName)
{
	if (m_meshes.find(meshName) == m_meshes.end()) { return; }

	delete m_meshes.at(meshName);
	m_meshes.erase(meshName);
}

void Visualization::generateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string & meshName)
{
	if (m_meshes.find(meshName) != m_meshes.end()) { return; }

	MeshGL *newMesh = new MeshGL(vertices, indices);
	newMesh->initualize();
	//std::pair<std::string, MeshGL> meshPair;
	//meshPair.first = meshName;
	//meshPair.second = MeshGL(vertices, indices);
	m_meshes.emplace(meshName, newMesh);
}

void Visualization::generateSquareMesh(const int & minSize, const int & maxSize, const int & minTexCoord, const int & maxTexCoord, const std::string & meshName)
{

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	generateFace(vertices, indices, Mage::Maths::Vector3(minSize, minSize, 0), Mage::Maths::Vector3(maxSize, maxSize, 0), Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord), Mage::Maths::Vector3(0, 0, 1), 0);

	generateMesh(vertices, indices, meshName);
}

void Visualization::generateBoxMesh(const int & minSize, const int & maxSize, const int & minTexCoord, const int & maxTexCoord, const std::string & meshName)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//back
	generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, minSize), Mage::Maths::Vector3(maxSize, maxSize, minSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, 0, -1), 0);
	//front
	generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, maxSize), Mage::Maths::Vector3(maxSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, 0, 1), 4);
	//top
	generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, maxSize, minSize), Mage::Maths::Vector3(maxSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, 1, 0), 8);
	//bottom
	generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, minSize), Mage::Maths::Vector3(maxSize, minSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, -1, 0), 12);
	//left
	generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, minSize), Mage::Maths::Vector3(minSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(-1, 0, 0), 16);
	//right
	generateFace(vertices, indices,
		Mage::Maths::Vector3(maxSize, minSize, minSize), Mage::Maths::Vector3(maxSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord),	Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(1, 0, 0), 20);

	generateMesh(vertices, indices, meshName);
}

void Visualization::generateSphereMesh(const Mage::Maths::Vector3 & center, const float & radius, const int & details, const std::string &meshName)
{

	auto GenerateSphereIndices = [](std::vector<unsigned int>& indices, const int details)
	{
		int iOffset, iOffset1;
		int jOffset, jOffset1;
		//rows
		for (int i = 0; i < details; i++)
		{
			//columns
			for (int j = 0; j < details + 1; j++)
			{
				iOffset = i + 2; // current row
				iOffset1 = i + 1; // row before

				jOffset = j * details; // current column
				jOffset1 = (j + 1) * details; // next column

				// 0 = top , 1 = bottom

				if (i != 0 && i != (details - 1)) // middle
				{
					indices.emplace_back(iOffset1 + jOffset);
					indices.emplace_back(iOffset1 + jOffset1);
					indices.emplace_back(iOffset + jOffset);

					indices.emplace_back(iOffset + jOffset);
					indices.emplace_back(iOffset1 + jOffset1);
					indices.emplace_back(iOffset + jOffset1);
				}
				else if (i == 0) // top
				{
					indices.emplace_back(0);
					indices.emplace_back(iOffset + jOffset1);
					indices.emplace_back(iOffset + jOffset);
				}
				else if (i == (details - 1)) // bottom
				{
					//last middle
					indices.emplace_back(iOffset1 + jOffset);
					indices.emplace_back(iOffset1 + jOffset1);
					indices.emplace_back(iOffset + jOffset);

					indices.emplace_back(iOffset + jOffset);
					indices.emplace_back(iOffset1 + jOffset1);
					indices.emplace_back(iOffset + jOffset1);

					//bottom
					indices.emplace_back(iOffset + jOffset);
					indices.emplace_back(iOffset + jOffset1);
					indices.emplace_back(1);
				}
			}
		}
	};

	auto GenerateSphereVertices = [](std::vector<Vertex>& vertices, const Mage::Maths::Vector3& center, const int details, Mage::Maths::Vector3& newPos, float& theta, float& theta1, float& cs, float& sn)
	{
		auto GenerateSphereColumn = [](std::vector<Vertex>& vertices, const Mage::Maths::Vector3& center, const int details, Mage::Maths::Vector3& newPos, float theta, float theta1, float cs, float sn)
		{
			StackDebugging::GetInstance()->LogFunction("visualization::generateSphere::genVertices::genColumn");
			float xTexCoord = (theta * (180 / PI) / 360);
			for (int j = 1; j < details + 1; j++)
			{
				Vertex v;

				float cs1 = cos(theta1 * j);
				float sn1 = sin(theta1 * j);

				//z rotation
				v.position = Mage::Maths::Vector3(vertices[0].position.x * cs1 - vertices[0].position.y * sn1, vertices[0].position.x * sn1 + vertices[0].position.y * cs1, 0);

				//y rotation
				newPos = Mage::Maths::Vector3(v.position.x * cs + v.position.z * sn, v.position.y, -(v.position.x * sn) + v.position.z * cs);
				v.position = newPos;
				v.color = Mage::Maths::Vector3(0, 0, 0);
				v.normal = (v.position - center).Normalized();
				v.texCoords = Mage::Maths::Vector2(xTexCoord, (Mage::Maths::Vector3(0, 1, 0).Dot(v.position.Normalized()) + 1) / 2);
				std::cout << v.texCoords.x << std::endl;
				vertices.emplace_back(std::move(v));
			}
			StackDebugging::GetInstance()->PopFunction();
		};

		StackDebugging::GetInstance()->LogFunction("visualization::generateSphere::genVertices");
		float thetaChange = (360 / (details + 1)) * (PI / 180);
		theta1 = (180 / (details + 1)) * (PI / 180);
		for (int i = 0; i < details; i++)
		{
			theta = thetaChange * i;

			cs = cos(theta);
			sn = sin(theta);

			GenerateSphereColumn(vertices, center, details, newPos, theta, theta1, cs, sn);

			if (i == details - 1)
			{
				theta = 360 * (PI / 180);
				cs = cos(theta);
				sn = sin(theta);

				GenerateSphereColumn(vertices, center, details, newPos, theta, theta1, cs, sn);
				//generateSphereColumn(vertices, center, details, newPos, theta, theta1, cs, sn);
			}
		};
		StackDebugging::GetInstance()->PopFunction();
		return;
	};

	std::vector<Vertex> vertices;
	vertices.reserve(2 + (details * details));
	std::vector<unsigned int> indices;

	vertices.emplace_back();
	vertices.emplace_back();

	//top
	vertices[0].position = center + Mage::Maths::Vector3(0, radius, 0);
	vertices[0].color = Mage::Maths::Vector3(0, 0, 0);
	vertices[0].normal = Mage::Maths::Vector3(0, 1, 0);
	vertices[0].texCoords = Mage::Maths::Vector2(0, 0);

	//bottom
	vertices[1].position = center - Mage::Maths::Vector3(0, radius, 0);
	vertices[1].color = Mage::Maths::Vector3(0, 0, 0);
	vertices[1].normal = Mage::Maths::Vector3(0, -1, 0);
	vertices[1].texCoords = Mage::Maths::Vector2(1, 1);

	//theta = y rotation
	//theta1 = z rotation
	float theta, theta1;
	float cs, cs1;
	float sn, sn1;

	Mage::Maths::Vector3 newPos;

	//vertex generation
	GenerateSphereVertices(vertices, center, details, newPos, theta, theta1, cs, sn);

	GenerateSphereIndices(indices, details);

	generateMesh(vertices, indices, meshName);
}

void Visualization::loadObject(const std::string &filePath, const std::string & fileName, const std::string &fileType)
{
	std::vector<Vertex> vertices;
	std::vector<Mage::Maths::Vector3> vertexPositions;
	std::vector<Mage::Maths::Vector3> vertexNormals;
	std::vector<Mage::Maths::Vector2> vertexTexcoords;
	std::vector<Mage::Maths::Vector3i> vertexElements;
	std::vector<unsigned int> elements;
	int faceElements = 0;
	for (int i = 0; i < 4; i++)
	{
		vertexElements.emplace_back(Mage::Maths::Vector3i());
	}
	std::ifstream reader(filePath + fileName + fileType);
	std::string readLine;
	if (!reader)
	{
		std::cout << "error opening file " + (fileName + fileType) << std::endl;
		return;
	}
	while (!reader.eof())
	{
		reader >> readLine;
		if (readLine == "v")
		{
			vertexPositions.emplace_back(Mage::Maths::Vector3());
			Mage::Maths::Vector3 &newVertexPosition = vertexPositions.back();
			reader >> newVertexPosition.x;
			reader >> newVertexPosition.y;
			reader >> newVertexPosition.z;
		}
		else if (readLine == "vn")
		{
			vertexNormals.emplace_back(Mage::Maths::Vector3());
			Mage::Maths::Vector3 &newVertexNormal = vertexNormals.back();
			reader >> newVertexNormal.x;
			reader >> newVertexNormal.y;
			reader >> newVertexNormal.z;
		}
		else if (readLine == "vt")
		{
			vertexTexcoords.emplace_back(Mage::Maths::Vector2());
			Mage::Maths::Vector2 &newVertexTexCoords = vertexTexcoords.back();
			reader >> newVertexTexCoords.x;
			reader >> newVertexTexCoords.y;
		}
		else if (readLine == "f")
		{
			std::cout << "now testing reader" << std::endl;
			int test = reader.peek();
			char c = static_cast<char>(test);
			std::cout << c << std::endl;
			for (int i = 0; i < 4; i++)
			{
				std::string output;
				reader >> output;
				if (output.empty())
				{
					break;
				}
				char test = output[0];
				if (!std::isdigit(test))
				{
					reader.unget();
					break;
				}
				size_t pos = 0;
				std::string delimiter = "/";
				std::string tokens[3];
				unsigned int tokenIndex = 0;
				while (pos = output.find(delimiter) != output.npos)
				{
					tokens[tokenIndex] = output.substr(0, output.find(delimiter));
					tokenIndex++;
					output.erase(0, output.find(delimiter) + delimiter.length());
				}
				tokens[2] = output;
				vertexElements[faceElements].x = std::stoi(tokens[0]) - 1;
				vertexElements[faceElements].y = std::stoi(tokens[2]) - 1;
				vertexElements[faceElements].z = std::stoi(tokens[1]) - 1;
				faceElements++;
			}
			std::vector<unsigned int> indices;
			for (int i = 0; i < faceElements; i++)
			{
				Vertex newVertex;
				int indexCheck = -1;
				newVertex.position = vertexPositions[vertexElements[i].x];
				newVertex.normal = vertexNormals[vertexElements[i].y];
				newVertex.texCoords = vertexTexcoords[vertexElements[i].z];
				if (vertices.empty())
				{
					vertices.emplace_back(newVertex);
					indices.emplace_back(0);
				}
				else
				{
					for (int j = 0; j < static_cast<int>(vertices.size()); j++)
					{
						if (vertices[j].position == newVertex.position && vertices[j].normal == newVertex.normal && vertices[j].texCoords == newVertex.texCoords)
						{
							indexCheck = j;
							break;
						}
					}
					if (indexCheck != -1)
					{
						indices.emplace_back(indexCheck);
						indexCheck = -1;
					}
					else
					{
						vertices.emplace_back(newVertex);
						indices.emplace_back(static_cast<int>(vertices.size()) - 1);
					}
				}
			}
			if (faceElements == 4)
			{
				elements.emplace_back(indices[1]);
				elements.emplace_back(indices[0]);
				elements.emplace_back(indices[2]);

				elements.emplace_back(indices[2]);
				elements.emplace_back(indices[0]);
				elements.emplace_back(indices[3]);
			}
			else if (faceElements == 3)
			{
				elements.emplace_back(indices[1]);
				elements.emplace_back(indices[0]);
				elements.emplace_back(indices[2]);
			}
			indices.clear();
			faceElements = 0;
		}
	}
	reader.close();
	generateMesh(vertices, elements, fileName);
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
	for (auto i : m_meshes)
	{
		delete i.second;
	}
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
