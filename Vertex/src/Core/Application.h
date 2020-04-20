#pragma once

#include "Core.h"
#include "Logger.h"

#include "Event/Event.h"

/* For now, all platforms use the windows implementation. */
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
	#include "Platforms/Windows/WindowImpl.h"
#else
	#error Unsupported platform.
#endif

namespace Vertex {

	class VX_PUBLIC_API Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& event);

		void Run();

		inline Window& GetWindow() { return *m_Window; }

	private:
		std::shared_ptr<Window> m_Window;
	};

}