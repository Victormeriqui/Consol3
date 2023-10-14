#ifndef WINDOWSTERMINALMANAGER_HPP
#define WINDOWSTERMINALMANAGER_HPP

#include "Display/ITerminalManager.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <string>

namespace Display
{
	namespace Windows
	{
		static const uint32_t palette_default[16] = {
			0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080, 0xC0C0C0,
			0x808080, 0xFF0000, 0x00FF00, 0xFFFF00, 0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF,
		};

		class WindowsTerminalManager : public ITerminalManager<CHAR_INFO>
		{
		private:
			short width;
			short height;

			HANDLE consolescreenbuffer;

			// this is the write region used by the WriteConsoleOutput function to write to the backbuffer, cached
			SMALL_RECT writeregion;

			COORD original_front_size;
			std::wstring original_font_name;
			short original_font_weight;
			short original_font_family;

			COORD font_size;
			std::wstring font_name;
			short font_weight;
			short font_family;

			void SetConsoleScreenBufferInfo(
				HANDLE consolescreenbuffer, short width, short height, short font_width, short font_height, const uint32_t palette[]);

			void StoreOriginalFontInfo();
			void SetFontInfo(std::wstring name, short width, short height, short weight, short family);

		public:
			WindowsTerminalManager(
				short width, short height, std::wstring font_name, short font_width, short font_height, const uint32_t palette[] = palette_default);
			~WindowsTerminalManager();

			virtual void SetPalette(const uint32_t palette[]) override;
			virtual void SetTitle(const std::string& title) override;

			virtual void DisableCursor() override;
			virtual void EnableCursor() override;

			virtual void WriteFrameBufferData(const CHAR_INFO* data) override;
			virtual void WriteSizedString(const std::string& string, uint64_t size) override;
		};
	}
}
#endif