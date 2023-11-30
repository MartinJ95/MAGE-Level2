#include "MeshGeneration.h"
#include <thread>
#include <mutex>
#include "StackDebugging.h"

#define PI 3.14159265

MeshGeneration::MeshGeneration()
{
}

void MeshGeneration::DegenerateMesh(const std::string& meshName)
{
	if (m_meshes.find(meshName) == m_meshes.end()) { return; }

	delete m_meshes.at(meshName);
	m_meshes.erase(meshName);
}

void MeshGeneration::GenerateMesh(const VertexData& data, const std::string& meshName)
{
	if (m_meshes.find(meshName) != m_meshes.end()) { return; }

	//MeshGL *newMesh = new MeshGL(vertices, indices);
	//newMesh->initualize();

	//std::pair<std::string, MeshGL> meshPair;
	//meshPair.first = meshName;
	//meshPair.second = MeshGL(vertices, indices);
	m_meshes.emplace(meshName, new MeshGL(data.vertices, data.indices));
	m_meshes.at(meshName)->initualize();
}

void MeshGeneration::GenerateSquareMesh(const ObjectDataSingular& data, const std::string& meshName)
{
	//std::vector<Vertex> vertices;
	//std::vector<unsigned int> indices;
	VertexData vData;

	//GenerateFace(vData, Mage::Maths::Vector3(minSize, minSize, 0), Mage::Maths::Vector3(maxSize, maxSize, 0), Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord), Mage::Maths::Vector3(0, 0, 1), 0);
	GenerateFace(
		vData, FaceData(
			ObjectData(
				Mage::Maths::Vector3(data.minSize, data.minSize, 0),
				Mage::Maths::Vector3(data.maxSize, data.maxSize, 0),
				Mage::Maths::Vector2(data.minTexcoord, data.minTexcoord),
				Mage::Maths::Vector2(data.maxTexcoord, data.maxTexcoord)),
			Mage::Maths::Vector3(0, 0, 1), 0)
	);

	GenerateMesh(vData, meshName);
}

void MeshGeneration::GenerateBoxMesh(const ObjectDataSingular& data, const std::string& meshName)
{
	//std::vector<Vertex> vertices;
	//std::vector<unsigned int> indices;
	VertexData vData;


	auto FaceGen = [](MeshGeneration *m, const Mage::Maths::Vector3i&& begin, const Mage::Maths::Vector3i&& end,
		const Mage::Maths::Vector3&& facingNormal, VertexData& vData, const ObjectDataSingular& data, int offset)
	{
		int vals[] = { data.minSize, data.maxSize };
		m->GenerateFace(
			vData, FaceData(
				ObjectData(
					Mage::Maths::Vector3(vals[begin.x], vals[begin.y], vals[begin.z]),
					Mage::Maths::Vector3(vals[end.x], vals[end.y], vals[end.z]),
					Mage::Maths::Vector2(data.minTexcoord, data.minTexcoord),
					Mage::Maths::Vector2(data.maxTexcoord, data.maxTexcoord)
				),
				facingNormal, offset
			)
		);
	};

	typedef Mage::Maths::Vector3i V3I;
	typedef Mage::Maths::Vector3 V3;

	//back
	FaceGen(this, std::move(V3I(0, 0, 0)), std::move(V3I(1, 1, 0)),
		std::move(V3(0, 0, -1)), vData, data, 0);
	/*GenerateFace(
		vData, FaceData(
			ObjectData(
				Mage::Maths::Vector3(data.minSize, data.minSize, data.minSize),
				Mage::Maths::Vector3(data.maxSize, data.maxSize, data.minSize),
				Mage::Maths::Vector2(data.minTexcoord, data.minTexcoord),
				Mage::Maths::Vector2(data.maxTexcoord, data.maxTexcoord)
			),
			Mage::Maths::Vector3(0, 0, -1), 0
		)
	);*/
	/*generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, minSize), Mage::Maths::Vector3(maxSize, maxSize, minSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, 0, -1), 0);*/
	//front
	FaceGen(this, std::move(V3I(0, 0, 1)), std::move(V3I(1, 1, 1)),
		std::move(V3(0, 0, 1)), vData, data, 4);
	/*generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, maxSize), Mage::Maths::Vector3(maxSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, 0, 1), 4);*/
	//top
	FaceGen(this, std::move(V3I(0, 1, 0)), std::move(V3I(1, 1, 1)),
		std::move(V3(0, 1, 0)), vData, data, 8);
	/*generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, maxSize, minSize), Mage::Maths::Vector3(maxSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, 1, 0), 8);*/
	//bottom
	FaceGen(this, std::move(V3I(0, 0, 0)), std::move(V3I(1, 0, 1)),
		std::move(V3(0, -1, 0)), vData, data, 12);
	/*generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, minSize), Mage::Maths::Vector3(maxSize, minSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(0, -1, 0), 12);*/
	//left
	FaceGen(this, std::move(V3I(0, 0, 0)), std::move(V3I(0, 1, 1)),
		std::move(V3(-1, 0, 0)), vData, data, 16);
	/*generateFace(vertices, indices,
		Mage::Maths::Vector3(minSize, minSize, minSize), Mage::Maths::Vector3(minSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(-1, 0, 0), 16);*/
	//right
	FaceGen(this, std::move(V3I(1, 0, 0)), std::move(V3I(1, 1, 1)),
		std::move(V3(1, 0, 0)), vData, data, 20);
	/*generateFace(vertices, indices,
		Mage::Maths::Vector3(maxSize, minSize, minSize), Mage::Maths::Vector3(maxSize, maxSize, maxSize),
		Mage::Maths::Vector2(minTexCoord, minTexCoord), Mage::Maths::Vector2(maxTexCoord, maxTexCoord),
		Mage::Maths::Vector3(1, 0, 0), 20);*/

	GenerateMesh(vData, meshName);
	//generateMesh(vertices, indices, meshName);
}

void MeshGeneration::GenerateSphereMesh(const Mage::Maths::Vector3& center, const float radius, const int details, const std::string& meshName)
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

	auto GenerateSphereVertices = [](std::vector<Vertex>& vertices, const Mage::Maths::Vector3& center, const int details, Mage::Maths::Vector3 newPos, float& theta, float& theta1, float& cs, float& sn)
	{
		auto GenerateSphereColumn = [](std::vector<Vertex>& vertices, const Mage::Maths::Vector3& center, const int details, Mage::Maths::Vector3 newPos, float theta, float theta1, float cs, float sn, int offset, std::mutex& lock)
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
				//vertices.emplace_back(std::move(v));
				std::lock_guard<std::mutex> l(lock);
				size_t vertexPos = (2 + ((details * offset) + j));
				if (vertexPos > vertices.size())
				{
					std::cout << "here ye bugger" << "/n";
				}
				//vertices[vertices.begin() + (2 + ((details * offset) + j)] = std::move(v);
				//vertices.emplace(vertices.begin() + (2 + ((details * offset) + j)), std::move(v));
				//todo this beneath one
				//vertices.at(vertexPos) = std::move(v);
				vertices.emplace_back(v);
			}
			StackDebugging::GetInstance()->PopFunction();
		};
		std::stack<std::thread> columnThreads;
		std::mutex mutex;
		StackDebugging::GetInstance()->LogFunction("visualization::generateSphere::genVertices");
		float thetaChange = (360 / (details + 1)) * (PI / 180);
		theta1 = (180 / (details + 1)) * (PI / 180);
		for (int i = 0; i < details; i++)
		{
			theta = thetaChange * i;

			cs = cos(theta);
			sn = sin(theta);

			/*std::thread(GenerateSphereColumn, std::ref(vertices), std::ref(center),
				std::ref(details), std::ref(newPos), std::ref(theta), std::ref(theta1),
				std::ref(cs), std::ref(sn));
				*/
				/*
				columnThreads.emplace(GenerateSphereColumn, std::ref(vertices), std::ref(center),
					std::ref(details), std::ref(newPos), std::ref(theta), std::ref(theta1),
					std::ref(cs), std::ref(sn), std::ref(i), std::ref(mutex));
					*/
					/*
					std::thread t(GenerateSphereColumn, vertices, center, details,
						newPos, theta, theta1, cs, sn, i, mutex);
						*/
						/*std::thread t([](std::function<void>(
							std::vector<Vertex>&, const Mage::Maths::Vector3&,
							const Mage::Maths::Vector3&, int, Mage::Maths::Vector3,
							float, float, float, float, int, std::mutex&)
						{

						}*/

						//columnThreads.push(std::move(t));


			GenerateSphereColumn(vertices, center, details, newPos, theta, theta1, cs, sn, i, mutex);

			if (i == details - 1)
			{
				theta = 360 * (PI / 180);
				cs = cos(theta);
				sn = sin(theta);
				/*
				std::thread t(GenerateSphereColumn, vertices, center, details,
					newPos, theta, theta1, cs, sn, i, mutex);

				columnThreads.push(std::move(t));
				*/
				/*
				columnThreads.emplace(GenerateSphereColumn, std::ref(vertices), std::ref(center),
					std::ref(details), std::ref(newPos), std::ref(theta), std::ref(theta1),
					std::ref(cs), std::ref(sn), std::ref(i), std::ref(mutex));
					*/


					//GenerateSphereColumn(vertices, center, details, newPos, theta, theta1, cs, sn, mutex);

				GenerateSphereColumn(vertices, center, details, newPos, theta, theta1, cs, sn, i, mutex);
			}
		};
		while (!columnThreads.empty())
		{
			columnThreads.top().join();
			columnThreads.pop();
		}
		StackDebugging::GetInstance()->PopFunction();
		return;
	};
	VertexData vData;
	//std::vector<Vertex> vertices;
	/*
	vertices.reserve(2 + (details * details));
	vertices.resize(2 + (details * details));
	*/
	//std::vector<unsigned int> indices;


	vData.vertices.emplace_back();
	vData.vertices.emplace_back();

	//top
	vData.vertices[0].position = center + Mage::Maths::Vector3(0, radius, 0);
	vData.vertices[0].color = Mage::Maths::Vector3(0, 0, 0);
	vData.vertices[0].normal = Mage::Maths::Vector3(0, 1, 0);
	vData.vertices[0].texCoords = Mage::Maths::Vector2(0, 0);

	//bottom
	vData.vertices[1].position = center - Mage::Maths::Vector3(0, radius, 0);
	vData.vertices[1].color = Mage::Maths::Vector3(0, 0, 0);
	vData.vertices[1].normal = Mage::Maths::Vector3(0, -1, 0);
	vData.vertices[1].texCoords = Mage::Maths::Vector2(1, 1);

	//theta = y rotation
	//theta1 = z rotation
	float theta, theta1;
	float cs, cs1;
	float sn, sn1;

	Mage::Maths::Vector3 newPos;

	//vertex generation
	GenerateSphereVertices(vData.vertices, center, details, newPos, theta, theta1, cs, sn);

	GenerateSphereIndices(vData.indices, details);

	GenerateMesh(vData, meshName);
}

MeshGL* MeshGeneration::GetMesh(const std::string& meshName) const
{
	if (m_meshes.find(meshName) == m_meshes.end())
		return nullptr;

	return m_meshes.at(meshName);
}

std::unordered_map<std::string, MeshGL*>& MeshGeneration::GetAllMeshes()
{
	return m_meshes;
}

void MeshGeneration::LoadObject(const std::string& filePath, const std::string& fileName, const std::string& fileType)
{
	VertexData vData;
	//std::vector<Vertex> vertices;
	std::vector<Mage::Maths::Vector3> vertexPositions;
	std::vector<Mage::Maths::Vector3> vertexNormals;
	std::vector<Mage::Maths::Vector2> vertexTexcoords;
	std::vector<Mage::Maths::Vector3i> vertexElements;
	//std::vector<unsigned int> elements;
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
			Mage::Maths::Vector3& newVertexPosition = vertexPositions.back();
			reader >> newVertexPosition.x;
			reader >> newVertexPosition.y;
			reader >> newVertexPosition.z;
		}
		else if (readLine == "vn")
		{
			vertexNormals.emplace_back(Mage::Maths::Vector3());
			Mage::Maths::Vector3& newVertexNormal = vertexNormals.back();
			reader >> newVertexNormal.x;
			reader >> newVertexNormal.y;
			reader >> newVertexNormal.z;
		}
		else if (readLine == "vt")
		{
			vertexTexcoords.emplace_back(Mage::Maths::Vector2());
			Mage::Maths::Vector2& newVertexTexCoords = vertexTexcoords.back();
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
				if (vData.vertices.empty())
				{
					vData.vertices.emplace_back(newVertex);
					indices.emplace_back(0);
				}
				else
				{
					for (int j = 0; j < static_cast<int>(vData.vertices.size()); j++)
					{
						if (vData.vertices[j].position == newVertex.position && vData.vertices[j].normal == newVertex.normal && vData.vertices[j].texCoords == newVertex.texCoords)
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
						vData.vertices.emplace_back(newVertex);
						indices.emplace_back(static_cast<int>(vData.vertices.size()) - 1);
					}
				}
			}
			if (faceElements == 4)
			{
				vData.indices.emplace_back(indices[1]);
				vData.indices.emplace_back(indices[0]);
				vData.indices.emplace_back(indices[2]);

				vData.indices.emplace_back(indices[2]);
				vData.indices.emplace_back(indices[0]);
				vData.indices.emplace_back(indices[3]);
			}
			else if (faceElements == 3)
			{
				vData.indices.emplace_back(indices[1]);
				vData.indices.emplace_back(indices[0]);
				vData.indices.emplace_back(indices[2]);
			}
			indices.clear();
			faceElements = 0;
		}
	}
	reader.close();
	GenerateMesh(vData, fileName);
}

MeshGeneration::~MeshGeneration()
{
	for (auto i : m_meshes)
	{
		delete i.second;
	}
}

void MeshGeneration::GenerateFace(VertexData& data, const FaceData& fData)
{
	unsigned int newIndices[]{ 0, 1, 2, 2, 1, 3 };

	for (auto i : newIndices)
	{
		data.indices.emplace_back(i + fData.offset);
	}

	for (int i = 0; i < 4; i++)
	{
		data.vertices.emplace_back(Vertex());
		if (fData.minSize.x != fData.maxSize.x)
		{
			data.vertices[i + fData.offset].position = Mage::Maths::Vector3(
				(i % 2 == 0) ? fData.minSize.x : fData.maxSize.x,
				(i < 2) ? fData.minSize.y : fData.maxSize.y,
				(i < 2) ? fData.minSize.z : fData.maxSize.z);
		}
		else
		{
			data.vertices[i + fData.offset].position = Mage::Maths::Vector3(
				fData.minSize.x,
				(i < 2) ? fData.minSize.y : fData.maxSize.y,
				(i % 2 == 0) ? fData.minSize.z : fData.maxSize.z);
		}

		data.vertices[i + fData.offset].normal = fData.normal;
		data.vertices[i + fData.offset].color = Mage::Maths::Vector3(0, 0, 0);

		data.vertices[i + fData.offset].texCoords = Mage::Maths::Vector2(
			(i % 2 == 0) ? fData.minTexcoord.x : fData.maxTexcoord.x,
			(i < 2) ? fData.minTexcoord.y : fData.maxTexcoord.y);
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
