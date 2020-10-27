#ifndef CONSOLEBUFFER_HPP
#define CONSOLEBUFFER_HPP

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <string>

namespace Display
{
	class ConsoleManager
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

		void SetConsoleScreenBufferInfo(HANDLE consolescreenbuffer, short width, short height, short font_width, short font_height, const COLORREF palette[]);

		void StoreOriginalFontInfo();
		void SetFontInfo(std::wstring name, short width, short height, short weight, short family);

		void DisableCursor();
		void EnableCursor();

	public:
		ConsoleManager(short width, short height, std::wstring font_name, short font_width, short font_height, const COLORREF palette[]);
		~ConsoleManager();

		void SetPalette(const COLORREF palette[]);
		void SetTitle(const std::string& title);

		void FillScreenBuffer(const CHAR_INFO* data);
		void WriteConsoleString(const std::shared_ptr<std::string> string, uint32_t size);
	};
}

#endif