#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Math/Matrix4.hpp"
#include "Clipper.hpp"
#include "DepthBuffer.hpp"
#include "Shaders/IShader.hpp"
#include "Transform.hpp"
#include "Triangle.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;
		using namespace Shaders;

		class Rasterizer
		{
		private:
			Matrix4 model_mat;
			Matrix4 normal_mat;
			Matrix4 view_mat;
			Matrix4 inverse_view_mat;
			Matrix4 projection_mat;
			Matrix4 inverse_projection_mat;
			Matrix4 viewport_mat;

			std::shared_ptr<IFrameDrawer> frame_drawer;

			Clipper clipper;

			inline Vertex GetTransformedVertexInverseViewProjection(const Vertex& vertex);
			inline Vertex& TransformVertexScreenspace(Vertex& vertex);

			void ClipAndRasterize(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const RGBColor& color, IShader& shader);
			void RasterizeTriangle(DepthBuffer& depthbuffer, const Triangle& triangle, const RGBColor& color, IShader& shader);

		public:
			Rasterizer(std::shared_ptr<IFrameDrawer> frame_drawer);

			void SetModelMatrix(const Transform& model_transform);
			void SetModelMatrix(const Matrix4& model_matrix);
			void SetViewMatrix(const Matrix4& view_matrix);
			void SetProjectionMatrix(const Matrix4& projection_matrix);
			void SetViewportMatrix(const Matrix4& viewport_matrix);

			void DrawVertexBuffer(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const RGBColor& color, IShader& shader);
			void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);
		};
	}
}

#endif
