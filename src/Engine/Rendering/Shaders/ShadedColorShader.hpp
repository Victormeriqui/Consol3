#ifndef SHADEDCOLORSHADER_HPP
#define SHADEDCOLORSHADER_HPP

#include "IShader.hpp"

#include "../../../Display/HSVColor.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../DepthBuffer.hpp"
#include "../Lighting/LightingSystem.hpp"
#include "../Vertex.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Display;
			using namespace Lighting;

			class ShadedColorShader : public IShader
			{
			private:
				std::shared_ptr<LightingSystem> lighting_system;

				// set by the vertex shader for the fragment shader
				Vertex vert_v0_model;
				Vertex vert_v1_model;
				Vertex vert_v2_model;

				int vert_lights_count = 0;
				// the same vertices but in light space, in multiple lights
				Vertex vert_v0_light[10];
				Vertex vert_v1_light[10];
				Vertex vert_v2_light[10];

				// a pointer here is prefered to avoid calling make_shared on every triangle
				DepthBuffer* vert_light_depthbuffer[10];

				// non linear projections need to account for perspective
				bool vert_light_islinearprojection[10];

				Vector3 frag_position_lights[10];

			public:
				virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats) override;
				virtual void FragmentShader(
					HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) override;

				void SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system);
			};

		}
	}
}
#endif
