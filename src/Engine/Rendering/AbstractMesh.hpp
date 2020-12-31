#ifndef MESH_HPP
#define MESH_HPP

#include "../../Display/RGBColor.hpp"
#include "../../Display/HSVColor.hpp"
#include "Model.hpp"
#include "../../Math/Vector3.hpp"
#include "../../Math/Angle.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Lighting/LightingSystem.hpp"

#include <memory>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;
		using namespace Lighting;

		class AbstractMesh
		{
		protected:
			Model model;
			Texture texture;
			bool has_texture;

			// rgb color is the one that is actually exposed, hsvcolor is internal for the frambuffer
			RGBColor rgbcolor;
			HSVColor hsvcolor;

			Transform transform;
			// these fields are just for useful getters, the real values that are used in the transform are encoded in the matrices
			Vector3 position;
			Angle rotation;
			Vector3 scale;

		public:
			
			AbstractMesh();
			AbstractMesh(const Model& model, const Vector3& position);
			AbstractMesh(const Model& model, const Vector3& position, const RGBColor& color);
			AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation);
			AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation, const RGBColor& color);

			AbstractMesh(const Model& model, const Texture& texture, const Vector3& position);
			AbstractMesh(const Model& model, const Texture& texture, const Vector3& position, const RGBColor& color);
			AbstractMesh(const Model& model, const Texture& texture, const Vector3& position, const Angle& rotation);
			AbstractMesh(const Model& model, const Texture& texture, const Vector3& position, const Angle& rotation, const RGBColor& color);

			[[nodiscard]] Model GetModel() const;
			[[nodiscard]] Texture GetTexture() const;
			[[nodiscard]] RGBColor GetColor() const;
			[[nodiscard]] Vector3 GetPosition() const;
			[[nodiscard]] Angle GetRotation() const;
			[[nodiscard]] Vector3 GetScale() const;

			AbstractMesh& SetModel(const Model& model);
			AbstractMesh& SetTexture(const Texture& texture);
			AbstractMesh& SetColor(const RGBColor& color);
			AbstractMesh& SetPosition(const Vector3& position);
			AbstractMesh& SetRotation(const Angle& rotation);
			AbstractMesh& SetScale(const Vector3& scale);

			virtual void DrawMesh(Camera& camera, std::shared_ptr<LightingSystem> lighting_system, Rasterizer& rasterizer) const = 0;
			virtual void DrawShadedMesh(Camera& camera, std::shared_ptr<LightingSystem> lighting_system, Rasterizer& rasterizer) const = 0;
		};
	}
}

#endif