#include "Application.h"

namespace Vertex {

	Application::Application()
	{
		m_Window.reset(new Window());
		m_Window->SetEventCallback(VX_BIND_FUNC_1(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& event)
	{
	}

	void Application::Run()
	{
		while (true);
	}

}