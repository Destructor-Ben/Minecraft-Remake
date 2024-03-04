#include "Material.h"

namespace Minecraft
{
    Material::Material(Shader& shader) : m_Shader(shader) { }

    void Material::Bind()
    {
        m_Shader.Bind();
        m_Shader.SetUniform("uTransform", Transform);
    }
}
