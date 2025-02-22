#include "MultiColoredTriangles.h"

#include <iostream>
void test::TestMultiColoredTriangle::m_ClearAndUpdateBuffersToNewData()
{
	m_VertexData.clear();
	m_IndexData.clear();
	m_VertexData.resize(Triangles.size() * 7 * 3);
	m_IndexData.resize(Triangles.size() * 3);
	for (int Index = 0; Index < Triangles.size(); Index++)
	{
		Triangle* CurrentTriangle = &Triangles[Index];

		CurrentTriangle->Updated = false;

		const float vertexData[3 * 7] =
		{
			CurrentTriangle->Position[0],CurrentTriangle->Position[1],CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],

			CurrentTriangle->Position[0] + 50.0f,CurrentTriangle->Position[1],CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],

			CurrentTriangle->Position[0],CurrentTriangle->Position[1] + 50.0f,CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3]
		};

		std::copy(std::begin(vertexData), std::end(vertexData), m_VertexData.begin() + (Index * 7 * 3));
		const unsigned int added = 3 * Index;
		const unsigned int indexData[3] =
		{
			added,
			1 + added,
			2 + added
		};

		std::copy(std::begin(indexData), std::end(indexData), m_IndexData.begin() + (Index * 3));
	}

	m_Vb->RewriteBuffer(m_VertexData.data(), sizeof(float) * 7 * 3 * Triangles.size(), GL_DYNAMIC_DRAW);
	m_Ib->RewriteBuffer(m_IndexData.data(), 3 * Triangles.size(), GL_DYNAMIC_DRAW);
}

void test::TestMultiColoredTriangle::m_UpdateBuffersToChangedData()
{
	m_VertexData.resize(Triangles.size() * 7 * 3);
	m_IndexData.resize(Triangles.size() * 3);
	for (int Index = 0; Index < Triangles.size(); Index++)
	{

		Triangle* CurrentTriangle = &Triangles[Index];
		if (!CurrentTriangle->Updated) continue;
		CurrentTriangle->Updated = false;
		//UPDATE VB
		// THIS SECTION IS TO COMPACT THE "TRIANGLE" struct into a continous list of float values so i can pass it to the gpu
		const float vertexData[3 * 7] =
		{
			// FIRST VERTEX (0,0,0 offset) -- THESE ARE X Y Z BTW
			CurrentTriangle->Position[0],CurrentTriangle->Position[1],CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],
			// SECOND VERTEX (50,0,0 offset)
			CurrentTriangle->Position[0] + 50.0f,CurrentTriangle->Position[1],CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],
			// THIRD VERTEX (0,50,0 offset)
			// issue was, third vertex had offset 0,0,50, and im using ortho graphic view, so z doesnt work hence it wasnt showing! and hence it was sideways
			CurrentTriangle->Position[0],CurrentTriangle->Position[1] + 50.0f,CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3]
		};

		std::copy(std::begin(vertexData), std::end(vertexData), m_VertexData.begin() + (Index * 7 * 3));

		m_Vb->UpdateSection(sizeof(float) * Index * 7 * 3, vertexData, sizeof(float) * 7 * 3, GL_DYNAMIC_DRAW);


		//UPDATE IB
		const unsigned int added = 3 * Index;
		const unsigned int indexData[3] =
		{
			added,
			1 + added,
			2 + added
		};

		std::copy(std::begin(indexData), std::end(indexData), m_IndexData.begin() + (Index * 3));

		m_Ib->UpdateSection(sizeof(unsigned int) * Index * 3, indexData, 3, GL_DYNAMIC_DRAW);
	}
}

test::TestMultiColoredTriangle::TestMultiColoredTriangle(Renderer& renderer, float screenWidth, float screenHeight)
	: m_Renderer(renderer)
	, m_Vb(new VertexBuffer(nullptr, 7*3*sizeof(float), GL_DYNAMIC_DRAW))
	, m_Ib(new IndexBuffer(nullptr, 0, GL_DYNAMIC_DRAW))
	, m_Shader(new Shader(m_ShaderPath))
	, m_screenWidth(screenWidth)
	, m_Proj(glm::perspective(glm::radians(90.0f), screenWidth/screenHeight, 0.1f, 1000.0f))
{
	m_Layout.Push<float>(3);
	m_Layout.Push<float>(4);
	m_Va.AddBuffer(*m_Vb, m_Layout);

	m_Shader->Bind();
}

test::TestMultiColoredTriangle::~TestMultiColoredTriangle()
{
	delete m_Vb;
	delete m_Ib;
	delete m_Shader;
}

void test::TestMultiColoredTriangle::OnRender()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1000.0f));

	m_Shader->SetUniformMat4f("u_MVP", m_Proj * model * view);
	m_Renderer.Draw(m_Va, *m_Ib, *m_Shader);
}

void test::TestMultiColoredTriangle::AddTriangle(const float pos[3], const float color[4])
{
	Triangles.emplace_back(pos, color, true);
	m_ClearAndUpdateBuffersToNewData();
}

void test::TestMultiColoredTriangle::OnImGuiRender()
{
	ImGui::Text("Counter: %d", m_TriangleCount); // Display the counter
	if (ImGui::Button("New Triangle")) {
		m_TriangleCount++; // Increment
							
		const float pos[3] = { 0.0f, 0.0f, 0.0f };
		const float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		AddTriangle(pos, color);
	}

	bool ClearFlag = false;
	bool UpdateFlag = false;

	for (int Index = 0; Index < Triangles.size(); Index++)
	{
		auto& CurrentTriangle = Triangles[Index];
		std::string label = "Triangle #" + std::to_string(Index) + " ##zz";
		if (ImGui::CollapsingHeader(label.c_str()))
		{
			if (ImGui::Button(("Delete Triangle ##" + std::to_string(Index)).c_str()))
			{
				Triangles.erase(Triangles.begin()+Index);
				m_TriangleCount--;
				Index--;
				ClearFlag = true;
				continue;
			}
			if (ImGui::SliderFloat3(("Position ##" + std::to_string(Index)).c_str(), CurrentTriangle.Position, -960, 960))
			{
				CurrentTriangle.Updated = true;
				UpdateFlag = true;
			}
			if (ImGui::ColorEdit4(("Color ##" + std::to_string(Index)).c_str(), CurrentTriangle.Color, ImGuiColorEditFlags_Float))
			{
				std::cout << CurrentTriangle.Color[0] << " " << CurrentTriangle.Color[1] << " " << CurrentTriangle.Color[2] << " " << CurrentTriangle.Color[3] << std::endl;
				CurrentTriangle.Updated = true;
				UpdateFlag = true;
			}
		}
	}
	if (ClearFlag) 
	{
		m_ClearAndUpdateBuffersToNewData();
		return;
	}
	if (UpdateFlag)
	{
		m_UpdateBuffersToChangedData();
		return;
	}
}

void test::TestMultiColoredTriangle::OnUpdate(float deltaTime)
{
}
