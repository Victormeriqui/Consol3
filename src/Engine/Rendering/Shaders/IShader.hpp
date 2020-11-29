#ifndef ISHADER_HPP
#define ISHADER_HPP

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Triangle.hpp"

#include <map>
#include <string>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;

			class IShader
			{
			public:
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const = 0;
			};
		}
	}
}

#endif