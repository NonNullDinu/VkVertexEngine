#pragma once

#include "Core/Input.h"

#include "Core/Application.h"

#ifndef VX_INCLUDED_INPUTS
    #define VX_INCLUDED_INPUTS
#else
    #error Already included platform specific inputs. Do not include multiple input classes.
#endif

namespace Vertex {

    class InputImpl : public Input
    {
    protected:
        bool IsKeyPressedImpl(int key) override;

        bool IsMouseButtonPressedImpl(int button) override;

        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
    };

}