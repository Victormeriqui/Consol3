#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "ILight.hpp"
#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"

#include <optional>
#include <functional>

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

				virtual float GetLightAmountAt(const Vertex& vertex) const override;

				virtual bool IsShadowCaster() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetLightMatrix() const override;
				virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() override;
			};
		}
	}
}

#endif
