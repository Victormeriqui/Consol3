#include "AnimatedMesh.hpp"

namespace Engine
{
	namespace Rendering
	{
		AnimatedMesh::AnimatedMesh() : AbstractMesh(), anim_current_name("null"), anim_current_progress(0.0f), is_animating(false)
		{
		}

		AnimatedMesh::AnimatedMesh(const std::string& model_resource, const Vector3& position, const RGBColor& color) :
			AbstractMesh(model_resource, position, color),
			anim_current_name("null"),
			anim_current_progress(0.0f),
			is_animating(false)
		{
		}

		AnimatedMesh::AnimatedMesh(const std::string& model_resource, const Vector3& position, const Angle& rotation, const RGBColor& color) :
			AbstractMesh(model_resource, position, rotation, color),
			anim_current_name("null"),
			anim_current_progress(0.0f),
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
			is_animating(false)
		{
		}

		bool AnimatedMesh::IsAnimated() const
		{
			return true;
		}

		void AnimatedMesh::PlayAnimation(const std::string& animation)
		{
			is_animating = true;

			anim_current_progress = 0;

			anim_current_name = animation;
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

		void AnimatedMesh::UpdateAnimation(float progress)
		{
			if (!is_animating)
				return;

			anim_current_progress += progress;

			if (anim_current_progress >= 1.0f)
			{
				anim_current_progress = 1.0f;
				is_animating		  = false;
				PlayAnimation("attack");
			}
		}
	}
}
