#include "AbstractShader.hpp"

#include <map>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Shaders
		{
			std::shared_ptr<void> AbstractShader::GetFragmentData(const std::string& key) const
			{
				return fragment_shader_attributes.at(key);
			}

			void AbstractShader::SetFragmentData(const std::string& key, std::shared_ptr<void> attribute)
			{
				fragment_shader_attributes.insert_or_assign(key, attribute);
			}
		}
	}
}