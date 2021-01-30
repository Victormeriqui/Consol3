#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP

#include "../Engine/Rendering/StaticModel.hpp"

#include <cstdint>
#include <map>

namespace Game
{
	using namespace Engine;
	using namespace Rendering;

	class ModelGenerator
	{
	public:
		ModelGenerator();

		StaticModel GeneratePlane(uint32_t tile_amount_x, uint32_t tile_amount_y);
		StaticModel GenerateSphere(uint8_t detail);
	};
}

#endif
