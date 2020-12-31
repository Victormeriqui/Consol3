#ifndef SHADEDTEXTURECOLORSHADER_HPP
#define SHADEDTEXTURECOLORSHADER_HPP

#include "IShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector2.hpp"
#include "../../../Math/Vector3.hpp"
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
				Vector3 vert_v0_position;
				Vector3 vert_v1_position;
				Vector3 vert_v2_position;
				Vector3 vert_v0_normal;
				Vector3 vert_v1_normal;
				Vector3 vert_v2_normal;

				Vector2 vert_v0_texture_coord;
				Vector2 vert_v1_texture_coord;
				Vector2 vert_v2_texture_coord;

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