#include "SpotLight.hpp"

#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Util/MathUtil.hpp"
#include "../../../Math/Vector3.hpp"
#include "../Transform.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			SpotLight::SpotLight(const Vector3& position, const Vector3& direction, float range, RGBColor color) :
				angle(90),
				attenuation({ 0.01f, 0.01f, 1.0f }),
				range(range),
				color(color),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				// TODO: calculate width, height and fov with the light's angle
				projection_mat(Matrix4().SetPerspectiveProjection(200, 200, 0.1f, 100.0f, 50))
			{
				SetPosition(position);
				SetDirection(direction);
			}

			void SpotLight::UpdateViewMatrix()
			{
				Vector3 translation = Vector3(-position.x, -position.y, -position.z);

				Vector3 right = Util::yaxis.GetCrossProduct(direction);
				right.Normalize();

				Vector3 up = direction.GetCrossProduct(right);
				up.Normalize();

				Matrix4 translation_mat = Matrix4().SetTranslation(translation);
				Matrix4 rotation_mat	= Matrix4().SetDirectionalRotation(right, up, direction);

				view_mat = rotation_mat * translation_mat;
			}

			Vector3 SpotLight::GetPosition() const
			{
				return position;
			}

			void SpotLight::SetPosition(const Vector3& position)
			{
				this->position = position;

				UpdateViewMatrix();
			}

			Vector3 SpotLight::GetDirection() const
			{
				return direction;
			}
			void SpotLight::SetDirection(const Vector3& direction)
			{
				this->direction = direction;

				UpdateViewMatrix();
			}

			float SpotLight::GetAngle() const
			{
				return angle;
			}

			void SpotLight::SetAngle(float angle)
			{
				this->angle = angle;
			}

			float SpotLight::GetRange() const
			{
				return range;
			}

			void SpotLight::SetRange(float range)
			{
				this->range = range;
			}

			RGBColor SpotLight::GetColor() const
			{
				return color;
			}

			void SpotLight::SetColor(RGBColor color)
			{
				this->color = color;
			}

			RGBColor SpotLight::GetColorAt(const Vector3& position,
										   const Vector3& normal,
										   const Vector3& cam_pos,
										   const MaterialProperties& material_properties) const
			{
				Vector3 light_dir = position - this->position;
				float light_dist  = light_dir.GetLength();

				if (light_dist > range)
					return RGBColor(0, 0, 0);

				light_dir.Normalize();

				float spotfactor = light_dir.GetDotProduct(direction);

				float ang = Math::Util::ToDegrees(std::acosf(spotfactor));

				if (ang > angle)
					return RGBColor(0, 0, 0);

				float attenuation_amount = attenuation.c + attenuation.b * light_dist + attenuation.a * light_dist * light_dist + 0.0001f;

				float amount = normal.GetDotProduct(-light_dir);

				amount /= attenuation_amount;
				amount += GetSpecularHighlightAt(position, normal, cam_pos, light_dir, material_properties);
				amount = std::clamp(amount, 0.0f, 1.0f);

				return color.GetBlendMultiplied(amount);
			}

			bool SpotLight::IsShadowCaster() const
			{
				return true;
			}

			std::optional<bool> SpotLight::IsLinearProjection() const
			{
				return std::optional(false);
			}

			std::optional<std::reference_wrapper<const Matrix4>> SpotLight::GetProjectionMatrix() const
			{
				return std::optional(std::reference_wrapper(projection_mat));
			}

			std::optional<std::reference_wrapper<const Matrix4>> SpotLight::GetViewMatrix() const
			{
				return std::optional(std::reference_wrapper(view_mat));
			}

			std::optional<std::reference_wrapper<DepthBuffer>> SpotLight::GetLightDepthBuffer()
			{
				return std::optional(std::reference_wrapper<DepthBuffer>(depthbuffer));
			}

			std::optional<float> SpotLight::GetBias() const
			{
				return 0.005f;
			}

			void SpotLight::ClearDepthBuffer()
			{
				depthbuffer.FillBuffer(std::numeric_limits<float>::max());
			}
		}
	}
}
