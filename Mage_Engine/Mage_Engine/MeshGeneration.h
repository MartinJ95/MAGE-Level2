#pragma once
#include "Graphics/MeshGL.h"
#include <unordered_map>

struct VertexData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

struct ObjectDataSingular
{
	ObjectDataSingular() :
		minSize(), maxSize(), minTexcoord(), maxTexcoord()
	{}
	ObjectDataSingular(int MinSize, int MaxSize,
		int MinTexcoord, int MaxTexcoord) :
		minSize(MinSize), maxSize(MaxSize), minTexcoord(MinTexcoord), maxTexcoord(MaxTexcoord)
	{}
	ObjectDataSingular(const ObjectDataSingular& other) :
		minSize(other.minSize), maxSize(other.maxSize), minTexcoord(other.minTexcoord), maxTexcoord(other.maxTexcoord)
	{}
	int minSize;
	int maxSize;
	int minTexcoord;
	int maxTexcoord;
};

struct ObjectData
{
	ObjectData() :
		minSize(), maxSize(), minTexcoord(), maxTexcoord()
	{}
	ObjectData(const Mage::Maths::Vector3& MinSize, const Mage::Maths::Vector3& MaxSize,
		const Mage::Maths::Vector2& MinTexcoord, const Mage::Maths::Vector2& MaxTexcoord) :
		minSize(MinSize), maxSize(MaxSize), minTexcoord(MinTexcoord), maxTexcoord(MaxTexcoord)
	{}
	ObjectData(const ObjectData& other) :
		minSize(other.minSize), maxSize(other.maxSize), minTexcoord(other.minTexcoord), maxTexcoord(other.maxTexcoord)
	{}
	ObjectData(const ObjectDataSingular& other) :
		minSize(Mage::Maths::Vector3((float)other.minSize))
	{}
	Mage::Maths::Vector3 minSize;
	Mage::Maths::Vector3 maxSize;
	Mage::Maths::Vector2 minTexcoord;
	Mage::Maths::Vector2 maxTexcoord;
};

struct FaceData : public ObjectData
{
	FaceData() :
		ObjectData(), normal(), offset()
	{}
	FaceData(const ObjectData &data, Mage::Maths::Vector3 Normal, int Offset) :
		ObjectData(data), normal(Normal), offset(Offset)
	{}
	Mage::Maths::Vector3 normal;
	int offset;
};

class MeshGeneration
{
public:
	MeshGeneration();
	void DegenerateMesh(const std::string& meshName);
	void GenerateMesh(const VertexData &data, const std::string& meshName);
	void GenerateSquareMesh(const ObjectDataSingular& data, const std::string& meshName);
	void GenerateBoxMesh(const ObjectDataSingular&data, const std::string& meshName);
	void GenerateSphereMesh(const Mage::Maths::Vector3& center, const float radius, const int details, const std::string& meshName);
	MeshGL* GetMesh(const std::string& meshName) const;
	std::unordered_map<std::string, MeshGL*>& GetAllMeshes();
	void LoadObject(const std::string& filePath, const std::string& fileName, const std::string& fileType = ".obj");
	~MeshGeneration();
private:
	void GenerateFace(VertexData& data, const FaceData& fData);
	std::unordered_map<std::string, MeshGL*> m_meshes;
};

