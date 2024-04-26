#pragma once

#include "../Common.h"
#include "GraphicsResource.h"

namespace Minecraft
{
	class FragmentShader : public GraphicsResource
	{
	public:
		explicit FragmentShader(const string& source);
		~FragmentShader();

		uint32 GetID() const;

	private:
		uint32 m_ID = 0;
	};
}
