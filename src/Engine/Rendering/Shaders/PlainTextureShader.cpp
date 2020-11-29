#include "PlainTextureShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Texture.hpp"

#include <string>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			void PlainTextureShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
			{
				std::shared_ptr<Texture> texture = std::static_pointer_cast<Texture>(GetFragmentData(std::string("texture")));

				Vector2 v0_texture_coord = triangle.v0.GetTextureCoords();
				Vector2 v1_texture_coord = triangle.v1.GetTextureCoords();
				Vector2 v2_texture_coord = triangle.v2.GetTextureCoords();

				v0_texture_coord *= triangle.v0_oneoverw;
				v1_texture_coord *= triangle.v1_oneoverw;
				v2_texture_coord *= triangle.v2_oneoverw;

				float u = v0_texture_coord.x * barcoord0 + v1_texture_coord.x * barcoord1 + v2_texture_coord.x * barcoord2;
				float v = v0_texture_coord.y * barcoord0 + v1_texture_coord.y * barcoord1 + v2_texture_coord.y * barcoord2;

				float oneoverperspective = 1.0f / ((barcoord0 * triangle.v0_oneoverw) + (barcoord1 * triangle.v1_oneoverw) + (barcoord2 * triangle.v2_oneoverw));
				
				u *= oneoverperspective;
				v *= oneoverperspective;

				RGBColor texel_rgb = texture->GetColorFromUV(u, v);
				
				out_color = HSVColor(texel_rgb);
			}
		}
	}
}