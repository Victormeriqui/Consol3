#include "PointLight.hpp"

#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Vector3.hpp"

#include <algorithm>
#include <functional>
#include <optional>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			using namespace Math;

			PointLight::PointLight(const Vector3& position, float range, float intensity, RGBColor color) :
				position(position),
				range(range),
				intensity(intensity),
				color(color),
				attenuation({ 1.2f, 1, 1 })
			{
			}

			Vector3 PointLight::GetPosition() const
			{
				return position;
			}

			void PointLight::SetPosition(const Vector3& position)
			{
				this->position = position;
			}

			float PointLight::GetRange() const
			{
				return range;
			}

			void PointLight::SetRange(float range)
			{
				this->range = range;
			}

			float PointLight::GetIntensity() const
			{
				return intensity;
			}

			void PointLight::SetIntensity(float intensity)
			{
				this->intensity = intensity;
			}

			RGBColor PointLight::GetColor() const
			{
				return color;
			}

			void PointLight::SetColor(RGBColor color)
			{
				this->color = color;
			}

			RGBColor PointLight::GetColorAt(const Vector3& position,
											const Vector3& normal,
											const Vector3& cam_pos,
											const MaterialProperties& material_properties) const
			{
				Vector3 light_dir = position - this->position;
				float light_dist  = light_dir.GetLength();

				if (light_dist > range)
					return RGBColor(0, 0, 0);

				light_dir.Normalize();

				float attenuation_amount = attenuation.c + attenuation.b * light_dist + attenuation.a * light_dist * light_dist + 0.0001f;

				float amount = normal.GetDotProduct(-light_dir);

				amount = (amount * intensity) / attenuation_amount;
				amount += GetSpecularHighlightAt(position, normal, cam_pos, light_dir, material_properties);
				amount = std::clamp(amount, 0.0f, 1.0f);

				return color.GetBlendMultiplied(amount);
			}

			bool PointLight::IsShadowCaster() const
			{
				return false;
			}

			std::optional<bool> PointLight::IsLinearProjection() const
			{
				return std::nullopt;
			}

			std::optional<std::reference_wrapper<const Matrix4>> PointLight::GetProjectionMatrix() const
			{
				return std::nullopt;
			}

			std::optional<std::reference_wrapper<const Matrix4>> PointLight::GetViewMatrix() const
			{
				return std::nullopt;
			}

			std::optional<std::reference_wrapper<DepthBuffer>> PointLight::GetLightDepthBuffer()
			{
				return std::nullopt;
			}

			std::optional<float> PointLight::GetBias() const
			{
				return std::nullopt;
			}

			void PointLight::ClearDepthBuffer()
			{
			}
		}
	}
}
