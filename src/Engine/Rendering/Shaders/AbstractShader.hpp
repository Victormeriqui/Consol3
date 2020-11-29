#ifndef ABSTRACTSHADER_HPP
#define ABSTRACTSHADER_HPP

#include "../Vertex.hpp"
#include "../../../Math/Matrix4.hpp"
#include "../../../Display/HSVColor.hpp"
#include "../Triangle.hpp"

#include <map>
#include <string>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			using namespace Math;
			using namespace Display;

			class AbstractShader
			{
			private:
				std::map<std::string, std::shared_ptr<void>> fragment_shader_attributes;
			
			protected:
				[[nodiscard]] std::shared_ptr<void> GetFragmentData(const std::string& key) const;
			
			public:
				virtual void FragmentShader(HSVColor& out_color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) const = 0;

				void SetFragmentData(const std::string& key, std::shared_ptr<void> attribute);
			};
		}
	}
}

#endif