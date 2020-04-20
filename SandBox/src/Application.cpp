#include <Vertex.h>

class SandBoxApp : public Vertex::Application
{
public:
	SandBoxApp()
	{
		Vertex::Logger::GetClientLogger()->info("Initializing Sandbox...");
	}

	~SandBoxApp()
	{
	}
};

Vertex::Application* Vertex::CreateApplication()
{
	return new SandBoxApp();
}