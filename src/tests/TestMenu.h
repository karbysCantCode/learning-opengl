#pragma once
#include "TestClearColor.h"
#include "tests/TestTexture.h"

class TestMenu
{
private:
	bool m_TestScenesEnabled = false;
	int m_CurrentTestScene = 0;
	test::Test* m_SceneInstance = nullptr;
	enum Scenes
	{
		TEXTURE,
		CLEAR_COLOR,
		MAX_SCENE
	};
	const char* m_SceneNames[2] = { "Clear color", "Texture" };
	Renderer& m_Renderer;
	float m_screenWidth;
	float m_screenHeight;


	void UpdateScene();

public:
	TestMenu(Renderer& renderer, float screenWidth, float screenHeight);
	~TestMenu();

	void OnRender();
};

