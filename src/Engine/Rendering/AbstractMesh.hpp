#ifndef MESH_HPP
#define MESH_HPP

#include "../../Display/HSVColor.hpp"
#include "../../Display/RGBColor.hpp"
#include "../../Math/Angle.hpp"
#include "../../Math/Vector3.hpp"
#include "Transform.hpp"

#include <string>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;

		class AbstractMesh
		{
		protected:
			std::string model_resource;
			std::string texture_resource;
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
			AbstractMesh(const std::string& model_resource, const Vector3& position, const RGBColor& color = RGBColor());

			AbstractMesh(const std::string& model_resource, const Vector3& position, const Angle& rotation, const RGBColor& color = RGBColor());

			AbstractMesh(const std::string& model_resource,
						 const std::string& texture_resource,
						 const Vector3& position,
						 const RGBColor& color = RGBColor());

			AbstractMesh(const std::string& model_resource,
						 const std::string& texture_resource,
						 const Vector3& position,
						 const Angle& rotation = Angle(),
						 const RGBColor& color = RGBColor());

			[[nodiscard]] const std::string& GetModelResource() const;
			[[nodiscard]] const std::string& GetTextureResource() const;
			[[nodiscard]] RGBColor GetColor() const;
			[[nodiscard]] Vector3 GetPosition() const;
			[[nodiscard]] Angle GetRotation() const;
			[[nodiscard]] Vector3 GetScale() const;
			[[nodiscard]] const Transform& GetTransform() const;
			[[nodiscard]] bool IsTextured() const;

			[[nodiscard]] virtual bool IsAnimated() const = 0;

			AbstractMesh& SetModelResource(const std::string& model_resource);
			AbstractMesh& SetTextureResource(const std::string& texture_resource);
			AbstractMesh& SetColor(const RGBColor& color);
			AbstractMesh& SetPosition(const Vector3& position);
			AbstractMesh& SetRotation(const Angle& rotation);
			AbstractMesh& SetScale(const Vector3& scale);
		};
	}
}

#endif
