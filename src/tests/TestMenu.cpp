#include "TestMenu.h"

#include <iostream>
#include "ImGui/imgui.h"
#include "Renderer.h"

TestMenu::TestMenu(Renderer& renderer, float screenWidth, float screenHeight)
	: m_Renderer(renderer)
	, m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
{

}

TestMenu::~TestMenu()
{

}

void TestMenu::UpdateScene()
{
	if (m_TestScenesEnabled)
	{
		switch (m_CurrentTestScene)
		{
		case 0: // clear color scene
			if (m_SceneInstance != nullptr) delete m_SceneInstance;
			m_SceneInstance = new test::TestClearColor();
			break;
		case 1:
			if (m_SceneInstance != nullptr) delete m_SceneInstance;
			m_SceneInstance = new test::TestTexture(m_Renderer, m_screenWidth, m_screenHeight);
			break;
		default:
			break;
		}
	}
}

void TestMenu::OnRender()
{
	if (ImGui::CollapsingHeader("Test Scenes"))
	{
		ImGui::Checkbox("Test Scenes Enabled", &m_TestScenesEnabled);

		if (!m_TestScenesEnabled && m_SceneInstance != nullptr) {
			delete m_SceneInstance;
			m_SceneInstance = nullptr;
		}
		else if (m_TestScenesEnabled)
		{
			const int SceneAtFirst = m_CurrentTestScene;
			
			ImGui::SliderInt("Scene", &m_CurrentTestScene, 0, 1, m_SceneNames[m_CurrentTestScene]);
			
			if (m_TestScenesEnabled && m_SceneInstance == nullptr)
			{
				UpdateScene();
			}
			else if (SceneAtFirst != m_CurrentTestScene) // if changed
			{
				UpdateScene();
			}

			m_SceneInstance->OnUpdate(0);
			m_SceneInstance->OnRender();
			m_SceneInstance->OnImGuiRender();

		}
	}
}
