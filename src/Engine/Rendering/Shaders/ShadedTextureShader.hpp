#ifndef SHADEDTEXTURECOLORSHADER_HPP
#define SHADEDTEXTURECOLORSHADER_HPP

#include "IShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector2.hpp"
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

				// set by the vertex shader for the fragment shader
				float v0_light_amount;
				float v1_light_amount;
				float v2_light_amount;

				Vector2 v0_texture_coord;
				Vector2 v1_texture_coord;
				Vector2 v2_texture_coord;

			public:

				virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats) override;
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const override;

				void SetTexture(std::shared_ptr<Texture> texture);
				void SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system);
			};
		}
	}
}
#endif