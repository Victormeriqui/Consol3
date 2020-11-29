#include "ShadedColorShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
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

			void ShadedColorShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				std::shared_ptr<LightingSystem> lighting_system = std::static_pointer_cast<LightingSystem>(GetFragmentData(std::string("lighting_system")));

				float v0_light = lighting_system->GetLightAmountAt(triangle.v0);
				float v1_light = lighting_system->GetLightAmountAt(triangle.v1);
				float v2_light = lighting_system->GetLightAmountAt(triangle.v2);

				v0_light *= triangle.v0_oneoverw;
				v1_light *= triangle.v1_oneoverw;
				v2_light *= triangle.v2_oneoverw;

				float light_amount = (barcoord0 * v0_light) + (barcoord1 * v1_light) + (barcoord2 * v2_light);

				float perspective = (barcoord0 * triangle.v0_oneoverw) + (barcoord1 * triangle.v1_oneoverw) + (barcoord2 * triangle.v2_oneoverw);
				light_amount /= perspective;

				out_color = HSVColor(out_color.hue, out_color.saturation, std::min(light_amount + 0.02f, 1.0f));
			}
		}
	}
}