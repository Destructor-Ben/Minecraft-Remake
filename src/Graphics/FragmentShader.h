#pragma once

#include "../Common.h"

namespace Minecraft
{
	class FragmentShader
	{
	public:
		explicit FragmentShader(const string& source);
		~FragmentShader();

		uint32 GetID() const;

        static FragmentShader FromFile(const string& filePath);

	private:
		uint32 m_ID = 0;
	};
}
