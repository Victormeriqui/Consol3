#ifndef ABSTRACTRASTERIZER_HPP
#define ABSTRACTRASTERIZER_HPP

#include "Matrix4.hpp"
#include "Vertex.hpp"
#include "Color.hpp"
#include "Transform.hpp"
#include "FrameBuffer.hpp"

#include <cstdint>

using namespace Engine::Math;
using namespace Display;

namespace Engine
{
	namespace Rendering 
	{
		class AbstractRasterizer
		{
		protected:

			Matrix4 model_mat;
			Matrix4 view_mat;
			Matrix4 projection_mat;
			Matrix4 viewport_mat;

			FrameBuffer& framebuffer;

			AbstractRasterizer(FrameBuffer& framebuffer) : framebuffer(framebuffer)
			{
				float width_h = framebuffer.GetWidth() / 2.0f;
				float height_h = framebuffer.GetHeight() / 2.0f;
				float mat[4][4];

				mat[0][0] = width_h;  mat[0][1] = 0;          mat[0][2] = 0;     mat[0][3] = width_h - 0.5f;
				mat[1][0] = 0;        mat[1][1] = -height_h;  mat[1][2] = 0;     mat[1][3] = height_h - 0.5f;
				mat[2][0] = 0;        mat[2][1] = 0;          mat[2][2] = 1;     mat[2][3] = 0;
				mat[3][0] = 0;        mat[3][1] = 0;          mat[3][2] = 0;     mat[3][3] = 1;

				viewport_mat = Matrix4(mat);
			}

			Vertex& TransformVertexMVP(Vertex& vertex)
			{
				vertex *= model_mat;
				vertex *= view_mat;
				vertex *= projection_mat;

				return vertex;
			}

			Vertex& TransformVertexNDC(Vertex& vertex)
			{
				
				vertex *= viewport_mat;
				vertex.PerspectiveDivide();
				return vertex;
			}

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

			void SetViewportMatrix(const Matrix4& viewport_matrix)
			{
				viewport_mat = viewport_matrix;
			}

			virtual void RasterizeTriangle(Vertex v0, Vertex v1, Vertex v2, Color color) = 0;
			
		};
	}
}

#endif
