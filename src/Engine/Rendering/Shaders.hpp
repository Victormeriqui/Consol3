#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "../../Display/IRenderer.hpp"
#include "Vertex.hpp"
#include "../../Display/HSVColor.hpp"
#include "DepthBuffer.hpp"
#include "../Rendering/Lighting/LightingSystem.hpp"
#include "Rasterizer.hpp"

#include <algorithm>
#include <memory>

// TODO: find a better way to do this, repeating it in the Rasterizer methods arguments is not an option either
// arguments received by every shader lambda:
// x, y, z, lighting_system, renderer, barycentric coord 0, barycentric coord 1, barycentric coord 2, vertex 0, vertex 1, vertex 2, color
#define SHADER_ARGUMENTS uint16_t, uint16_t, float, std::shared_ptr<LightingSystem>, std::shared_ptr<IRenderer>, float, float, float, const Vertex& v0, const Vertex& v1, const Vertex& v2, const HSVColor&

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;
			using namespace Math;
			using namespace Lighting;

			// TODO: maybe all these arguments can be wrapped in a ShaderData or something similar
			auto shaded_color_shader([](
				uint16_t x,
				uint16_t y,
				float z,
				std::shared_ptr<LightingSystem> lighting_system,
				std::shared_ptr<IRenderer> renderer,
				float barcoord0,
				float barcoord1,
				float barcoord2,
				const Vertex& v0,
				const Vertex& v1,
				const Vertex& v2,
				const HSVColor& color)
			{
				float light_amount = barcoord0 * lighting_system->GetLightAmountAt(v0) +
									barcoord1 * lighting_system->GetLightAmountAt(v1) +
									barcoord2 * lighting_system->GetLightAmountAt(v2);

				HSVColor lit_color = HSVColor(color.hue, color.saturation, std::min(1.0f, color.value + light_amount + 0.02f));
				renderer->SetPixel(x, y, lit_color);
			});

			auto fixed_color_shader([](
				uint16_t x,
				uint16_t y,
				float z,
				std::shared_ptr<LightingSystem> lighting_system,
				std::shared_ptr<IRenderer> renderer,
				float barcoord0,
				float barcoord1,
				float barcoord2,
				const Vertex& v0,
				const Vertex& v1,
				const Vertex& v2,
				const HSVColor& color)
			{
				renderer->SetPixel(x, y, color);
			});

		}
	}
}


#endif