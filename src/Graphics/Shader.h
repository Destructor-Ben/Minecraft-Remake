#pragma once

#include "../Common.h"

#include "VertexShader.h"
#include "FragmentShader.h"

namespace Minecraft
{
    // TODO: functions to set uniforms
    // TODO: options to take in shaders already and also make FromFile functions so u can specify either a filepath or source
	class Shader
	{
	public:
		Shader(const str& filePath);
		~Shader();

		void Bind() const;

		static void Unbind();

	private:
		uint m_ID = 0;
		str m_FilePath;
	};
}
