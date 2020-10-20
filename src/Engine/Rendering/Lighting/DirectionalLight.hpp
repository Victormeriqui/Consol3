#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "ILight.hpp"
#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"

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

			public:
				DirectionalLight();
				DirectionalLight(const Vector3& direction);
				DirectionalLight(const Vector3& direction, float intensity);

				[[nodiscard]] Vector3 GetDirection() const;
				void SetDirection(const Vector3& direction);

				[[nodiscard]] float GetIntensity() const;
				void SetIntensity(float intensity);

				virtual float GetLightAmountAt(const Vertex& vertex) const override;
			};
		}
	}
}

#endif
