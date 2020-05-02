#include <Vertex.h>

class TestLayer : public Vertex::Layer
{
public:
	TestLayer(const char* name = "TestLayer")
		: Layer(name)
	{
		Vertex::ClientLogger::Info("Initialized test layer");
	}

	void OnUpdate() override
	{
		// Vertex::Logger::GetClientLogger()->info("{0} update", GetName()); // <- eyes hurt
	}

	void OnEvent(Vertex::Event& event) override
	{
		// Vertex::Logger::GetClientLogger()->debug("{0} event : {1}", GetName(), event.GetDetails());
	}
};

class SandBoxApp : public Vertex::Application
{
public:
	SandBoxApp()
	{
		PushLayer(new TestLayer());
	}

	~SandBoxApp()
	{
	}
};

Vertex::Application* Vertex::CreateApplication()
{
	return new SandBoxApp();
}