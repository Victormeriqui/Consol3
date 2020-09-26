#include "ConsoleManager.hpp"

#include <Windows.h>
#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>
#include <wchar.h>
#include <strsafe.h>

namespace Display
{
	ConsoleManager::ConsoleManager(short width, short height, std::wstring font_name, short font_width, short font_height) : width(width), height(height)
	{
		consolescreenbuffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE, // access
			FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
			NULL, // security attributes
			CONSOLE_TEXTMODE_BUFFER, // mode TODO: check if CONSOLE_GRAPHICS_BUFFER works
			NULL); // used by CONSOLE_GRAPHICS_BUFFER

		SetConsoleScreenBufferInfo(consolescreenbuffer, width, height, font_width, font_height);

		writeregion = { 0, 0, width, height };

		SetConsoleActiveScreenBuffer(consolescreenbuffer);
		
		StoreOriginalFontInfo();
		SetFontInfo(font_name, font_width, font_height, 0, 0);

		DisableCursor();
	}

	ConsoleManager::~ConsoleManager()
	{
		// restore the console to it's original state

		SetFontInfo(original_font_name, original_front_size.X, original_front_size.Y, original_font_weight, original_font_family);
		EnableCursor();
	}

	void ConsoleManager::SetConsoleScreenBufferInfo(HANDLE consolescreenbuffer, short width, short height, short font_width, short font_height)
	{
		CONSOLE_SCREEN_BUFFER_INFOEX info;

		// struct
		info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		// resolution, how many chars there are
		info.dwSize = { width + 1, height + 1 }; // the +1 avoids problems when resizing
		// position of the cursor (irrelevant since we're going to hide it)
		info.dwCursorPosition = { 0, 0 };
		// attributes used by preceding writes (irrelevant since we're not going to give input to the console)
		info.wAttributes = 0;
		// window size
		info.srWindow = { 0, 0, width, height };
		// maximum window size in chars
		info.dwMaximumWindowSize = { 1920, 1080 };
		// attributes used by popups (irrelevant)
		info.wPopupAttributes = 0;
		// TODO: test this?
		info.bFullscreenSupported = false;

		int test =  SetConsoleScreenBufferInfoEx(consolescreenbuffer, &info);
	}

	void ConsoleManager::SetConsoleWindowTitle(const std::wstring title) const
	{
		SetConsoleTitle(title.c_str());
	}

	void ConsoleManager::SetPalette(const COLORREF palette[])
	{
		CONSOLE_SCREEN_BUFFER_INFOEX curinfo = {};

		// the struct size needs to be filled before the call to GetConsoleScreenBufferInfoEx, otherwise it will fail for some reason
		curinfo.cbSize = sizeof(curinfo);

		GetConsoleScreenBufferInfoEx(consolescreenbuffer, &curinfo);

		std::copy(palette, palette + 16, curinfo.ColorTable);

		SetConsoleScreenBufferInfoEx(consolescreenbuffer, &curinfo);
	}

	void ConsoleManager::StoreOriginalFontInfo()
	{
		CONSOLE_FONT_INFOEX font_info;

		// the struct size needs to be filled before the call to GetCurrentConsoleFontEx, otherwise it will fail for some reason
		font_info.cbSize = sizeof(font_info);

		GetCurrentConsoleFontEx(consolescreenbuffer, false, &font_info);
		
		original_front_size = font_info.dwFontSize;
		original_font_name = std::wstring(font_info.FaceName);
		original_font_weight = font_info.FontWeight;
		original_font_family = font_info.FontFamily;
	}

	void ConsoleManager::SetFontInfo(std::wstring name, short width, short height, short weight, short family)
	{
		CONSOLE_FONT_INFOEX font_info;
		COORD font_size = { (short)width, (short)height };

		font_info.cbSize = sizeof(font_info);

		font_info.dwFontSize = font_size;
		this->font_size = font_size;

		wcscpy_s(font_info.FaceName, name.c_str());
		font_name = name;

		font_info.FontWeight = weight;
		font_weight = weight;

		font_info.FontFamily = family;
		font_family = family;

		SetCurrentConsoleFontEx(consolescreenbuffer, false, &font_info);

		CONSOLE_FONT_INFOEX font_info2;

		// the struct size needs to be filled before the call to GetCurrentConsoleFontEx, otherwise it will fail for some reason
		font_info2.cbSize = sizeof(font_info2);

		GetCurrentConsoleFontEx(consolescreenbuffer, false, &font_info2);

	}

	void ConsoleManager::EnableCursor()
	{
		CONSOLE_CURSOR_INFO cursorinfo;

		GetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
		cursorinfo.bVisible = true;

		SetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
	}

	void ConsoleManager::DisableCursor()
	{
		CONSOLE_CURSOR_INFO cursorinfo;

		GetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
		cursorinfo.bVisible = false;

		SetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
	}

	void ConsoleManager::FillScreenBuffer(const CHAR_INFO* data)
	{
		WriteConsoleOutputA(
			consolescreenbuffer, // screen buffer to write to
			data, // data to write
			{ width, height }, // size of the data to write
			{ 0, 0 }, // start of the data to write
			&writeregion); // region to write to
	}
}