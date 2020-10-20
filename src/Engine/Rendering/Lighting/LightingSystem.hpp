#ifndef LIGHTINGSYSTEM_HPP
#define LIGHTINGSYSTEM_HPP

#include "ILight.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"

#include <vector>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			class LightingSystem
			{
			private:
				std::vector<std::shared_ptr<ILight>> lights;

			public:
				LightingSystem();

				void AddLight(std::shared_ptr<ILight> light);
				void RemoveLight(int index);

				[[nodiscard]] float GetLightAmountAt(const Vertex& vertex) const;

			};
		}
	}
}

#endif