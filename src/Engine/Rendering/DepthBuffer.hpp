#ifndef DEPTHBUFFER_HPP
#define DEPTHBUFFER_HPP

#include <cstdint>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class DepthBuffer
		{
		protected:
			const uint16_t width;
			const uint16_t height;

			std::vector<float> buffer;

		public:
			DepthBuffer(uint16_t width, uint16_t height);

			void SetDepth(uint16_t x, uint16_t y, float depth);
			[[nodiscard]] float GetDepth(uint16_t x, uint16_t y) const;

			[[nodiscard]] uint16_t GetWidth() const;
			[[nodiscard]] uint16_t GetHeight() const;

			[[nodiscard]] const float* GetDeptBufferData() const;

			void ClearBuffer();
			void FillBuffer(float value);
		};
	}
}

#endif