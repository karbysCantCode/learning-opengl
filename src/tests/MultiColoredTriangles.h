#pragma once
#include "Test.h"

namespace test {

	struct Triangle
	{
		float Position[3] = {0.0f, 0.0f, 0.0f};
		float Color[4]    = {0.0f ,0.0f, 0.0f, 0.0f};
		bool Updated = false;

		Triangle(const float pos[3], const float color[4], const bool updated = false)
			: Position{ pos[0],pos[1],pos[2] }
			, Color{ color[0], color[1], color[2], color[3] }
			, Updated(updated)
		{}
	};

	class TestMultiColoredTriangle : public test::Test
	{
	private:
		std::string m_ShaderPath = "res/shaders/MultiColoredTriangles.shader";

		Renderer& m_Renderer;

		VertexArray m_Va;
		VertexBuffer* m_Vb = nullptr;
		VertexBufferLayout m_Layout;
		IndexBuffer* m_Ib = nullptr;

		Shader* m_Shader = nullptr;

		int m_TriangleCount = 0;

		float m_screenWidth = 0;

		glm::mat4 m_Proj;

		std::vector<float> m_VertexData;
		std::vector<unsigned int> m_IndexData;

		void m_ClearAndUpdateBuffersToNewData();
		void m_UpdateBuffersToChangedData();
	public:
		std::vector<Triangle> Triangles;

		TestMultiColoredTriangle(Renderer& renderer, float screenWidth, float screenHeight);
		~TestMultiColoredTriangle();

		void OnRender();
		void AddTriangle(const float pos[3], const float color[4]);
		void OnImGuiRender();
		void OnUpdate(float deltaTime) override;
	};

}