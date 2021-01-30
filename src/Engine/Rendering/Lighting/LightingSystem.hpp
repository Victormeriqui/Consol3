#ifndef LIGHTINGSYSTEM_HPP
#define LIGHTINGSYSTEM_HPP

#include "ILight.hpp"

#include "../../../Math/Vector3.hpp"
#include "../DepthBuffer.hpp"
#include "../Rasterizer.hpp"
#include "../Transform.hpp"
#include "../Vertex.hpp"

#include <memory>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			class LightingSystem
			{
			private:
				float ambient_light;
				std::vector<std::shared_ptr<ILight>> lights;

			public:
				LightingSystem();

				void AddLight(std::shared_ptr<ILight> light);
				void RemoveLight(int index);

				void SetAmbientLight(float ambient_light);
				[[nodiscard]] float GetAmbientLight() const;

				const std::vector<std::shared_ptr<ILight>> GetLights() const;

				[[nodiscard]] float GetLightAmountAt(const Vertex& vertex, const Vector3 vertex_position_lights[]) const;
				[[nodiscard]] float GetLightAmountAt(const Vector3& position, const Vector3& normal, const Vector3 position_lights[]) const;

				void ClearDepthBuffers();
			};
		}
	}
}

#endif
