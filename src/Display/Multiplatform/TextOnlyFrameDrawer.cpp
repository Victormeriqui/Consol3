#include "TextOnlyFrameDrawer.hpp"

#include "Math/Util/MathUtil.hpp"

#ifdef SYS_WINDOWS
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#elif defined(SYS_LINUX)
#include "Display/Windows/WindowsStructsForLinux.hpp"
#endif

namespace Display
{
	namespace Multiplatform
	{
		template<>
		TextOnlyFrameDrawer<CHAR_INFO>::TextOnlyFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer,
															std::shared_ptr<ITerminalManager<CHAR_INFO>> terminal_manager) :
			framebuffer(std::move(framebuffer)),
			terminal_manager(std::move(terminal_manager)),
			shades({ ' ', (char)250, ';', '%', (char)176, (char)240, (char)157, (char)177, (char)178, (char)219 }),
			// shades(" .:-=+*#%@"),
			shades_count((uint8_t)shades.length())
		{
		}

		template<>
		TextOnlyFrameDrawer<char>::TextOnlyFrameDrawer(std::shared_ptr<FrameBuffer<char>> framebuffer,
													   std::shared_ptr<ITerminalManager<char>> terminal_manager) :
			framebuffer(std::move(framebuffer)),
			terminal_manager(std::move(terminal_manager)),
			shades({ ' ', (char)250, ';', '%', (char)176, (char)240, (char)157, (char)177, (char)178, (char)219 }),
			// shades(" .:-=+*#%@"),
			shades_count((uint8_t)shades.length())
		{
		}

		template<>
		void TextOnlyFrameDrawer<CHAR_INFO>::SetPixel(uint16_t x, uint16_t y, RGBColor color)
		{
			float luminance = color.GetColorNormal();

			uint8_t index = Math::Util::LerpCast<uint8_t>(luminance, 0, shades_count - 1);

			framebuffer->SetValue(x, y, { static_cast<WCHAR>(shades[index]), 0x0F });
		}

		template<>
		void TextOnlyFrameDrawer<char>::SetPixel(uint16_t x, uint16_t y, RGBColor color)
		{
			float luminance = color.GetColorNormal();

			uint8_t index = Math::Util::LerpCast<uint8_t>(luminance, 0, shades_count - 1);

			framebuffer->SetValue(x, y, shades[index]);
		}

		template<typename T>
		void TextOnlyFrameDrawer<T>::DisplayFrame()
		{
			terminal_manager->WriteFrameBufferData(framebuffer->GetFrameBufferData());
		}

		template<typename T>
		void TextOnlyFrameDrawer<T>::ReportInformation(const std::string& info)
		{
			terminal_manager->SetTitle(info);
		}

		template<>
		void TextOnlyFrameDrawer<CHAR_INFO>::ClearFrameBuffer()
		{
			this->framebuffer->FillBuffer({ { ' ' }, 0x0F });
		}

		template<>
		void TextOnlyFrameDrawer<char>::ClearFrameBuffer()
		{
			this->framebuffer->FillBuffer(' ');
		}

		template<typename T>
		const uint16_t TextOnlyFrameDrawer<T>::GetFrameBufferWidth() const
		{
			return framebuffer->GetWidth();
		}

		template<typename T>
		const uint16_t TextOnlyFrameDrawer<T>::GetFrameBufferHeight() const
		{
			return framebuffer->GetHeight();
		}
	}
}
