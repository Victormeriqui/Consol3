#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector3.hpp"
#include "ILight.hpp"

#include <functional>
#include <optional>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			class PointLight : public ILight
			{
			private:
				Vector3 position;
				Attenuation attenuation;

				float range;
				float intensity;

			public:
				PointLight();
				PointLight(const Vector3& position);
				PointLight(const Vector3& position, float range);

				[[nodiscard]] Vector3 GetPosition() const;
				void SetPosition(const Vector3& position);

				[[nodiscard]] float GetRange() const;
				void SetRange(float range);

				[[nodiscard]] float GetIntensity() const;
				void SetIntensity(float intensity);

				virtual float GetLightAmountAt(const Vector3& position, const Vector3& normal) const override;

				virtual bool IsShadowCaster() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetProjectionMatrix() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetViewMatrix() const override;
				virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() override;
				virtual std::optional<float> GetBias() const override;
				virtual std::optional<bool> IsLinearProjection() const override;
				virtual void ClearDepthBuffer() override;
			};
		}
	}
}

#endif
