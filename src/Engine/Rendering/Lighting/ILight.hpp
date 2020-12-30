#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "../Vertex.hpp"
#include "../DepthBuffer.hpp"

#include <optional>
#include <functional>

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
				
				[[nodiscard]] virtual bool IsShadowCaster() const = 0;
				// i dislike the need for the referece_wrapper here, but C++ optionals don't allow references
				[[nodiscard]] virtual std::optional<std::reference_wrapper<const Matrix4>> GetLightMatrix() const = 0;
				[[nodiscard]] virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() = 0;
			};
		}
	}
}

#endif