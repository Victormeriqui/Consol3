#ifndef DEPTHMAPSHADER_HPP
#define DEPTHMAPSHADER_HPP

#include "IShader.hpp"

#include "Display/RGBColor.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "Math/Matrix4.hpp"

namespace Engine
{
    namespace Rendering
    {
        namespace Shaders
        {
            using namespace Display;

            class DepthMapShader : public IShader
            {
            private:
            public:
                virtual size_t GetFragmentContextSize() const override;
                virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats, void* context) override;
                virtual RGBColor FragmentShader(RGBColor color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2, const void* context) override;
            };

        }
    }
}
#endif
