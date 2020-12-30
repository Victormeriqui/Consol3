#include "ShadedColorShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Lighting/LightingSystem.hpp"

#include <algorithm>
#include <string>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Lighting;

			void ShadedColorShader::SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system)
			{
				this->lighting_system = lighting_system;
			}

			bool ShadedColorShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
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

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			void ShadedColorShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				// perspective correct
				float v0_light_amount_pc = v0_light_amount * triangle.v0_oneoverw;
				float v1_light_amount_pc = v1_light_amount * triangle.v1_oneoverw;
				float v2_light_amount_pc = v2_light_amount * triangle.v2_oneoverw;

				float light_amount = (barcoord0 * v0_light_amount_pc) + (barcoord1 * v1_light_amount_pc) + (barcoord2 * v2_light_amount_pc);

				float perspective = (barcoord0 * triangle.v0_oneoverw) + (barcoord1 * triangle.v1_oneoverw) + (barcoord2 * triangle.v2_oneoverw);
				light_amount /= perspective;

				out_color = HSVColor(out_color.hue, out_color.saturation, std::min(light_amount + 0.02f, 1.0f));
			}
		}
	}
}