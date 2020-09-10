#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include "../../Display/FrameBuffer.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Matrix4.hpp"
#include "Vertex.hpp"
#include "../../Display/Color.hpp"
#include "Transform.hpp"
#include "../../Display/FrameBuffer.hpp"

using namespace Display;

namespace Engine
{
	namespace Rendering
	{
		class Rasterizer
		{
		private:
			Matrix4 model_mat;
			Matrix4 view_mat;
			Matrix4 projection_mat;
			Matrix4 viewport_mat;

			FrameBuffer& framebuffer;

			int32_t GetEdgeMagnituteToPoint(const Point2& p1, const Point2& p2, const Point2& point);

			Vertex& TransformVertexMVP(Vertex& vertex);
			Vertex& TransformVertexNDC(Vertex& vertex);
	
		public:
			Rasterizer(FrameBuffer& framebuffer);

			void SetModelMatrix(const Transform& model_transform);
			void SetModelMatrix(const Matrix4& model_matrix);
			void SetViewMatrix(const Matrix4& view_matrix);
			void SetProjectionMatrix(const Matrix4& projection_matrix);
			void SetViewportMatrix(const Matrix4& viewport_matrix);

			void RasterizeTriangle(Vertex v0, Vertex v1, Vertex v2, Color color);
		};
	}
}

#endif
