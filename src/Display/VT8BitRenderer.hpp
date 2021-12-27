

#ifndef VT8BITRENDERER_HPP
#define VT8BITRENDERER_HPP

#include "ConsoleManager.hpp"
#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "IRenderer.hpp"
#include "RGBColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace Display
{
	static const COLORREF palette_ansi1[16] = {
		0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080, 0xC0C0C0,
		0x808080, 0xFF0000, 0x00FF00, 0xFFFF00, 0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF,
	};

	struct IndexedColor
	{
		uint8_t console_color_index;
		RGBColor real_color;
	};

	static const IndexedColor indexed_colors[] = {
		{ 0, RGBColor(0, 0, 0) },		  { 1, RGBColor(128, 0, 0) },		{ 2, RGBColor(0, 128, 0) },		  { 3, RGBColor(128, 128, 0) },
		{ 4, RGBColor(0, 0, 128) },		  { 5, RGBColor(128, 0, 128) },		{ 6, RGBColor(0, 128, 128) },	  { 7, RGBColor(192, 192, 192) },
		{ 8, RGBColor(128, 128, 128) },	  { 9, RGBColor(255, 0, 0) },		{ 10, RGBColor(0, 255, 0) },	  { 11, RGBColor(255, 255, 0) },
		{ 12, RGBColor(0, 0, 255) },	  { 13, RGBColor(255, 0, 255) },	{ 14, RGBColor(0, 255, 255) },	  { 15, RGBColor(255, 255, 255) },
		{ 16, RGBColor(0, 0, 0) },		  { 17, RGBColor(0, 0, 95) },		{ 18, RGBColor(0, 0, 135) },	  { 19, RGBColor(0, 0, 175) },
		{ 20, RGBColor(0, 0, 215) },	  { 21, RGBColor(0, 0, 255) },		{ 22, RGBColor(0, 95, 0) },		  { 23, RGBColor(0, 95, 95) },
		{ 24, RGBColor(0, 95, 135) },	  { 25, RGBColor(0, 95, 175) },		{ 26, RGBColor(0, 95, 215) },	  { 27, RGBColor(0, 95, 255) },
		{ 28, RGBColor(0, 135, 0) },	  { 29, RGBColor(0, 135, 95) },		{ 30, RGBColor(0, 135, 135) },	  { 31, RGBColor(0, 135, 175) },
		{ 32, RGBColor(0, 135, 215) },	  { 33, RGBColor(0, 135, 255) },	{ 34, RGBColor(0, 175, 0) },	  { 35, RGBColor(0, 175, 95) },
		{ 36, RGBColor(0, 175, 135) },	  { 37, RGBColor(0, 175, 175) },	{ 38, RGBColor(0, 175, 215) },	  { 39, RGBColor(0, 175, 255) },
		{ 40, RGBColor(0, 215, 0) },	  { 41, RGBColor(0, 215, 95) },		{ 42, RGBColor(0, 215, 135) },	  { 43, RGBColor(0, 215, 175) },
		{ 44, RGBColor(0, 215, 215) },	  { 45, RGBColor(0, 215, 255) },	{ 46, RGBColor(0, 255, 0) },	  { 47, RGBColor(0, 255, 95) },
		{ 48, RGBColor(0, 255, 135) },	  { 49, RGBColor(0, 255, 175) },	{ 50, RGBColor(0, 255, 215) },	  { 51, RGBColor(0, 255, 255) },
		{ 52, RGBColor(95, 0, 0) },		  { 53, RGBColor(95, 0, 95) },		{ 54, RGBColor(95, 0, 135) },	  { 55, RGBColor(95, 0, 175) },
		{ 56, RGBColor(95, 0, 215) },	  { 57, RGBColor(95, 0, 255) },		{ 58, RGBColor(95, 95, 0) },	  { 59, RGBColor(95, 95, 95) },
		{ 60, RGBColor(95, 95, 135) },	  { 61, RGBColor(95, 95, 175) },	{ 62, RGBColor(95, 95, 215) },	  { 63, RGBColor(95, 95, 255) },
		{ 64, RGBColor(95, 135, 0) },	  { 65, RGBColor(95, 135, 95) },	{ 66, RGBColor(95, 135, 135) },	  { 67, RGBColor(95, 135, 175) },
		{ 68, RGBColor(95, 135, 215) },	  { 69, RGBColor(95, 135, 255) },	{ 70, RGBColor(95, 175, 0) },	  { 71, RGBColor(95, 175, 95) },
		{ 72, RGBColor(95, 175, 135) },	  { 73, RGBColor(95, 175, 175) },	{ 74, RGBColor(95, 175, 215) },	  { 75, RGBColor(95, 175, 255) },
		{ 76, RGBColor(95, 215, 0) },	  { 77, RGBColor(95, 215, 95) },	{ 78, RGBColor(95, 215, 135) },	  { 79, RGBColor(95, 215, 175) },
		{ 80, RGBColor(95, 215, 215) },	  { 81, RGBColor(95, 215, 255) },	{ 82, RGBColor(95, 255, 0) },	  { 83, RGBColor(95, 255, 95) },
		{ 84, RGBColor(95, 255, 135) },	  { 85, RGBColor(95, 255, 175) },	{ 86, RGBColor(95, 255, 215) },	  { 87, RGBColor(95, 255, 255) },
		{ 88, RGBColor(135, 0, 0) },	  { 89, RGBColor(135, 0, 95) },		{ 90, RGBColor(135, 0, 135) },	  { 91, RGBColor(135, 0, 175) },
		{ 92, RGBColor(135, 0, 215) },	  { 93, RGBColor(135, 0, 255) },	{ 94, RGBColor(135, 95, 0) },	  { 95, RGBColor(135, 95, 95) },
		{ 96, RGBColor(135, 95, 135) },	  { 97, RGBColor(135, 95, 175) },	{ 98, RGBColor(135, 95, 215) },	  { 99, RGBColor(135, 95, 255) },
		{ 100, RGBColor(135, 135, 0) },	  { 101, RGBColor(135, 135, 95) },	{ 102, RGBColor(135, 135, 135) }, { 103, RGBColor(135, 135, 175) },
		{ 104, RGBColor(135, 135, 215) }, { 105, RGBColor(135, 135, 255) }, { 106, RGBColor(135, 175, 0) },	  { 107, RGBColor(135, 175, 95) },
		{ 108, RGBColor(135, 175, 135) }, { 109, RGBColor(135, 175, 175) }, { 110, RGBColor(135, 175, 215) }, { 111, RGBColor(135, 175, 255) },
		{ 112, RGBColor(135, 215, 0) },	  { 113, RGBColor(135, 215, 95) },	{ 114, RGBColor(135, 215, 135) }, { 115, RGBColor(135, 215, 175) },
		{ 116, RGBColor(135, 215, 215) }, { 117, RGBColor(135, 215, 255) }, { 118, RGBColor(135, 255, 0) },	  { 119, RGBColor(135, 255, 95) },
		{ 120, RGBColor(135, 255, 135) }, { 121, RGBColor(135, 255, 175) }, { 122, RGBColor(135, 255, 215) }, { 123, RGBColor(135, 255, 255) },
		{ 124, RGBColor(175, 0, 0) },	  { 125, RGBColor(175, 0, 95) },	{ 126, RGBColor(175, 0, 135) },	  { 127, RGBColor(175, 0, 175) },
		{ 128, RGBColor(175, 0, 215) },	  { 129, RGBColor(175, 0, 255) },	{ 130, RGBColor(175, 95, 0) },	  { 131, RGBColor(175, 95, 95) },
		{ 132, RGBColor(175, 95, 135) },  { 133, RGBColor(175, 95, 175) },	{ 134, RGBColor(175, 95, 215) },  { 135, RGBColor(175, 95, 255) },
		{ 136, RGBColor(175, 135, 0) },	  { 137, RGBColor(175, 135, 95) },	{ 138, RGBColor(175, 135, 135) }, { 139, RGBColor(175, 135, 175) },
		{ 140, RGBColor(175, 135, 215) }, { 141, RGBColor(175, 135, 255) }, { 142, RGBColor(175, 175, 0) },	  { 143, RGBColor(175, 175, 95) },
		{ 144, RGBColor(175, 175, 135) }, { 145, RGBColor(175, 175, 175) }, { 146, RGBColor(175, 175, 215) }, { 147, RGBColor(175, 175, 255) },
		{ 148, RGBColor(175, 215, 0) },	  { 149, RGBColor(175, 215, 95) },	{ 150, RGBColor(175, 215, 135) }, { 151, RGBColor(175, 215, 175) },
		{ 152, RGBColor(175, 215, 215) }, { 153, RGBColor(175, 215, 255) }, { 154, RGBColor(175, 255, 0) },	  { 155, RGBColor(175, 255, 95) },
		{ 156, RGBColor(175, 255, 135) }, { 157, RGBColor(175, 255, 175) }, { 158, RGBColor(175, 255, 215) }, { 159, RGBColor(175, 255, 255) },
		{ 160, RGBColor(215, 0, 0) },	  { 161, RGBColor(215, 0, 95) },	{ 162, RGBColor(215, 0, 135) },	  { 163, RGBColor(215, 0, 175) },
		{ 164, RGBColor(215, 0, 215) },	  { 165, RGBColor(215, 0, 255) },	{ 166, RGBColor(215, 95, 0) },	  { 167, RGBColor(215, 95, 95) },
		{ 168, RGBColor(215, 95, 135) },  { 169, RGBColor(215, 95, 175) },	{ 170, RGBColor(215, 95, 215) },  { 171, RGBColor(215, 95, 255) },
		{ 172, RGBColor(215, 135, 0) },	  { 173, RGBColor(215, 135, 95) },	{ 174, RGBColor(215, 135, 135) }, { 175, RGBColor(215, 135, 175) },
		{ 176, RGBColor(215, 135, 215) }, { 177, RGBColor(215, 135, 255) }, { 178, RGBColor(215, 175, 0) },	  { 179, RGBColor(215, 175, 95) },
		{ 180, RGBColor(215, 175, 135) }, { 181, RGBColor(215, 175, 175) }, { 182, RGBColor(215, 175, 215) }, { 183, RGBColor(215, 175, 255) },
		{ 184, RGBColor(215, 215, 0) },	  { 185, RGBColor(215, 215, 95) },	{ 186, RGBColor(215, 215, 135) }, { 187, RGBColor(215, 215, 175) },
		{ 188, RGBColor(215, 215, 215) }, { 189, RGBColor(215, 215, 255) }, { 190, RGBColor(215, 255, 0) },	  { 191, RGBColor(215, 255, 95) },
		{ 192, RGBColor(215, 255, 135) }, { 193, RGBColor(215, 255, 175) }, { 194, RGBColor(215, 255, 215) }, { 195, RGBColor(215, 255, 255) },
		{ 196, RGBColor(255, 0, 0) },	  { 197, RGBColor(255, 0, 95) },	{ 198, RGBColor(255, 0, 135) },	  { 199, RGBColor(255, 0, 175) },
		{ 200, RGBColor(255, 0, 215) },	  { 201, RGBColor(255, 0, 255) },	{ 202, RGBColor(255, 95, 0) },	  { 203, RGBColor(255, 95, 95) },
		{ 204, RGBColor(255, 95, 135) },  { 205, RGBColor(255, 95, 175) },	{ 206, RGBColor(255, 95, 215) },  { 207, RGBColor(255, 95, 255) },
		{ 208, RGBColor(255, 135, 0) },	  { 209, RGBColor(255, 135, 95) },	{ 210, RGBColor(255, 135, 135) }, { 211, RGBColor(255, 135, 175) },
		{ 212, RGBColor(255, 135, 215) }, { 213, RGBColor(255, 135, 255) }, { 214, RGBColor(255, 175, 0) },	  { 215, RGBColor(255, 175, 95) },
		{ 216, RGBColor(255, 175, 135) }, { 217, RGBColor(255, 175, 175) }, { 218, RGBColor(255, 175, 215) }, { 219, RGBColor(255, 175, 255) },
		{ 220, RGBColor(255, 215, 0) },	  { 221, RGBColor(255, 215, 95) },	{ 222, RGBColor(255, 215, 135) }, { 223, RGBColor(255, 215, 175) },
		{ 224, RGBColor(255, 215, 215) }, { 225, RGBColor(255, 215, 255) }, { 226, RGBColor(255, 255, 0) },	  { 227, RGBColor(255, 255, 95) },
		{ 228, RGBColor(255, 255, 135) }, { 229, RGBColor(255, 255, 175) }, { 230, RGBColor(255, 255, 215) }, { 231, RGBColor(255, 255, 255) },
		{ 232, RGBColor(8, 8, 8) },		  { 233, RGBColor(18, 18, 18) },	{ 234, RGBColor(28, 28, 28) },	  { 235, RGBColor(38, 38, 38) },
		{ 236, RGBColor(48, 48, 48) },	  { 237, RGBColor(58, 58, 58) },	{ 238, RGBColor(68, 68, 68) },	  { 239, RGBColor(78, 78, 78) },
		{ 240, RGBColor(88, 88, 88) },	  { 241, RGBColor(98, 98, 98) },	{ 242, RGBColor(108, 108, 108) }, { 243, RGBColor(118, 118, 118) },
		{ 244, RGBColor(128, 128, 128) }, { 245, RGBColor(138, 138, 138) }, { 246, RGBColor(148, 148, 148) }, { 247, RGBColor(158, 158, 158) },
		{ 248, RGBColor(168, 168, 168) }, { 249, RGBColor(178, 178, 178) }, { 250, RGBColor(188, 188, 188) }, { 251, RGBColor(198, 198, 198) },
		{ 252, RGBColor(208, 208, 208) }, { 253, RGBColor(218, 218, 218) }, { 254, RGBColor(228, 228, 228) }, { 255, RGBColor(238, 238, 238) },
	};
	static constexpr uint8_t indexed_colors_len = 255;

	class VT8BitRenderer : public IRenderer
	{
	private:
		std::shared_ptr<FrameBuffer<uint8_t>> framebuffer;
		ConsoleManager console_manager;

		std::map<uint32_t, uint8_t> color_lookup_table;

		std::string framebuffer_string;
		uint64_t framebuffer_string_len;

		const std::string esc_sequence_start = "\x1b[48;5;";
		const uint8_t esc_sequence_start_len = (uint8_t)esc_sequence_start.length();
		const std::string esc_sequence_end	 = "m";
		const uint8_t esc_sequence_len		 = (uint8_t)esc_sequence_end.length();

		void GenerateLookupTable();
		void TranslateFrameBuffer();

	public:
		VT8BitRenderer(std::shared_ptr<FrameBuffer<uint8_t>> framebuffer);

		virtual void SetPixel(uint16_t x, uint16_t y, RGBColor color) override;

		virtual void DisplayFrame() override;

		virtual void ClearFrameBuffer() override;

		virtual void ReportInformation(const std::string& info) override;

		[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
		[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
	};
}

#endif
