#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "ILight.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector3.hpp"
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
			class DirectionalLight : public ILight
			{
			private:
				Vector3 direction;
				float intensity;

				DepthBuffer depthbuffer;

				Matrix4 projection_mat;
				Matrix4 view_mat;

				void UpdateViewMatrix();

			public:
				DirectionalLight();
				DirectionalLight(const Vector3& direction);
				DirectionalLight(const Vector3& direction, float intensity);

				[[nodiscard]] Vector3 GetDirection() const;
				void SetDirection(const Vector3& direction);

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
