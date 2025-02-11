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

			CurrentTriangle->Position[0],CurrentTriangle->Position[1],CurrentTriangle->Position[2] + 50.0f,CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],
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
		const float vertexData[3 * 7] =
		{
			CurrentTriangle->Position[0],CurrentTriangle->Position[1],CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],

			CurrentTriangle->Position[0] + 50.0f,CurrentTriangle->Position[1],CurrentTriangle->Position[2],CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],

			CurrentTriangle->Position[0],CurrentTriangle->Position[1],CurrentTriangle->Position[2] + 50.0f,CurrentTriangle->Color[0],
			CurrentTriangle->Color[1],CurrentTriangle->Color[2],CurrentTriangle->Color[3],
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

		m_Ib->UpdateSection(sizeof(unsigned int) * Index * 3, indexData, sizeof(unsigned int) * 3, GL_DYNAMIC_DRAW);
	}
}

test::TestMultiColoredTriangle::TestMultiColoredTriangle(Renderer& renderer, float screenWidth, float screenHeight)
	: m_Renderer(renderer)
	, m_Vb(new VertexBuffer(0, 0, GL_DYNAMIC_DRAW))
	, m_Ib(new IndexBuffer(0, 0, GL_DYNAMIC_DRAW))
	, m_Shader(new Shader(m_ShaderPath))
	, m_screenWidth(screenWidth)
	, m_Proj(glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f))
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
	m_Shader->SetUniformMat4f("u_MVP", m_Proj);
	m_Renderer.Draw(m_Va, *m_Ib, *m_Shader);
}

void test::TestMultiColoredTriangle::AddTriangle(const float pos[3], const float color[4])
{
	Triangles.emplace_back(pos, color, true);
	m_UpdateBuffersToChangedData();
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
		if (ImGui::CollapsingHeader("Triangle #" + Index))
		{
			if (ImGui::Button("Delete Triangle"))
			{
				Triangles.erase(Triangles.begin()+Index);
				ClearFlag = true;
				continue;
			}
			if (ImGui::SliderFloat3("Position", CurrentTriangle.Position, 0, 960))
			{
				CurrentTriangle.Updated = true;
				UpdateFlag = true;
			}
			if (ImGui::ColorEdit4("Color", CurrentTriangle.Color, ImGuiColorEditFlags_Float))
			{
				std::cout << CurrentTriangle.Color[0] << " " << CurrentTriangle.Color[1] << " " << CurrentTriangle.Color[2] << " " << CurrentTriangle.Color[3] << std::endl;
				CurrentTriangle.Updated = true;
				UpdateFlag = true;
			}
			if (ImGui::Button("Breakpoint button"))
			{
				std::cout << "BP\n";
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
