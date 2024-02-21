#pragma once

#include "../Common.h"

namespace Minecraft
{
	class VertexShader
	{
	public:
		VertexShader(const str& source);
		~VertexShader();

		uint GetID() const;

	private:
		uint m_ID = 0;
	};
}
