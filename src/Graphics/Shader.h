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
        void SetUniform(const string& name, int32 value);

		static void Unbind();

	private:
        int GetUniformLocation(const string& name);

		uint32 m_ID = 0;
	};
}
