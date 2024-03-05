#pragma once

#include "../Common.h"

namespace Minecraft
{
	class FragmentShader
	{
	public:
		explicit FragmentShader(const string& source);
		~FragmentShader();

		uint GetID() const;

        static FragmentShader FromFile(const string& filePath);

	private:
		uint m_ID = 0;
	};
}
