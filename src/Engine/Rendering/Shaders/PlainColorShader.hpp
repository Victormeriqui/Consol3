#ifndef PLAINCOLORSHADER_HPP
#define PLAINCOLORSHADER_HPP

#include "IShader.hpp"

#include "../../../Display/HSVColor.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../Vertex.hpp"

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;

			class PlainColorShader : public IShader
			{
				virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats) override;
				virtual void FragmentShader(
					HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) override;
			};
		}
	}
}
#endif
