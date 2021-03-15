#ifndef RGBCONSTANTS_HPP
#define RGBCONSTANTS_HPP

#include "RGBColor.hpp"

namespace Display
{
	class RGBConstants
	{
	public:
		static RGBColor White()
		{
			return RGBColor(255, 255, 255);
		}
		static RGBColor Black()
		{
			return RGBColor(0, 0, 0);
		}
		static RGBColor Red()
		{
			return RGBColor(255, 0, 0);
		}
		static RGBColor Green()
		{
			return RGBColor(0, 255, 0);
		}
		static RGBColor Blue()
		{
			return RGBColor(0, 0, 255);
		}
	};

}

#endif
