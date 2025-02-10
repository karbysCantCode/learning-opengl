#include "TestTexture.h"
#include <iostream>

test::TestTexture::TestTexture(Renderer &renderer, float screenWidth, float screenHeight)
	: m_Vb(new VertexBuffer(m_Positions, sizeof(float) * 4 * 4))
	, m_Ib(new IndexBuffer(m_Indicies, 6))
	, m_Shader(new Shader(m_ShaderPath))
    , m_Texture(new Texture(m_TexturePath))
    , m_Renderer(renderer)
    , m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_Proj(glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f))
{
	m_Layout.Push<float>(2);
	m_Layout.Push<float>(2);
	m_Va.AddBuffer(*m_Vb, m_Layout);

    m_Shader->Bind();
    m_Texture->Bind();
    m_Shader->SetUniform1i("u_Texture", 0);
}

test::TestTexture::~TestTexture()
{
	delete m_Vb;
	delete m_Ib;
	delete m_Shader;
    delete m_Texture;
}

void test::TestTexture::OnUpdate(float deltaTime)
{
}

void test::TestTexture::OnRender()
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPosition);
    glm::mat4 mvp = m_Proj * m_View * model;
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    m_Renderer.Draw(m_Va, *m_Ib, *m_Shader);
}

struct InputTextCallback_UserData
{
    std::string* Str;
    ImGuiInputTextCallback  ChainCallback;
    void* ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

void test::TestTexture::OnImGuiRender()
{
	if (ImGui::CollapsingHeader("Texture"))
	{
        InputText("Shader File Path", &m_ShaderPath);
        /*ImGui::SameLine();
        if (ImGui::Button("Update to this path"))
        {

        }*/

        InputText("Texture File Path", &m_TexturePath);

        ImGui::SliderFloat3("Model Position", &m_ModelPosition.x, 0.0f, m_screenWidth);
	}
}
