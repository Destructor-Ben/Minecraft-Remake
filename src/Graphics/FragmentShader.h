#pragma once

#include "../Common.h"

namespace Minecraft
{
	class FragmentShader
	{
	public:
		explicit FragmentShader(const str& source);
		~FragmentShader();

		uint GetID() const;

        static FragmentShader FromFile(const str& filePath);

	private:
		uint m_ID = 0;
	};
}
