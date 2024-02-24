#include "DepthMapShader.hpp"

namespace Engine
{
    namespace Rendering
    {
        namespace Shaders
        {
            size_t DepthMapShader::GetFragmentContextSize() const
            {
                return 0;
            }

            bool DepthMapShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats, void* context)
            {
                TransformVertexMVP(v0, mvp_mats);
                TransformVertexMVP(v1, mvp_mats);
                TransformVertexMVP(v2, mvp_mats);

                return true;    // IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
            }

            RGBColor DepthMapShader::FragmentShader(RGBColor color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2, const void* context)
            {
                return RGBColor();
            }
        }
    }
}
