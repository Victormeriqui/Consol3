#include "DitheredFrameDrawer.hpp"

#include "../../Math/Util/MathUtil.hpp"
#include "../ColorMapping.hpp"

namespace Display
{
	namespace Windows
	{
		using namespace Math;

		DitheredFrameDrawer::DitheredFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
			framebuffer(std::move(framebuffer)),
			terminal_manager(WindowsTerminalManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4))
		{
			this->framebuffer->FillBuffer({ { ' ' }, 0x00 });
		}

		void DitheredFrameDrawer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
		{
			uint32_t real_color_hex = color.GetHexValues();

			framebuffer->SetValue(x, y, dithered_colors[dithered_color_mapping[real_color_hex]].console_color);
		}

		void DitheredFrameDrawer::DisplayFrame()
		{
			terminal_manager.WriteFrameBufferData(framebuffer->GetFrameBufferData());
		}

		void DitheredFrameDrawer::ReportInformation(const std::string& info)
		{
			terminal_manager.SetTitle(info);
		}

		void DitheredFrameDrawer::ClearFrameBuffer()
		{
			this->framebuffer->FillBuffer({ { ' ' }, 0x00 });
		}

		const uint16_t DitheredFrameDrawer::GetFrameBufferWidth() const
		{
			return framebuffer->GetWidth();
		}

		const uint16_t DitheredFrameDrawer::GetFrameBufferHeight() const
		{
			return framebuffer->GetHeight();
		}
	}
}
