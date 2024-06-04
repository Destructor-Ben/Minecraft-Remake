#include "Shader.h"

#include "Game.h"
#include "Graphics/VertexShader.h"
#include "Graphics/FragmentShader.h"

namespace Minecraft
{
    Shader::Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader) : m_ID(0)
    {
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader.GetID());
        glAttachShader(m_ID, fragmentShader.GetID());
        glLinkProgram(m_ID);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_ID);
    }

    void Shader::Bind()
    {
        glUseProgram(m_ID);
        SetUniform("", 0.0f);
    }

    #pragma region Uniforms

    template<typename T>
    void Shader::SetUniform(const string& name, T value)
    {
        Logger->Throw("Unsupported data type for setting uniforms");
    }

    #define UNIFORM_FUNCTION(type, function) \
    template<>\
    void Shader::SetUniform<type>(const string& name, type value)\
    {\
        int32 location = GetUniformLocation(name);\
        function;\
    }

    UNIFORM_FUNCTION(float32, glUniform1f(location, value))
    UNIFORM_FUNCTION(int32, glUniform1i(location, value))
    UNIFORM_FUNCTION(uint32, glUniform1ui(location, value))

    UNIFORM_FUNCTION(vec2, glUniform2fv(location, 1, &value[0]))
    UNIFORM_FUNCTION(vec2i, glUniform2iv(location, 1, &value[0]))

    UNIFORM_FUNCTION(vec3, glUniform3fv(location, 1, &value[0]))
    UNIFORM_FUNCTION(vec3i, glUniform3iv(location, 1, &value[0]))

    UNIFORM_FUNCTION(vec4, glUniform4fv(location, 1, &value[0]))
    UNIFORM_FUNCTION(vec4i, glUniform4iv(location, 1, &value[0]))

    UNIFORM_FUNCTION(mat2, glUniformMatrix2fv(location, 1, false, &value[0][0]))
    UNIFORM_FUNCTION(mat3, glUniformMatrix3fv(location, 1, false, &value[0][0]))
    UNIFORM_FUNCTION(mat4, glUniformMatrix4fv(location, 1, false, &value[0][0]))

    #pragma endregion

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    int32 Shader::GetUniformLocation(const string& name)
    {
        return glGetUniformLocation(m_ID, name.c_str());
    }
}
