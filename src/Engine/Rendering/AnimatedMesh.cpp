#include "AnimatedMesh.hpp"
#include "../../Math/Util/MathUtil.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		AnimatedMesh::AnimatedMesh() :
			AbstractMesh(),
			anim_current_name("null"),
			anim_current_progress(0.0f),
			anim_current_fps(0.0f),
			is_animating(false)
		{
		}

		AnimatedMesh::AnimatedMesh(const std::string& model_resource, const Vector3& position, const RGBColor& color) :
			AbstractMesh(model_resource, position, color),
			anim_current_name("null"),
			anim_current_progress(0.0f),
			anim_current_fps(0.0f),
			is_animating(false)
		{
		}

		AnimatedMesh::AnimatedMesh(const std::string& model_resource, const Vector3& position, const Angle& rotation, const RGBColor& color) :
			AbstractMesh(model_resource, position, rotation, color),
			anim_current_name("null"),
			anim_current_progress(0.0f),
			anim_current_fps(0.0f),
			is_animating(false)
		{
		}

		AnimatedMesh::AnimatedMesh(const std::string& model_resource,
								   const std::string& texture_resource,
								   const Vector3& position,
								   const RGBColor& color) :
			AbstractMesh(model_resource, texture_resource, position, color),
			anim_current_name("null"),
			anim_current_progress(0.0f),
			anim_current_fps(0.0f),
			is_animating(false)
		{
		}

		AnimatedMesh::AnimatedMesh(const std::string& model_resource,
								   const std::string& texture_resource,
								   const Vector3& position,
								   const Angle& rotation,
								   const RGBColor& color) :
			AbstractMesh(model_resource, texture_resource, position, rotation, color),
			anim_current_name("null"),
			anim_current_progress(0.0f),
			anim_current_fps(0.0f),
			is_animating(false)
		{
		}

		bool AnimatedMesh::IsAnimated() const
		{
			return true;
		}

		void AnimatedMesh::PlayAnimation(const std::string& animation, float fps)
		{
			is_animating = true;

			anim_current_progress = 0;
			anim_current_fps	  = fps;
			anim_current_name	  = animation;

			last_update = std::chrono::high_resolution_clock::now();
		}

		bool AnimatedMesh::IsAnimating() const
		{
			return is_animating;
		}

		const std::string& AnimatedMesh::GetCurrentAnimationName() const
		{
			return anim_current_name;
		}

		float AnimatedMesh::GetCurrentAnimationProgress() const
		{
			return anim_current_progress;
		}

		void AnimatedMesh::UpdateAnimation()
		{
			if (!is_animating)
				return;

			std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();

			uint64_t difference = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update).count();

			float frametime			= 1.0f / anim_current_fps;
			uint64_t frametime_mili = (uint64_t)(frametime * 1000.0f);

			float progress = (float)difference / frametime_mili;

			anim_current_progress += progress;

			if (anim_current_progress >= 1.0f)
			{
				anim_current_progress = 1.0f;
				is_animating		  = false;
				PlayAnimation("attack", 0.4f);
			}

			last_update = std::chrono::high_resolution_clock::now();
		}
	}
}
