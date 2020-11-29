#include "ShadedTextureShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
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

			void ShadedTextureShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				std::shared_ptr<LightingSystem> lighting_system = std::static_pointer_cast<LightingSystem>(GetFragmentData(std::string("lighting_system")));
				std::shared_ptr<Texture> texture = std::static_pointer_cast<Texture>(GetFragmentData(std::string("texture")));

				float v0_light = lighting_system->GetLightAmountAt(triangle.v0);
				float v1_light = lighting_system->GetLightAmountAt(triangle.v1);
				float v2_light = lighting_system->GetLightAmountAt(triangle.v2);

				Vector2 v0_texture_coord = triangle.v0.GetTextureCoords();
				Vector2 v1_texture_coord = triangle.v1.GetTextureCoords();
				Vector2 v2_texture_coord = triangle.v2.GetTextureCoords();

				v0_light *= triangle.v0_oneoverw;
				v1_light *= triangle.v1_oneoverw;
				v2_light *= triangle.v2_oneoverw;

				v0_texture_coord *= triangle.v0_oneoverw;
				v1_texture_coord *= triangle.v1_oneoverw;
				v2_texture_coord *= triangle.v2_oneoverw;

				float light_amount = (barcoord0 * v0_light) + (barcoord1 * v1_light) + (barcoord2 * v2_light);

				float u = v0_texture_coord.x * barcoord0 + v1_texture_coord.x * barcoord1 + v2_texture_coord.x * barcoord2;
				float v = v0_texture_coord.y * barcoord0 + v1_texture_coord.y * barcoord1 + v2_texture_coord.y * barcoord2;

				float oneoverperspective = 1.0f / ((barcoord0 * triangle.v0_oneoverw) + (barcoord1 * triangle.v1_oneoverw) + (barcoord2 * triangle.v2_oneoverw));

				
				light_amount *= oneoverperspective;
				u *= oneoverperspective;
				v *= oneoverperspective;
				
				HSVColor texel_hsv = HSVColor(texture->GetColorFromUV(u, v));
				out_color = HSVColor(texel_hsv.hue, texel_hsv.saturation, std::min(light_amount + 0.02f, 1.0f));
			}
		}
	}
}