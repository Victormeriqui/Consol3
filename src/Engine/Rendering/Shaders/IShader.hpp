#ifndef ISHADER_HPP
#define ISHADER_HPP

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Triangle.hpp"

#include <map>
#include <string>
#include <memory>
#include <cstdint>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;

			struct MVPTransform
			{
				const Matrix4& model_mat;
				const Matrix4& normal_mat;
				const Matrix4& view_mat;
				const Matrix4& projection_mat;
			};

			class IShader
			{
			protected:
				inline Vertex& TransformVertexMVP(Vertex& vertex, const MVPTransform& mvp_mats)
				{
					vertex *= mvp_mats.model_mat;
					vertex.TransformNormals(mvp_mats.normal_mat);
					vertex *= mvp_mats.view_mat;
					vertex *= mvp_mats.projection_mat;

					return vertex;
				}

				inline Vertex& TransformVertexModel(Vertex& vertex, const MVPTransform& mvp_mats)
				{
					vertex *= mvp_mats.model_mat;
					vertex.TransformNormals(mvp_mats.normal_mat);

					return vertex;
				}

				inline Vertex& TransformVertexViewProjection(Vertex& vertex, const MVPTransform& mvp_mats)
				{
					vertex *= mvp_mats.view_mat;
					vertex *= mvp_mats.projection_mat;

					return vertex;
				}

				[[nodiscard]] inline bool IsBackface(const Vector3& p0, const Vector3& p1, const Vector3& p2) const
				{
					Vector3 center = Vector3(
						(p0.x + p1.x + p2.x) / 3,
						(p0.y + p1.y + p2.y) / 3,
						(p0.z + p1.z + p2.z) / 3);

					Vector3 edge1 = p0 - p1;
					Vector3 edge2 = p1 - p2;

					Vector3 facenormal = edge1.GetCrossProduct(edge2);

					return center.GetDotProduct(facenormal) > 0;
				}

				template <class T>
				[[nodiscard]] inline T PerspectiveCorrectInterpolate(const T& value0, const T& value1, const T& value2, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const
				{
					// perspective correct
					T value0_pc = value0 * triangle.v0_oneoverw;
					T value1_pc = value1 * triangle.v1_oneoverw;
					T value2_pc = value2 * triangle.v2_oneoverw;
				
					T interpolated = (value0_pc * barcoord0) + (value1_pc * barcoord1) + (value2_pc * barcoord2);
					
					float w = (triangle.v0_oneoverw * barcoord0) + (triangle.v1_oneoverw * barcoord1) + (triangle.v2_oneoverw * barcoord2);
					interpolated /= w;

					return interpolated;
				}

			public:
				virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats) = 0;
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) = 0;
			};
		}
	}
}

#endif