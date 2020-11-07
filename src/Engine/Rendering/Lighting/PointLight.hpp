#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "ILight.hpp"
#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			struct Attenuation
			{
				float a;
				float b;
				float c;
			};

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
			};
		}
	}
}

#endif
