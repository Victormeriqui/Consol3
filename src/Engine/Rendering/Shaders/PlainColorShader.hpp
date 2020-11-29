#ifndef PLAINCOLORSHADER_HPP
#define PLAINCOLORSHADER_HPP

#include "AbstractShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"


namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			class PlainColorShader : public AbstractShader
			{
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const override;
			};
		}
	}
}
#endif