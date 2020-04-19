#pragma once

#include "Core.h"
#include "Logger.h"

#include "Event/Event.h"

namespace Vertex {

	class VX_PUBLIC_API Application
	{
	public:
		Application();
		~Application();

		void Run();
	};

}