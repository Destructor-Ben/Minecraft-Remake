#pragma once

#include "../Common.h"

#include "VertexShader.h"
#include "FragmentShader.h"

namespace Minecraft
{
    // TODO: functions to set uniforms
    // TODO: shaders in /res dont get copied again after changes
	class Shader
	{
	public:
		Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader);
		~Shader();

		void Bind() const;
        uint GetID() const;

		static void Unbind();
        // TODO: ideally this wont return a pointer - also fix for other shaders
        static Shader* FromFile(const str& filePath);

	private:
		uint m_ID = 0;
	};
}
