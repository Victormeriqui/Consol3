#ifndef LIGHTINGSYSTEM_HPP
#define LIGHTINGSYSTEM_HPP

#include "ILight.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"
#include "../Rasterizer.hpp"
#include "../DepthBuffer.hpp"
#include "../Transform.hpp"
#include "LightRenderer.hpp"

#include <vector>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		class Model;

		namespace Lighting
		{
			class LightingSystem
			{
			private:
				std::vector<std::shared_ptr<ILight>> lights;

				// a non functioning renderer for the secondary rasterizer
				std::shared_ptr<LightRenderer> lights_renderer;
				// a secondary rasterizer that writes to the light's depthbuffer, this is prefered so the main rasterizer doesn't need to be shared around
				Rasterizer lights_rasterizer;
			public:
				LightingSystem();

				void AddLight(std::shared_ptr<ILight> light);
				void RemoveLight(int index);

				[[nodiscard]] float GetLightAmountAt(const Vertex& vertex) const;
				[[nodiscard]] float GetLightAmountAt(const Vector3& position, const Vector3& normal) const;

				void ClearDepthBuffers();
				void RenderToDepthBuffers(const Model& model, const Transform transform);
			};
		}
	}
}

#endif