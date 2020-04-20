//
// Created by dinu on 4/20/20.
//

#include "Window.h"

#pragma once

#include "Core.h"

#include "Event/Event.h"

#include <GLFW/glfw3.h>

namespace Vertex {

	class WindowGLFW final : public Window {
	public:
		virtual void OnUpdate();

		virtual unsigned int GetWidth();

		virtual unsigned int GetHeight();

		template<typename T>
		void SetEventCallback(T &&func);

		virtual void SetVSync(bool conf);

		virtual bool IsVSync();

		virtual void *GetNativeWindow();

	private:
		GLFWwindow *m_window;
	};

	void WindowGLFW::OnUpdate()
	{
		glfwSwapBuffers(this->m_window);
		glfwPollEvents();
	}


	template<typename T>
	void WindowGLFW::SetEventCallback(T &&func)
	{

	}

	unsigned int WindowGLFW::GetWidth()
	{
		return 0;
	}

	unsigned int WindowGLFW::GetHeight()
	{
		return 0;
	}

	void WindowGLFW::SetVSync(bool conf)
	{

	}

	bool WindowGLFW::IsVSync()
	{
		return true; // default to true
	}

	void *WindowGLFW::GetNativeWindow()
	{
		return this->m_window;
	}
}
