#pragma once

#include "Core.h"
#include "Logger.h"

#include "Event/Event.h"

#if defined(_WIN32)
#include "Platforms/Windows/WindowImpl.h"
#elif defined(__linux__)

#include "Platforms/Linux/GLFWWindowImpl.h"

#else
#error Unsupported platform.
#endif

namespace Vertex {

	class VX_PUBLIC_API Application {
	public:
		Application();

		~Application();

		void OnEvent(Event &event);

		void Run();

		inline IWindow &GetWindow() { return *m_Window; }

	private:
		std::shared_ptr<IWindow> m_Window;
	};

}