#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "../../../Display/RGBColor.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector3.hpp"
#include "../DepthBuffer.hpp"
#include "../Transform.hpp"
#include "ILight.hpp"

#include <functional>
#include <optional>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			using namespace Display;

			class SpotLight : public ILight
			{
			private:
				Vector3 position;
				Vector3 direction;
				float angle;
				Attenuation attenuation;

				float range;
				float intensity;
				RGBColor color;

				DepthBuffer depthbuffer;

				Matrix4 projection_mat;
				Matrix4 view_mat;

				void UpdateViewMatrix();

			public:
				SpotLight(const Vector3& position  = Vector3(),
						  const Vector3& direction = Vector3(),
						  float range			   = 1.0f,
						  float intensity		   = 1.0f,
						  RGBColor color		   = RGBColor());

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

				virtual RGBColor GetColorAt(const Vector3& position,
											const Vector3& normal,
											const Vector3& cam_pos,
											const MaterialProperties& material_properties) const override;

				virtual bool IsShadowCaster() const override;

				virtual RGBColor GetColor() const override;
				virtual void SetColor(RGBColor color) override;

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
