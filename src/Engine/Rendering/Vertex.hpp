#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "../../Math/Matrix4.hpp"
#include "../../Math/Vector2.hpp"
#include "../../Math/Vector3.hpp"

#include <vector>
#include <xmmintrin.h>

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

			Vector3 tangent;
			Vector3 bitangent;

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

			[[nodiscard]] Vector3 GetTangent() const;
			Vertex& SetTangent(const Vector3& tangent);

			[[nodiscard]] Vector3 GetBitangent() const;
			Vertex& SetBitangent(const Vector3& bitangent);

			[[nodiscard]] float GetW() const;
			Vertex& SetW(float w);

			[[nodiscard]] Vector2 GetTextureCoords() const;
			Vertex& SetTextureCoords(const Vector2& texture_coords);

			[[nodiscard]] Vertex GetPerspectiveDivided() const;
			Vertex& PerspectiveDivide();

			[[nodiscard]] bool IsInsideViewFrustum() const;

			[[nodiscard]] Vertex GetLerped(const Vertex& other, float amount) const;
			Vertex& Lerp(const Vertex& other, float amount);

			Vertex& TransformNormal(const Matrix4& normal_mat);
			Vertex& TransformTangent(const Matrix4& normal_mat);
			Vertex& TransformBitangent(const Matrix4& normal_mat);

			inline void static CalculateNormals(std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices)
			{
				for (uint32_t i = 0; i < out_indices.size() - 3; i += 3)
				{
					Vertex& v0 = out_vertices[out_indices[i]];
					Vertex& v1 = out_vertices[out_indices[i + 1]];
					Vertex& v2 = out_vertices[out_indices[i + 2]];

					Vector3 edge0 = v1.GetPosition() - v0.GetPosition();
					Vector3 edge1 = v2.GetPosition() - v0.GetPosition();

					Vector3 normal = edge0.GetCrossProduct(edge1).GetNormalized();

					// each adjacent face to each vertex contributes to the vertex normal
					v0.SetNormal(v0.GetNormal() + normal);
					v1.SetNormal(v1.GetNormal() + normal);
					v2.SetNormal(v2.GetNormal() + normal);
				}

				for (Vertex& vert : out_vertices)
					vert.SetNormal(vert.GetNormal().GetNormalized());
			}

			inline void static CalculateTangents(std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices)
			{
				for (uint32_t i = 0; i <= out_indices.size() - 3; i += 3)
				{
					Vertex& v0 = out_vertices[out_indices[i]];
					Vertex& v1 = out_vertices[out_indices[i + 1]];
					Vertex& v2 = out_vertices[out_indices[i + 2]];

					Vector3 edge0 = v1.GetPosition() - v0.GetPosition();
					Vector3 edge1 = v2.GetPosition() - v0.GetPosition();

					Vector2 texture_edge0 = v1.GetTextureCoords() - v0.GetTextureCoords();
					Vector2 texture_edge1 = v2.GetTextureCoords() - v0.GetTextureCoords();

					float f = 1.0f / (texture_edge0.x * texture_edge1.y - texture_edge1.x * texture_edge0.y);

					// the bitangent could just be the cross product of the tangent and the normal, but since this is done only once on load, we don't
					// really care if that would be a little faster
					Vector3 tangent	  = (Vector3(edge0 * texture_edge1.y - edge1 * texture_edge0.y) * f).GetNormalized();
					Vector3 bitangent = (Vector3(edge1 * texture_edge0.x - edge0 * texture_edge1.x) * f).GetNormalized();

					// each adjacent face to each vertex contributes to the vertex tangent
					v0.SetTangent(v0.GetTangent() + tangent);
					v1.SetTangent(v1.GetTangent() + tangent);
					v2.SetTangent(v2.GetTangent() + tangent);

					v0.SetBitangent(v0.GetBitangent() + bitangent);
					v1.SetBitangent(v1.GetBitangent() + bitangent);
					v2.SetBitangent(v2.GetBitangent() + bitangent);
				}

				for (Vertex& vert : out_vertices)
				{
					vert.SetTangent(vert.GetTangent().GetNormalized());
					vert.SetBitangent(vert.GetBitangent().GetNormalized());
				}
			}

			Vertex& operator*=(const Matrix4& mat) noexcept
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
				/*
				__m128 acc = _mm_setzero_ps();
				__m128 vec = _mm_set_ps(position.x, position.y, position.z, w);

				__m128 mat0 = _mm_set_ps(mat.values[0][0], mat.values[0][1], mat.values[0][2], mat.values[0][3]);
				__m128 mat1 = _mm_set_ps(mat.values[1][0], mat.values[1][1], mat.values[1][2], mat.values[1][3]);
				__m128 mat2 = _mm_set_ps(mat.values[2][0], mat.values[2][1], mat.values[2][2], mat.values[2][3]);
				__m128 mat3 = _mm_set_ps(mat.values[3][0], mat.values[3][1], mat.values[3][2], mat.values[3][3]);

				__m128 x_new = _mm_mul_ps(mat0, vec);
				__m128 y_new = _mm_mul_ps(mat1, vec);
				__m128 z_new = _mm_mul_ps(mat2, vec);
				__m128 w_new = _mm_mul_ps(mat3, vec);

				__m128 x_shuf = _mm_movehdup_ps(x_new);
				__m128 x_sum  = _mm_add_ps(x_new, x_shuf);
				x_shuf		  = _mm_movehl_ps(x_shuf, x_sum);
				x_sum		  = _mm_add_ss(x_sum, x_shuf);

				__m128 y_shuf = _mm_movehdup_ps(y_new);
				__m128 y_sum  = _mm_add_ps(y_new, y_shuf);
				y_shuf		  = _mm_movehl_ps(y_shuf, y_sum);
				y_sum		  = _mm_add_ss(y_sum, y_shuf);

				__m128 z_shuf = _mm_movehdup_ps(z_new);
				__m128 z_sum  = _mm_add_ps(z_new, z_shuf);
				z_shuf		  = _mm_movehl_ps(z_shuf, z_sum);
				z_sum		  = _mm_add_ss(z_sum, z_shuf);

				__m128 w_shuf = _mm_movehdup_ps(w_new);
				__m128 w_sum  = _mm_add_ps(w_new, w_shuf);
				w_shuf		  = _mm_movehl_ps(w_shuf, w_sum);
				w_sum		  = _mm_add_ss(w_sum, w_shuf);

				position.x = _mm_cvtss_f32(x_sum);
				position.y = _mm_cvtss_f32(y_sum);
				position.z = _mm_cvtss_f32(z_sum);
				w		   = _mm_cvtss_f32(w_sum);

				return *this;*/
			}

			[[nodiscard]] Vertex operator*(const Matrix4& mat) const noexcept
			{
				return Vertex(*this) *= mat;
			}
		};
	}
}

#endif
