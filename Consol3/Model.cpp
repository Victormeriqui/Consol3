#include "Model.hpp"

#include <algorithm>

namespace Engine
{
	namespace Rendering
	{

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

		void Model::DrawModel(/*const Transform& transform*/) const
		{
			//TODO: do this, using the transform
		}

	}
}