#ifndef BARYCENTRICRASTERIZER_HPP
#define BARYCENTRICRASTERIZER_HPP

#include "AbstractRasterizer.hpp"

#include "../../Display/FrameBuffer.hpp"
#include "../Math/Point2.hpp"

namespace Engine
{
	namespace Rendering
	{
		class BarycentricRasterizer : public AbstractRasterizer
		{
		private:
			int32_t GetEdgeMagnituteToPoint(const Point2& p1, const Point2& p2, const Point2& point);



		public:
			BarycentricRasterizer(FrameBuffer& framebuffer);

			void RasterizeTriangle(Vertex v0, Vertex v1, Vertex v2, Color color);
		};
	}
}

#endif
