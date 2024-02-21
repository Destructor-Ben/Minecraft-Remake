#pragma once

#include "../Common.h"

namespace Minecraft
{
	class FragmentShader
	{
	public:
		FragmentShader(const str& source);
		~FragmentShader();

		uint GetID() const;

	private:
		uint m_ID = 0;
	};
}
