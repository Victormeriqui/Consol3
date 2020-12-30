#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include "../../Display/IRenderer.hpp"
#include "../../Math/Matrix4.hpp"
#include "Vertex.hpp"
#include "../../Display/RGBColor.hpp"
#include "../../Display/HSVColor.hpp"
#include "Transform.hpp"
#include "Clipper.hpp"
#include "DepthBuffer.hpp"
#include "Shaders/IShader.hpp"
#include "VertexBuffer.hpp"
#include "Triangle.hpp"

#include <cstdint>
#include <memory>
#include <functional>

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

			std::shared_ptr<IRenderer> renderer;

			Clipper clipper;

			inline Vertex& TransformVertexViewProjection(Vertex& vertex);
			inline Vertex& TransformVertexModel(Vertex& vertex);
			inline Vertex GetTransformedVertexInverseViewProjection(const Vertex& vertex);
			inline Vertex& TransformVertexScreenspace(Vertex& vertex);

			[[nodiscard]] inline bool IsBackface(const Vector3& p0, const Vector3& p1, const Vector3& p2) const;
			
			void ClipAndRasterize(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const HSVColor& color, IShader& shader);
			void RasterizeTriangle(DepthBuffer& depthbuffer, const Triangle& triangle, const HSVColor& color, const IShader& shader);
			
		public:
			Rasterizer(std::shared_ptr<IRenderer> renderer);

			void SetModelMatrix(const Transform& model_transform);
			void SetModelMatrix(const Matrix4& model_matrix);
			void SetViewMatrix(const Matrix4& view_matrix);
			void SetProjectionMatrix(const Matrix4& projection_matrix);
			void SetViewportMatrix(const Matrix4& viewport_matrix);

			void DrawVertexBuffer(DepthBuffer& depthbuffer, const VertexBuffer& vertex_buffer, const HSVColor& color, IShader& shader);
		};
	}
}

#endif