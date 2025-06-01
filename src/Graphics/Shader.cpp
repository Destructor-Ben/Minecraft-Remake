#include "Shader.h"

#include "Game.h"
#include "Logger.h"
#include "Graphics/VertexShader.h"
#include "Graphics/FragmentShader.h"
#include "Graphics/Texture.h"

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
    }

    #pragma region Uniforms

    #define UNIFORM_FUNCTION(type, function) \
    void Shader::SetUniform(const string& name, type value)\
    {\
        int location = GetUniformLocation(name);\
        function;\
    }

    UNIFORM_FUNCTION(float, glUniform1f(location, value))
    UNIFORM_FUNCTION(int, glUniform1i(location, value))
    UNIFORM_FUNCTION(uint, glUniform1ui(location, value))
    UNIFORM_FUNCTION(bool, glUniform1i(location, value)) // Booleans are actually ints in OpenGL

    UNIFORM_FUNCTION(vec2, glUniform2fv(location, 1, glm::value_ptr(value)))
    UNIFORM_FUNCTION(vec2i, glUniform2iv(location, 1, glm::value_ptr(value)))

    UNIFORM_FUNCTION(vec3, glUniform3fv(location, 1, glm::value_ptr(value)))
    UNIFORM_FUNCTION(vec3i, glUniform3iv(location, 1, glm::value_ptr(value)))

    UNIFORM_FUNCTION(vec4, glUniform4fv(location, 1, glm::value_ptr(value)))
    UNIFORM_FUNCTION(vec4i, glUniform4iv(location, 1, glm::value_ptr(value)))

    UNIFORM_FUNCTION(mat2, glUniformMatrix2fv(location, 1, false, glm::value_ptr(value)))
    UNIFORM_FUNCTION(mat3, glUniformMatrix3fv(location, 1, false, glm::value_ptr(value)))
    UNIFORM_FUNCTION(mat4, glUniformMatrix4fv(location, 1, false, glm::value_ptr(value)))

    // Textures and cube maps are different because they also have a slot parameter
    void Shader::SetUniform(const string& name, shared_ptr <Texture> value, int slot)
    {
        value->BindTextureUnit(slot);
        SetUniform(name, slot);
    }

    #pragma endregion

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    int Shader::GetUniformLocation(const string& name)
    {
        if (m_UniformCache.contains(name))
            return m_UniformCache.at(name);

        int location = glGetUniformLocation(m_ID, name.c_str());
        if (location == -1)
            Logger::Warn(format("Uniform '{}' wasn't found - Location was -1", name));

        m_UniformCache[name] = location;
        return location;
    }
}
