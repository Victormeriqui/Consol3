#include "TextOnlyRenderer.hpp"

#include "../Math/Util/MathUtil.hpp"

namespace Display
{
	TextOnlyRenderer::TextOnlyRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Terminal", 4, 4, palette_textonly)),
		shades({ ' ', (char)250, ';', '%', (char)176, (char)240, (char)157, (char)177, (char)178, (char)219 }),
		// shades(" .:-=+*#%@"),
		shades_count((uint8_t)shades.length())
	{
		ClearFrameBuffer();
	}

	void TextOnlyRenderer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		uint8_t index = Math::Util::LerpCast<uint8_t>(color.value, 0, shades_count - 1);

		framebuffer->SetValue(x, y, { (WCHAR)shades[index], 0x0F });
	}

	void TextOnlyRenderer::DisplayFrame()
	{
		console_manager.FillScreenBuffer(framebuffer->GetFrameBufferData());
	}

	void TextOnlyRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void TextOnlyRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer({ ' ', 0x0F });
	}

	const uint16_t TextOnlyRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t TextOnlyRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}
