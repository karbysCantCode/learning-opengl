#pragma once
#include "TestClearColor.h"
#include "TestTexture.h"
#include "MultiColoredTriangles.h"

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
	const char* m_SceneNames[3] = { "Clear color", "Texture", "Multi Colored Triangles!"};
	Renderer& m_Renderer;
	float m_screenWidth;
	float m_screenHeight;


	void UpdateScene();

public:
	TestMenu(Renderer& renderer, float screenWidth, float screenHeight);
	~TestMenu();

	void OnRender();
};

