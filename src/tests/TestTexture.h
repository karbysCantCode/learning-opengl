#pragma once
#include "Test.h"

namespace test {
	class TestTexture : public Test
	{
	private:
		std::string m_ShaderPath = "res/shaders/Basic.shader";
		std::string m_TexturePath = "res/textures/The Cherno logo.png";
		float m_Positions[16] = {
			-50.0f, -50.0f, 0.0f, 0.0f, //0
			 50.0f, -50.0f, 1.0f, 0.0f, //1
			 50.0f,  50.0f, 1.0f, 1.0f, //2
			-50.0f,  50.0f, 0.0f, 1.0f  //3
		};

		unsigned int m_Indicies[6] = {
			0,1,2,
			2,3,0
		};

		Renderer& m_Renderer;

		VertexArray m_Va;
		VertexBuffer* m_Vb = nullptr;
		VertexBufferLayout m_Layout;
		IndexBuffer* m_Ib = nullptr;

		Shader* m_Shader = nullptr;

		Texture* m_Texture = nullptr;

		glm::mat4 m_Proj;
		glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::vec3 m_ModelPosition = { 0.0f,0.0f,0.0f };

		float m_screenWidth;
		float m_screenHeight;

	public:
		TestTexture(Renderer& renderer, float screenWidth, float screenHeight);
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender();
		void OnImGuiRender();


	};
}

