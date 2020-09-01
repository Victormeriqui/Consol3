#ifndef ABSTRACTRASTERIZER_HPP
#define ABSTRACTRASTERIZER_HPP

#include "Matrix4.hpp"
#include "Vertex.hpp"
#include "Color.hpp"
#include "Transform.hpp"

using namespace Engine::Math;
using namespace Display;

namespace Engine
{
	namespace Rendering 
	{
		class AbstractRasterizer
		{
		private:
			Matrix4 model_mat;
			Matrix4 view_mat;
			Matrix4 projection_mat;

			Vertex& TransformVertexMVP(Vertex& vertex)
			{
				vertex *= model_mat;
				vertex *= view_mat;
				vertex *= projection_mat;
			}

		protected:
			AbstractRasterizer() {}

		public:
			void SetModelMatrix(const Transform& model_transform)
			{
				model_mat = model_transform.GetTransformationMatrix();
			}

			void SetModelMatrix(const Matrix4& model_matrix)
			{
				model_mat = model_matrix;
			}

			void SetViewMatrix(const Transform& view_transform)
			{
				view_mat = view_transform.GetTransformationMatrix();
			}

			void SetViewMatrix(const Matrix4& view_matrix)
			{
				view_mat = view_matrix;
			}

			void SetProjectionMatrix(const Matrix4& projection_matrix)
			{
				projection_mat = projection_matrix;
			}

			virtual void RasterizeTriangle(Vertex v1, Vertex v2, Vertex v3, Color color) = 0;
			
		};
	}
}

#endif
