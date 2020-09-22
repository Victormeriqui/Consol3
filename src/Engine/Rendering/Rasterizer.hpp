#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include "../../Display/FrameBuffer.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Matrix4.hpp"
#include "Vertex.hpp"
#include "../../Display/Color.hpp"
#include "Transform.hpp"
#include "../../Display/FrameBuffer.hpp"
#include "Clipper.hpp"

#include <cstdint>
#include <vector>

using namespace Display;

namespace Engine
{
	namespace Rendering
	{
		// a triangle edge for rasterization, initializes with the edge function value for the given point
		struct TriangleEdge
		{
			// the x component step of the edge function for each pixel to the right
			int32_t step_delta_x;
			// the y component step of the edge function for each pixel down
			int32_t step_delta_y;

			// the value of the edge function
			// will be initialized with the result for the starting pixel, remaining values can be stepped with the deltas
			int32_t edgefunction_res;

			// calculates the edge function for a specific point (starting point), and fills in the step deltas taken from the function's components
			TriangleEdge(const Point2& v_a, const Point2& v_b, const Point2& point);
		};


		class Rasterizer
		{
		private:
			Matrix4 model_mat;
			Matrix4 view_mat;
			Matrix4 projection_mat;
			Matrix4 viewport_mat;

			FrameBuffer& framebuffer;

			inline Vertex& TransformVertexMVP(Vertex& vertex);
			inline Vertex& TransformVertexScreenspace(Vertex& vertex);

			Clipper clipper;

			[[nodiscard]] bool IsBackface(const Vector3& p0, const Vector3& p1, const Vector3& p2) const;
			void RasterizeTriangle(Vertex v0, Vertex v1, Vertex v2, Color color);

		public:
			Rasterizer(FrameBuffer& framebuffer);

			void SetModelMatrix(const Transform& model_transform);
			void SetModelMatrix(const Matrix4& model_matrix);
			void SetViewMatrix(const Matrix4& view_matrix);
			void SetProjectionMatrix(const Matrix4& projection_matrix);
			void SetViewportMatrix(const Matrix4& viewport_matrix);

			void DrawTriangle(Vertex v0, Vertex v1, Vertex v2, Color color);
		};
	}
}

#endif