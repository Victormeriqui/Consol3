#ifndef SHADERSTORE_HPP
#define SHADERSTORE_HPP

#include "PlainColorShader.hpp"
#include "PlainTextureShader.hpp"
#include "ShadedColorShader.hpp"
#include "ShadedTextureShader.hpp"

// utility class to hold shader instances
namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			struct ShaderStore
			{
				PlainColorShader shader_plaincolor;
				PlainTextureShader shader_plaintexture;
				ShadedColorShader shader_shadedcolor;
				ShadedTextureShader shader_shadedtexture;

				ShaderStore()
				{
				
				}
			};
		}
	}
}

#endif
