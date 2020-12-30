#include "PlainColorShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			bool PlainColorShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
			{
				TransformVertexMVP(v0, mvp_mats);
				TransformVertexMVP(v1, mvp_mats);
				TransformVertexMVP(v2, mvp_mats);

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			void PlainColorShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{

			}
		}
	}
}