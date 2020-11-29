#ifndef PLAINTEXTURESHADER_HPP
#define PLAINTEXTURESHADER_HPP

#include "IShader.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Texture.hpp"

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

			public:
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const override;

				void SetTexture(std::shared_ptr<Texture> texture);
			};
		}
	}
}
#endif