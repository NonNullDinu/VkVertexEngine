#pragma once

#include "Core.h"
#include "Logger.h"

#include "Event/Event.h"

#if defined(_WIN32)
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

		void Run();
	};

}