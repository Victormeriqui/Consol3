#ifndef TEXTONLYFRAMEDRAWER_HPP
#define TEXTONLYFRAMEDRAWER_HPP

#include "../FrameBuffer.hpp"
#include "../IFrameDrawer.hpp"
#include "../ITerminalManager.hpp"
#include "../RGBColor.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace Display
{
	namespace Multiplatform
	{
		template<typename T>
		class TextOnlyFrameDrawer : public IFrameDrawer
		{
		private:
			std::shared_ptr<FrameBuffer<T>> framebuffer;
			std::shared_ptr<ITerminalManager<T>> terminal_manager;

			const std::string shades;
			const uint8_t shades_count;

		public:
			TextOnlyFrameDrawer(std::shared_ptr<FrameBuffer<T>> framebuffer, std::shared_ptr<ITerminalManager<T>> terminal_manager);

			virtual void SetPixel(uint16_t x, uint16_t y, RGBColor color) override;

			virtual void DisplayFrame() override;

			virtual void ClearFrameBuffer() override;

			virtual void ReportInformation(const std::string& info) override;

			[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
			[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
		};
	}

}

#endif
