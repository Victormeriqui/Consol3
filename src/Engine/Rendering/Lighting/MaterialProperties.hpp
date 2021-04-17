#ifndef MATERIALPROPERTIES_HPP
#define MATERIALPROPERTIES_HPP

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			struct MaterialProperties
			{
				float specular_factor;
				float specular_intensity;

				MaterialProperties(float specular_factor = 1.0f, float specular_intensity = 0.5f) :
					specular_factor(specular_factor),
					specular_intensity(specular_intensity)
				{
				}
			};
		}
	}
}

#endif
