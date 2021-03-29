#ifndef ANIMATEDMESH_HPP
#define ANIMATEDMESH_HPP

#include "AbstractMesh.hpp"

#include <chrono>
#include <cstdint>
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
			float anim_current_fps;

			std::chrono::steady_clock::time_point last_update;

		public:
			AnimatedMesh();

			virtual bool IsAnimated() const override;

			void PlayAnimation(const std::string& animation, float fps);

			[[nodiscard]] bool IsAnimating() const;

			[[nodiscard]] const std::string& GetCurrentAnimationName() const;
			[[nodiscard]] float GetCurrentAnimationProgress() const;

			void UpdateAnimation();
		};
	}
}

#endif
