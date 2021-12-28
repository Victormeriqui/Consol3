#include "DitheredRenderer.hpp"

#include "../Math/Util/MathUtil.hpp"
#include "ColorMapping.hpp"

namespace Display
{
	using namespace Math;

	DitheredRenderer::DitheredRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4))
	{
		ClearFrameBuffer();
	}

	void DitheredRenderer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
	{
		uint32_t real_color_hex = color.GetHexValues();

		framebuffer->SetValue(x, y, dithered_colors[dithered_color_mapping[real_color_hex]].console_color);
	}

	void DitheredRenderer::DisplayFrame()
	{
		console_manager.FillScreenBuffer(framebuffer->GetFrameBufferData());
	}

	void DitheredRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void DitheredRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer({ ' ', 0x00 });
	}

	const uint16_t DitheredRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t DitheredRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}
