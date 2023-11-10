#include "Application.h"
#include "hzpch.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case Hazel::ShaderDataType::Float:
                return GL_FLOAT;
            case Hazel::ShaderDataType::Float2:
                return GL_FLOAT;
            case Hazel::ShaderDataType::Float3:
                return GL_FLOAT;
            case Hazel::ShaderDataType::Float4:
                return GL_FLOAT;
            case Hazel::ShaderDataType::Mat3:
                return GL_FLOAT;
            case Hazel::ShaderDataType::Mat4:
                return GL_FLOAT;
            case Hazel::ShaderDataType::Int:
                return GL_INT;
            case Hazel::ShaderDataType::Int2:
                return GL_INT;
            case Hazel::ShaderDataType::Int3:
                return GL_INT;
            case Hazel::ShaderDataType::Int4:
                return GL_INT;
            case Hazel::ShaderDataType::Bool:
                return GL_BOOL;
        }

        HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    Application::Application()
    {
        HZ_CORE_ASSERT(s_Instance == nullptr, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        // application是单例的所以不用担心 ImGuiLayer 内存泄漏
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
                                 0.3f,  0.8f,  1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f};

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {{ShaderDataType::Float3, "a_Position"}, {ShaderDataType::Float4, "a_Color"}};

            m_VertexBuffer->SetLayout(layout);
        }

        uint32_t    index  = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.GetComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                  element.Normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  reinterpret_cast<const void*>(element.Offset));
            index++;
        }

        uint32_t indices[3] = {0, 1, 2};
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
            out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
                v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

        std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
            in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
			}
		)";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application() {}

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        // FIXME:trace
        // HZ_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
            {
                break;
            }
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            g_profiler.Frame();
            g_profiler.Begin(Profiler::Stage::Plot);
            glClearColor(0.2, 0.2, 0.2, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            g_profiler.Begin(Profiler::Stage::LayersOnUpdate);
            for (auto layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            g_profiler.End(Profiler::Stage::LayersOnUpdate);

            // 全局监听
            //            auto xy = Input::GetMousePosition();
            //            HZ_CORE_TRACE("{0}, {1}", xy.first, xy.second);

            // 渲染ImGui部分
            g_profiler.Begin(Profiler::Stage::LayersOnImGui);
            m_ImGuiLayer->Begin();
            for (auto layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            g_profiler.End(Profiler::Stage::LayersOnImGui);

            g_profiler.Begin(Profiler::Stage::WindowOnUpdate);
            m_Window->OnUpdate();
            g_profiler.End(Profiler::Stage::WindowOnUpdate);

            g_profiler.End(Profiler::Stage::Plot);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

} // namespace Hazel