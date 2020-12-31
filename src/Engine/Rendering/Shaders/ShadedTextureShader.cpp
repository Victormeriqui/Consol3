#include "ShadedTextureShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector2.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Texture.hpp"
#include "../Lighting/LightingSystem.hpp"

#include <algorithm>
#include <string>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Lighting;

			void ShadedTextureShader::SetTexture(std::shared_ptr<Texture> texture)
			{
				this->texture = texture;
			}

			void ShadedTextureShader::SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system)
			{
				this->lighting_system = lighting_system;
			}

			bool ShadedTextureShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
			{
				TransformVertexModel(v0, mvp_mats);
				TransformVertexModel(v1, mvp_mats);
				TransformVertexModel(v2, mvp_mats);

				vert_v0_position = v0.GetPosition();
				vert_v1_position = v1.GetPosition();
				vert_v2_position = v2.GetPosition();

				vert_v0_normal = v0.GetNormal();
				vert_v1_normal = v1.GetNormal();
				vert_v2_normal = v2.GetNormal();

				TransformVertexViewProjection(v0, mvp_mats);
				TransformVertexViewProjection(v1, mvp_mats);
				TransformVertexViewProjection(v2, mvp_mats);

				vert_v0_texture_coord = v0.GetTextureCoords();
				vert_v1_texture_coord = v1.GetTextureCoords();
				vert_v2_texture_coord = v2.GetTextureCoords();

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			void ShadedTextureShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				Vector3 frag_position = PerspectiveCorrectInterpolate<Vector3>(vert_v0_position, vert_v1_position, vert_v2_position, triangle, barcoord0, barcoord1, barcoord2);
				Vector3 frag_normal = PerspectiveCorrectInterpolate<Vector3>(vert_v0_normal, vert_v1_normal, vert_v2_normal, triangle, barcoord0, barcoord1, barcoord2);

				Vector2 frag_texture_coord = PerspectiveCorrectInterpolate<Vector2>(vert_v0_texture_coord, vert_v1_texture_coord, vert_v2_texture_coord, triangle, barcoord0, barcoord1, barcoord2);
				
				float light_amount = lighting_system->GetLightAmountAt(frag_position, frag_normal);
				HSVColor texel_hsv = HSVColor(texture->GetColorFromTextureCoords(frag_texture_coord.x, frag_texture_coord.y));

				out_color = HSVColor(texel_hsv.hue, texel_hsv.saturation, texel_hsv.value * std::min(light_amount + 0.02f, 1.0f));
			}
		}
	}
}