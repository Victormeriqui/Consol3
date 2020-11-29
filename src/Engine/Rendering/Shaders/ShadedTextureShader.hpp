#ifndef SHADEDTEXTURECOLORSHADER_HPP
#define SHADEDTEXTURECOLORSHADER_HPP

#include "IShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Lighting/LightingSystem.hpp"
#include "../Texture.hpp"

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;
			using namespace Lighting;

			class ShadedTextureShader : public IShader
			{
			private:
				std::shared_ptr<Texture> texture;
				std::shared_ptr<LightingSystem> lighting_system;
			public:

				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const override;

				void SetTexture(std::shared_ptr<Texture> texture);
				void SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system);
			};
		}
	}
}
#endif