#pragma once

#include "Application.h"

namespace Vertex
{

    extern Application* CreateApplication();

}

int main(int argc, char** argv)
{
    auto app = Vertex::CreateApplication();
    app->Run();
    delete app;
}