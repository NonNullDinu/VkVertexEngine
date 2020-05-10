#include "OpenGLVertexArray.h"

#include "OpenGLShader.h"

namespace Vertex
{

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_ID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {

    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vbo)
    {
        Bind();

        vbo->Bind();

        uint16_t index = 0;
        BufferLayout layout = vbo->GetLayout();
        for (const auto& elem : layout)
        {
            glEnableVertexAttribArray(index);

            glVertexAttribPointer(index,
                                  elem.component_count,
                                  ShaderDataTypeToOpenGL(elem.type),
                                  elem.normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  (const void*)elem.offset
            );

            index++;
        }

        m_VBOs.emplace_back(vbo);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> ibo)
    {
        Bind();
        ibo->Bind();
        m_IBO = ibo;
    }

}