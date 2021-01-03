#include "ShadedColorShader.hpp"

#include <algorithm>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Lighting;

			void ShadedColorShader::SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system)
			{
				this->lighting_system = std::move(lighting_system);
			}

			bool ShadedColorShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats)
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

			void ShadedColorShader::FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2)
			{
				Vector3 frag_position = PerspectiveCorrectInterpolate<Vector3>(vert_v0_model.GetPosition(),
																			   vert_v1_model.GetPosition(),
																			   vert_v2_model.GetPosition(),
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

				float light_amount = lighting_system->GetLightAmountAt(frag_position, frag_normal, frag_position_lights);

				float final_lighting = std::min(lighting_system->GetAmbientLight() + light_amount, 1.0f);

				out_color = HSVColor(out_color.hue, out_color.saturation, final_lighting);
			}
		}
	}
}
