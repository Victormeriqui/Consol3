#ifndef LIGHTRENDERER_HPP
#define LIGHTRENDERER_HPP

#include "../../../Display/IRenderer.hpp"
#include "../../../Display/HSVColor.hpp"

#include <string>
#include <cstdint>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			using namespace Display;

			// this class exists only to prevent the rasterizer from writing to the screen's framebuffer when rasterizing to a light's depthmap
			// thus none of these methods will have any action whatsoever
			class LightRenderer : public IRenderer
			{
			public:
				LightRenderer()
				{
				}

				virtual void SetPixel(uint16_t x, uint16_t y, const HSVColor& color) override
				{
				}

				virtual void DisplayFrame() override
				{
				}

				virtual void ClearFrameBuffer() override
				{
				}

				virtual void ReportInformation(const std::string& info) override
				{
				}

				[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override
				{
					return 200;
				}

				[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override
				{
					return 200;
				}
			};

		}
	}
}


#endif