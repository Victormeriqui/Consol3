#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "../../../Display/RGBColor.hpp"
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
			using namespace Display;

			class PointLight : public ILight
			{
			private:
				Vector3 position;
				Attenuation attenuation;

				float range;
				RGBColor color;

			public:
				PointLight(const Vector3& position = Vector3(), float range = 1.0f, RGBColor color = RGBColor());

				[[nodiscard]] Vector3 GetPosition() const;
				void SetPosition(const Vector3& position);

				[[nodiscard]] float GetRange() const;
				void SetRange(float range);

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
