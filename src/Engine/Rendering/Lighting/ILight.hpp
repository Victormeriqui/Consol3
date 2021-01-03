#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "../../../Math/Vector3.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../DepthBuffer.hpp"

#include <optional>
#include <functional>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			using namespace Math;

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
				[[nodiscard]] virtual float GetLightAmountAt(const Vector3& position, const Vector3& normal) const = 0;
				
				[[nodiscard]] virtual bool IsShadowCaster() const = 0;
				// i dislike the need for the referece_wrapper here, but C++ optionals don't allow references
				[[nodiscard]] virtual std::optional<std::reference_wrapper<const Matrix4>> GetProjectionMatrix() const = 0;
				[[nodiscard]] virtual std::optional<std::reference_wrapper<const Matrix4>> GetViewMatrix() const = 0;
				[[nodiscard]] virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() = 0;
				[[nodiscard]] virtual std::optional<bool> IsLinearProjection() const = 0;
				[[nodiscard]] virtual std::optional<float> GetBias() const = 0;
				virtual void ClearDepthBuffer() = 0;
			};
		}
	}
}

#endif