#pragma once

#include "../Common.h"

#include "VertexShader.h"
#include "FragmentShader.h"

namespace Minecraft
{
    // TODO: functions to set uniforms
	class Shader
	{
	public:
		Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader);
		~Shader();

		void Bind() const;

        void SetUniform(const string& name, const glm::mat4& value);

		static void Unbind();
        static Shader FromFile(const string& filePath);

	private:
        int GetUniformLocation(const string& name);

		uint m_ID = 0;
	};
}
