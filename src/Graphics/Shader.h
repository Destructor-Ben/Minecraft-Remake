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
		Shader(const str& filePath);
		~Shader();

		void Bind() const;

		static void Unbind();

	private:
		uint m_ID = 0;
		str m_FilePath;
	};
}
