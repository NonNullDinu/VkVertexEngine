#pragma once

#include "Core/Core.h"

namespace Vertex
{

    class VX_PUBLIC_API Input
    {
    public:
        inline static bool IsKeyPressed(int key)
        {
            return s_Instance->IsKeyPressedImpl(key);
        }

        inline static bool IsMouseButtonPressed(int button)
        {
            return s_Instance->IsMouseButtonPressedImpl(button);
        }

        inline static float GetMouseX()
        {
            return s_Instance->GetMouseXImpl();
        }
        inline static float GetMouseY()
        {
            return s_Instance->GetMouseYImpl();
        }

        static void Init();

    protected:
        virtual bool IsKeyPressedImpl(int key) = 0;

        virtual bool IsMouseButtonPressedImpl(int button) = 0;

        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Input* s_Instance;
    };

}

#include "Platforms/Linux/LinuxInput.h"
#include "Platforms/Windows/WindowsInput.h"