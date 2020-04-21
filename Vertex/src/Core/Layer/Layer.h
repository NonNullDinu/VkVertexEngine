#pragma once

#include "Core/Core.hpp"
#include "Core/Event/Event.hpp"

namespace Vertex {

    class Layer
    {
    public:
        Layer(const char* name = "layer")
            : m_Name(name)
        {
        }

        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}

        inline const char* GetName() { return m_Name; }

    protected:
        const char* m_Name; // this is supposed to be used for debugging only
    };

}