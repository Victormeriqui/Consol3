#ifndef PLAINTEXTURESHADER_HPP
#define PLAINTEXTURESHADER_HPP

#include "IShader.hpp"

#include "../../../Display/HSVColor.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector2.hpp"
#include "../Texture.hpp"
#include "../Vertex.hpp"

#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;

			class PlainTextureShader : public IShader
			{
			private:
				std::shared_ptr<Texture> texture;

				// set by the vertex shader for the fragment shader
				Vector2 vert_v0_texture_coord;
				Vector2 vert_v1_texture_coord;
				Vector2 vert_v2_texture_coord;

			public:
				virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats) override;
				virtual void FragmentShader(
					HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) override;

				void SetTexture(std::shared_ptr<Texture> texture);
			};
		}
	}
}
#endif
