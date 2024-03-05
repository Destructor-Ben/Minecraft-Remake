#pragma once

#include "../Common.h"

namespace Minecraft
{
	class VertexShader
	{
	public:
        explicit VertexShader(const string& source);
		~VertexShader();

		uint GetID() const;

        static VertexShader FromFile(const string& filePath);

	private:
		uint m_ID = 0;
	};
}
