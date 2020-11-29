#ifndef SHADEDCOLORSHADER_HPP
#define SHADEDCOLORSHADER_HPP

#include "IShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Lighting/LightingSystem.hpp"

#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;
			using namespace Lighting;

			class ShadedColorShader : public IShader
			{
			private:
				std::shared_ptr<LightingSystem> lighting_system;

			public:
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const override;

				void SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system);
			};


		}
	}
}
#endif