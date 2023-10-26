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

                MaterialProperties(float specular_factor = 1.0f, float specular_intensity = 0.0f) : specular_factor(specular_factor), specular_intensity(specular_intensity)
                {
                }
            };
        }
    }
}

#endif
