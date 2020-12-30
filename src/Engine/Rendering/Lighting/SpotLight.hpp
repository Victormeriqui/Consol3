#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "ILight.hpp"
#include "../Vertex.hpp"
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

				Transform transform;
				Matrix4 projection_mat;
				Matrix4 light_mat;

				void UpdateLightMatrix();

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

				virtual float GetLightAmountAt(const Vertex& vertex) const override;

				virtual bool IsShadowCaster() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetLightMatrix() const override;
				virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() override;
			};
		}
	}
}

#endif
