#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "Display/RGBColor.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Vector3.hpp"
#include "Engine/Rendering/DepthBuffer.hpp"
#include "Engine/Rendering/Transform.hpp"
#include "ILight.hpp"

#include <functional>
#include <optional>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			using namespace Display;

			class DirectionalLight : public ILight
			{
			private:
				Vector3 direction;
				RGBColor color;

				DepthBuffer depthbuffer;

				Matrix4 projection_mat;
				Matrix4 view_mat;

				void UpdateViewMatrix();

			public:
				DirectionalLight(const Vector3& direction = Vector3(), RGBColor color = RGBColor());

				[[nodiscard]] Vector3 GetDirection() const;
				void SetDirection(const Vector3& direction);

				virtual RGBColor GetColorAt(const Vector3& position,
											const Vector3& normal,
											const Vector3& cam_pos,
											const MaterialProperties& material_properties) const override;

				virtual bool IsShadowCaster() const override;

				virtual RGBColor GetColor() const override;
				virtual void SetColor(RGBColor color) override;

				virtual std::optional<std::reference_wrapper<const Matrix4>> GetProjectionMatrix() const override;
				virtual std::optional<std::reference_wrapper<const Matrix4>> GetViewMatrix() const override;
				virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer() override;
				virtual std::optional<float> GetBias() const override;
				virtual std::optional<bool> IsLinearProjection() const override;
				virtual void ClearDepthBuffer() override;
			};
		}
	}
}

#endif
