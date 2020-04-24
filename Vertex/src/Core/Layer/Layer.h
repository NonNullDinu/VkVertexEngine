#pragma once

#include "Core/Core.h"
#include "Core/Event/Event.h"

namespace Vertex {

    class VX_PUBLIC_API Layer
    {
    public:
        Layer(const char* name = "layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}

        virtual void OnImguiRender() {}

        inline const char* GetName() { return m_Name; }

    protected:
        const char* m_Name; // this is supposed to be used for debugging only
    };

}