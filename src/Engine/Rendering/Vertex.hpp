#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "../../Math/Matrix4.hpp"
#include "../../Math/Vector2.hpp"
#include "../../Math/Vector3.hpp"

#include <vector>

namespace Engine
{
	using namespace Math;

	namespace Rendering
	{
		class Vertex
		{
		private:
			Vector3 position;
			// w for homogenous coordinates for matrix multiplication and perspective divide
			float w;

			Vector3 normal;
			Vector2 texture_coords;

		public:
			constexpr Vertex() : position(Vector3()), w(1), normal(Vector3()), texture_coords(Vector2())
			{
			}
			constexpr Vertex(const Vector3& position) : position(position), w(1), normal(Vector3(0, 0, 0)), texture_coords(Vector2())
			{
			}
			constexpr Vertex(const Vector3& position, const Vector3& normal) : position(position), w(1), normal(normal), texture_coords(Vector2()) {};
			constexpr Vertex(const Vector3& position, const Vector2& uv) : position(position), w(1), normal(Vector3(0, 0, 0)), texture_coords(uv)
			{
			}
			constexpr Vertex(const Vector3& position, const Vector3& normal, const Vector2& uv) :
				position(position),
				w(1),
				normal(normal),
				texture_coords(uv) {};

			[[nodiscard]] Vector3 GetPosition() const;
			Vertex& SetPosition(const Vector3& position);

			[[nodiscard]] Vector3 GetNormal() const;
			Vertex& SetNormal(const Vector3& normal);

			[[nodiscard]] float GetW() const;
			Vertex& SetW(float w);

			[[nodiscard]] Vector2 GetTextureCoords() const;
			Vertex& SetTextureCoords(const Vector2& texture_coords);

			[[nodiscard]] Vertex GetPerspectiveDivided() const;
			Vertex& PerspectiveDivide();

			[[nodiscard]] bool IsInsideViewFrustum() const;

			[[nodiscard]] Vertex GetLerped(const Vertex& other, float amount) const;
			Vertex& Lerp(const Vertex& other, float amount);

			Vertex& TransformNormals(const Matrix4& normal_mat);

			inline void static CalculateNormals(std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices)
			{
				for (uint32_t i = 0; i < out_indices.size() - 3; i += 3)
				{
					Vertex& v0 = out_vertices[out_indices[i]];
					Vertex& v1 = out_vertices[out_indices[i + 1]];
					Vertex& v2 = out_vertices[out_indices[i + 2]];

					Vector3 edge0 = v1.GetPosition() - v0.GetPosition();
					Vector3 edge1 = v2.GetPosition() - v0.GetPosition();

					Vector3 normal = edge0.GetCrossProduct(edge1);
					normal.Normalize();

					// each adjacent face to each vertex contributes to the vertex normal
					v0.SetNormal(v0.GetNormal() + normal);
					v1.SetNormal(v1.GetNormal() + normal);
					v2.SetNormal(v2.GetNormal() + normal);
				}

				for (Vertex& vert : out_vertices)
					vert.SetNormal(vert.GetNormal().GetNormalized());
			}

			constexpr Vertex& operator*=(const Matrix4& mat) noexcept
			{
				float x_new = mat.values[0][0] * position.x + mat.values[0][1] * position.y + mat.values[0][2] * position.z + mat.values[0][3] * w;
				float y_new = mat.values[1][0] * position.x + mat.values[1][1] * position.y + mat.values[1][2] * position.z + mat.values[1][3] * w;
				float z_new = mat.values[2][0] * position.x + mat.values[2][1] * position.y + mat.values[2][2] * position.z + mat.values[2][3] * w;
				float w_new = mat.values[3][0] * position.x + mat.values[3][1] * position.y + mat.values[3][2] * position.z + mat.values[3][3] * w;

				position.x = x_new;
				position.y = y_new;
				position.z = z_new;
				w		   = w_new;

				return *this;
			}

			[[nodiscard]] constexpr Vertex operator*(const Matrix4& mat) const noexcept
			{
				return Vertex(*this) *= mat;
			}
		};
	}
}

#endif
