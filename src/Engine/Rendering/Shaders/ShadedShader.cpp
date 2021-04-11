#include "ShadedShader.hpp"

#include "../../../Math/Util/MathUtil.hpp"

#include <algorithm>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Lighting;
			using namespace Math;

			void ShadedShader::SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system)
			{
				this->lighting_system = std::move(lighting_system);
			}

			void ShadedShader::SetTexture(std::shared_ptr<Texture> texture)
			{
				this->texture = std::move(texture);
			}

			void ShadedShader::SetNormalMap(std::shared_ptr<Texture> normal_map)
			{
				this->normal_map = std::move(normal_map);
				has_normal_map	 = true;
			}

			void ShadedShader::DisableNormalMap()
			{
				has_normal_map = false;
			}

			bool ShadedShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
			{
				TransformVertexModel(v0, mvp_mats);
				TransformVertexModel(v1, mvp_mats);
				TransformVertexModel(v2, mvp_mats);

				vert_v0_model = v0;
				vert_v1_model = v1;
				vert_v2_model = v2;

				vert_lights_count = 0;

				for (std::shared_ptr<ILight> light : lighting_system->GetLights())
				{
					if (!light->IsShadowCaster())
						continue;

					const Matrix4& light_unused_mat		= Matrix4().SetIdentity();
					const Matrix4& light_view_mat		= light->GetViewMatrix().value().get();
					const Matrix4& light_projection_mat = light->GetProjectionMatrix().value().get();

					MVPTransform light_mvp_mats = { light_unused_mat, light_unused_mat, light_view_mat, light_projection_mat };

					vert_v0_light[vert_lights_count] = v0;
					vert_v1_light[vert_lights_count] = v1;
					vert_v2_light[vert_lights_count] = v2;

					TransformVertexViewProjection(vert_v0_light[vert_lights_count], light_mvp_mats);
					TransformVertexViewProjection(vert_v1_light[vert_lights_count], light_mvp_mats);
					TransformVertexViewProjection(vert_v2_light[vert_lights_count], light_mvp_mats);

					vert_v0_light[vert_lights_count].PerspectiveDivide();
					vert_v1_light[vert_lights_count].PerspectiveDivide();
					vert_v2_light[vert_lights_count].PerspectiveDivide();

					vert_light_depthbuffer[vert_lights_count]		 = &(light->GetLightDepthBuffer().value().get());
					vert_light_islinearprojection[vert_lights_count] = light->IsLinearProjection().value();

					vert_lights_count++;
				}

				TransformVertexViewProjection(v0, mvp_mats);
				TransformVertexViewProjection(v1, mvp_mats);
				TransformVertexViewProjection(v2, mvp_mats);

				return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
			}

			HSVColor ShadedShader::FragmentShader(const RGBColor& color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2)
			{
				Vector3 frag_position = PerspectiveCorrectInterpolate<Vector3>(vert_v0_model.GetPosition(),
																			   vert_v1_model.GetPosition(),
																			   vert_v2_model.GetPosition(),
																			   triangle,
																			   barcoord0,
																			   barcoord1,
																			   barcoord2);

				Vector2 frag_texture_coord = PerspectiveCorrectInterpolate<Vector2>(vert_v0_model.GetTextureCoords(),
																					vert_v1_model.GetTextureCoords(),
																					vert_v2_model.GetTextureCoords(),
																					triangle,
																					barcoord0,
																					barcoord1,
																					barcoord2);

				Vector3 frag_normal = PerspectiveCorrectInterpolate<Vector3>(vert_v0_model.GetNormal(),
																			 vert_v1_model.GetNormal(),
																			 vert_v2_model.GetNormal(),
																			 triangle,
																			 barcoord0,
																			 barcoord1,
																			 barcoord2);

				if (has_normal_map)
				{
					Vector3 frag_tangent = PerspectiveCorrectInterpolate<Vector3>(vert_v0_model.GetTangent(),
																				  vert_v1_model.GetTangent(),
																				  vert_v2_model.GetTangent(),
																				  triangle,
																				  barcoord0,
																				  barcoord1,
																				  barcoord2);

					Vector3 frag_bitangent = PerspectiveCorrectInterpolate<Vector3>(vert_v0_model.GetBitangent(),
																					vert_v1_model.GetBitangent(),
																					vert_v2_model.GetBitangent(),
																					triangle,
																					barcoord0,
																					barcoord1,
																					barcoord2);

					RGBColor frag_normal_color = normal_map->GetColorFromTextureCoords(frag_texture_coord.x, frag_texture_coord.y);

					// tangent bitangent normal matrix to translate normal to world space
					Matrix4 tbn_mat = Matrix4().SetTBNMatrix(frag_tangent, frag_bitangent, frag_normal);

					frag_normal = Vector3(Util::Lerp((float)frag_normal_color.r, 0.0f, 255.0f, -1.0f, 1.0f),
										  Util::Lerp((float)frag_normal_color.g, 0.0f, 255.0f, -1.0f, 1.0f),
										  Util::Lerp((float)frag_normal_color.b, 0.0f, 255.0f, -1.0f, 1.0f));

					frag_normal *= tbn_mat;

					frag_normal.Normalize();
				}

				for (int i = 0; i < vert_lights_count; i++)
				{
					const Vector3& v0_position_light = vert_v0_light[i].GetPosition();
					const Vector3& v1_position_light = vert_v1_light[i].GetPosition();
					const Vector3& v2_position_light = vert_v2_light[i].GetPosition();

					if (vert_light_islinearprojection[i])
						frag_position_lights[i] = (v0_position_light * barcoord0) + (v1_position_light * barcoord1) + (v2_position_light * barcoord2);
					else
						frag_position_lights[i] = PerspectiveCorrectInterpolate<Vector3>(v0_position_light,
																						 v1_position_light,
																						 v2_position_light,
																						 triangle,
																						 barcoord0,
																						 barcoord1,
																						 barcoord2);
				}

				float light_amount	 = lighting_system->GetLightAmountAt(frag_position, frag_normal, frag_position_lights);
				float final_lighting = std::min(lighting_system->GetAmbientLight() + light_amount, 1.0f);

				RGBColor texture_color = texture->GetColorFromTextureCoords(frag_texture_coord.x, frag_texture_coord.y);
				texture_color.BlendMultiply(color);
				HSVColor color_hsv = HSVColor(texture_color);

				return HSVColor(color_hsv.hue, color_hsv.saturation, color_hsv.value * final_lighting);
			}
		}
	}
}
