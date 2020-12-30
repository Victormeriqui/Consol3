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

				v0_texture_coord = v0.GetTextureCoords();
				v1_texture_coord = v1.GetTextureCoords();
				v2_texture_coord = v2.GetTextureCoords();

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			void PlainTextureShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				// perspective correct
				Vector2 v0_texture_coord_pc = v0_texture_coord * triangle.v0_oneoverw;
				Vector2 v1_texture_coord_pc = v1_texture_coord * triangle.v1_oneoverw;
				Vector2 v2_texture_coord_pc = v2_texture_coord * triangle.v2_oneoverw;

				float u = v0_texture_coord_pc.x * barcoord0 + v1_texture_coord_pc.x * barcoord1 + v2_texture_coord_pc.x * barcoord2;
				float v = v0_texture_coord_pc.y * barcoord0 + v1_texture_coord_pc.y * barcoord1 + v2_texture_coord_pc.y * barcoord2;

				float oneoverperspective = 1.0f / ((barcoord0 * triangle.v0_oneoverw) + (barcoord1 * triangle.v1_oneoverw) + (barcoord2 * triangle.v2_oneoverw));
				
				u *= oneoverperspective;
				v *= oneoverperspective;

				RGBColor texel_rgb = texture->GetColorFromTextureCoords(u, v);
				
				out_color = HSVColor(texel_rgb);
			}
		}
	}
}