#include <Vertex.h>

class SandBoxApp : public Vertex::Application
{
public:
	SandBoxApp()
	{
	}

	~SandBoxApp()
	{
	}
};

Vertex::Application* Vertex::CreateApplication()
{
	return new SandBoxApp();
}