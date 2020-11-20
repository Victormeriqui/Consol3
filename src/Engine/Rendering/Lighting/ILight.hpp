#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "../Vertex.hpp"

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			struct Attenuation
			{
				float a;
				float b;
				float c;
			};

			class ILight
			{
			protected:
				ILight() {}
			public:
				[[nodiscard]] virtual float GetLightAmountAt(const Vertex& vertex) const = 0;
			};
		}
	}
}

#endif