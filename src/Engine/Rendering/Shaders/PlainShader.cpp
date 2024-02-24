#include "PlainShader.hpp"

namespace
{
    using Engine::Vector2;
    struct PlainShaderContext
    {
        Vector2 vert_v0_texture_coord;
        Vector2 vert_v1_texture_coord;
        Vector2 vert_v2_texture_coord;
    };
}

namespace Engine
{
    namespace Rendering
    {
        namespace Shaders
        {
            void PlainShader::SetTexture(std::shared_ptr<Texture> texture)
            {
                this->texture = std::move(texture);
            }

            size_t PlainShader::GetFragmentContextSize() const
            {
                return sizeof(PlainShaderContext);
            }

            bool PlainShader::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats, void* context)
            {
                auto ctx = static_cast<PlainShaderContext*>(context);

                TransformVertexMVP(v0, mvp_mats);
                TransformVertexMVP(v1, mvp_mats);
                TransformVertexMVP(v2, mvp_mats);

                ctx->vert_v0_texture_coord = v0.GetTextureCoords();
                ctx->vert_v1_texture_coord = v1.GetTextureCoords();
                ctx->vert_v2_texture_coord = v2.GetTextureCoords();

                return !IsBackface(v0.GetPosition(), v1.GetPosition(), v2.GetPosition());
            }

            RGBColor PlainShader::FragmentShader(RGBColor color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2, const void* context)
            {
                auto ctx = static_cast<const PlainShaderContext*>(context);

                Vector2 frag_texture_coord = PerspectiveCorrectInterpolate<Vector2>(ctx->vert_v0_texture_coord, ctx->vert_v1_texture_coord, ctx->vert_v2_texture_coord, triangle, barcoord0, barcoord1, barcoord2);

                RGBColor final_color = texture->GetColorFromTextureCoords(frag_texture_coord.x, frag_texture_coord.y);
                final_color.BlendMultiply(color);

                return final_color;
            }
        }
    }
}
