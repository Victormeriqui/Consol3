#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "../Vertex.hpp"

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
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