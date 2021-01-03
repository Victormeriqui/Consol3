#include "PlainTextureShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector2.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Texture.hpp"

#include <string>
namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			void PlainTextureShader::SetTexture(std::shared_ptr<Texture> texture)
			{
				this->texture = texture;
			}

			bool PlainTextureShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
			{
				TransformVertexMVP(v0, mvp_mats);
				TransformVertexMVP(v1, mvp_mats);
				TransformVertexMVP(v2, mvp_mats);

				vert_v0_texture_coord = v0.GetTextureCoords();
				vert_v1_texture_coord = v1.GetTextureCoords();
				vert_v2_texture_coord = v2.GetTextureCoords();

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			void PlainTextureShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2)
			{
				Vector2 frag_texture_coord = PerspectiveCorrectInterpolate<Vector2>(vert_v0_texture_coord, vert_v1_texture_coord, vert_v2_texture_coord, triangle, barcoord0, barcoord1, barcoord2);

				out_color = HSVColor(texture->GetColorFromTextureCoords(frag_texture_coord.x, frag_texture_coord.y));
			}
		}
	}
}