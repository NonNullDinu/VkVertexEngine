#pragma once

#include "Core/Input.h"

#include "Core/Application.h"

namespace Vertex
{

    class WindowsInput : public Input
    {
    protected:
        bool IsKeyPressedImpl(int key) override;

        bool IsMouseButtonPressedImpl(int button) override;

        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
    };

}