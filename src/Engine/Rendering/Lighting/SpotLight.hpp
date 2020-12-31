#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "ILight.hpp"
#include "../../../Math/Vector3.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../DepthBuffer.hpp"
#include "../Transform.hpp"

#include <optional>
#include <functional>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			class SpotLight : public ILight
			{
			private:
				Vector3 position;
				Vector3 direction;
				float angle;
				Attenuation attenuation;

				float range;
				float intensity;

				DepthBuffer depthbuffer;

				Matrix4 projection_mat;
				Matrix4 view_mat;

				void UpdateViewMatrix();

			public:
				SpotLight();
				SpotLight(const Vector3& position, const Vector3& direction);
				SpotLight(const Vector3& position, const Vector3& direction, float range);

				[[nodiscard]] Vector3 GetPosition() const;
				void SetPosition(const Vector3& position);

				[[nodiscard]] Vector3 GetDirection() const;
				void SetDirection(const Vector3& direction);

				[[nodiscard]] float GetRange() const;
				void SetRange(float range);

				[[nodiscard]] float GetAngle() const;
				void SetAngle(float angle);

				[[nodiscard]] float GetIntensity() const;
				void SetIntensity(float intensity);

				virtual float GetLightAmountAt(const Vector3& position, const Vector3& normal) const override;

				virtual bool IsShadowCaster() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetProjectionMatrix() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetViewMatrix() const override;
				virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() override;
				virtual void ClearDepthBuffer() override;
			};
		}
	}
}

#endif
