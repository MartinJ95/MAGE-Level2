#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Maths/Vector.h"

struct Vertex
{
	Mage::Maths::Vector3 position;
	Mage::Maths::Vector3 color;
	Mage::Maths::Vector3 normal;
	Mage::Maths::Vector2 texCoords;
};

enum VertexAttributes
{
	vertexPosition = 0,
	vertexColor = 1,
	vertexNormal = 2,
	vertexTexCoords = 3
};

enum FragmentData
{
	FragmentColor = 0
};

class MeshGL
{
public:
	MeshGL();
	MeshGL(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices);
	void initualize();
	void render();
	~MeshGL();
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	unsigned int m_elementCount;
private:
	GLuint m_VertexVBO;
	GLuint m_IndexIBO;
	GLuint m_MeshVAO;
};