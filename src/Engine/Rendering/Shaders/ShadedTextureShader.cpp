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

				v0_light_amount = lighting_system->GetLightAmountAt(v0);
				v1_light_amount = lighting_system->GetLightAmountAt(v1);
				v2_light_amount = lighting_system->GetLightAmountAt(v2);

				TransformVertexViewProjection(v0, mvp_mats);
				TransformVertexViewProjection(v1, mvp_mats);
				TransformVertexViewProjection(v2, mvp_mats);

				v0_texture_coord = v0.GetTextureCoords();
				v1_texture_coord = v1.GetTextureCoords();
				v2_texture_coord = v2.GetTextureCoords();

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			void ShadedTextureShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				// perspective correct
				Vector2 v0_texture_coord_pc = v0_texture_coord * triangle.v0_oneoverw;
				Vector2 v1_texture_coord_pc = v1_texture_coord * triangle.v1_oneoverw;
				Vector2 v2_texture_coord_pc = v2_texture_coord * triangle.v2_oneoverw;
				
				float v0_light_amount_pc = v0_light_amount * triangle.v0_oneoverw;
				float v1_light_amount_pc = v1_light_amount * triangle.v1_oneoverw;
				float v2_light_amount_pc = v2_light_amount * triangle.v2_oneoverw;

				float light_amount = (barcoord0 * v0_light_amount_pc) + (barcoord1 * v1_light_amount_pc) + (barcoord2 * v2_light_amount_pc);

				float u = v0_texture_coord_pc.x * barcoord0 + v1_texture_coord_pc.x * barcoord1 + v2_texture_coord_pc.x * barcoord2;
				float v = v0_texture_coord_pc.y * barcoord0 + v1_texture_coord_pc.y * barcoord1 + v2_texture_coord_pc.y * barcoord2;

				float oneoverperspective = 1.0f / ((barcoord0 * triangle.v0_oneoverw) + (barcoord1 * triangle.v1_oneoverw) + (barcoord2 * triangle.v2_oneoverw));

				
				light_amount *= oneoverperspective;
				u *= oneoverperspective;
				v *= oneoverperspective;
				
				HSVColor texel_hsv = HSVColor(texture->GetColorFromTextureCoords(u, v));
				out_color = HSVColor(texel_hsv.hue, texel_hsv.saturation, std::min(light_amount + 0.02f, 1.0f));
			}
		}
	}
}