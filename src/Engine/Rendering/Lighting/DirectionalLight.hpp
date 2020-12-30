#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

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
			class DirectionalLight : public ILight
			{
			private:
				Vector3 direction;
				float intensity;

				DepthBuffer depthbuffer;
				
				Transform transform;
				Matrix4 projection_mat;
				Matrix4 light_mat;

				void UpdateLightMatrix();

			public:
				DirectionalLight();
				DirectionalLight(const Vector3& direction);
				DirectionalLight(const Vector3& direction, float intensity);

				[[nodiscard]] Vector3 GetDirection() const;
				void SetDirection(const Vector3& direction);

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
