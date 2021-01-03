#ifndef MODELGENERATOR_HPP
#define MODELGENERATOR_HPP

#include "../Engine/Rendering/Model.hpp"

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

		Model GeneratePlane(uint32_t tile_amount_x, uint32_t tile_amount_y);
		Model GenerateSphere(uint8_t detail);
	};
}

#endif
