#ifndef MODEL_HPP
#define MODEL_HPP

#include "Vertex.hpp"
#include "Transform.hpp"

#include <string>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class Model
		{
		private:
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

		public:
			Model(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
			Model(std::string filename);

			void CalculateNormals() const;

			[[nodiscard]] std::vector<Vertex> GetVertices() const;
			[[nodiscard]] std::vector<unsigned int> GetIndices() const;

			void DrawModel(const Transform& transform) const;
		};
	}
}

#endif