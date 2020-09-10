#include "Model.hpp"

#include "Transform.hpp"
#include "Rasterizer.hpp"

namespace Engine
{
	namespace Rendering
	{

		Model::Model()
		{
		}

		Model::Model(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
		{
			this->vertices = vertices;
			this->indices = indices;
		}

		Model::Model(std::string filename)
		{
			//TODO: do this
		}

		void Model::CalculateNormals() const
		{
			//TODO: do this
		}

		std::vector<Vertex> Model::GetVertices() const
		{
			return vertices;
		}

		std::vector<unsigned int> Model::GetIndices() const
		{
			return indices;
		}

		void Model::DrawModel(const Transform& transform, Rasterizer& rasterizer) const
		{
			rasterizer.SetModelMatrix(transform);

			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				Vertex v0 = vertices[indices[i]];
				Vertex v1 = vertices[indices[i + 1]];
				Vertex v2 = vertices[indices[i + 2]];

				rasterizer.RasterizeTriangle(v0, v1, v2, 0xFFFFFF);
			}
		}

	}
}