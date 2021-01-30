#ifndef ANIMATEDMESH_HPP
#define ANIMATEDMESH_HPP

#include "AbstractMesh.hpp"

#include <memory>
#include <string>

namespace Engine
{
	namespace Rendering
	{
		class AnimatedMesh : public AbstractMesh
		{
		private:
			bool is_animating;

			std::string anim_current_name;
			float anim_current_progress;

		public:
			AnimatedMesh();

			AnimatedMesh(const std::string& model_resource, const Vector3& position, const RGBColor& color = RGBColor());

			AnimatedMesh(const std::string& model_resource,
						 const Vector3& position,
						 const Angle& rotation = Angle(),
						 const RGBColor& color = RGBColor());

			AnimatedMesh(const std::string& model_resource,
						 const std::string& texture_resource,
						 const Vector3& position,
						 const RGBColor& color = RGBColor());

			AnimatedMesh(const std::string& model_resource,
						 const std::string& texture_resource,
						 const Vector3& position,
						 const Angle& rotation = Angle(),
						 const RGBColor& color = RGBColor());

			virtual bool IsAnimated() const override;

			void PlayAnimation(const std::string& animation);

			[[nodiscard]] bool IsAnimating() const;

			[[nodiscard]] const std::string& GetCurrentAnimationName() const;
			[[nodiscard]] float GetCurrentAnimationProgress() const;

			void UpdateAnimation(float progress);
		};
	}
}

#endif
