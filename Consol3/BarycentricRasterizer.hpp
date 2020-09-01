#ifndef BARYCENTRICRASTERIZER_HPP
#define BARYCENTRICRASTERIZER_HPP

#include "AbstractRasterizer.hpp"

namespace Engine
{
	namespace Rendering
	{
		class BarycentricRasterizer : public AbstractRasterizer
		{
		public:
			BarycentricRasterizer();

			void RasterizeTriangle(Vertex v1, Vertex v2, Vertex v3, Color color);
		};
	}
}

#endif
