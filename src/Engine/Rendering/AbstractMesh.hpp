#ifndef MESH_HPP
#define MESH_HPP

#include "../../Display/HSVColor.hpp"
#include "../../Display/RGBColor.hpp"
#include "../../Display/RGBColorConstants.hpp"
#include "../../Math/Angle.hpp"
#include "../../Math/Vector3.hpp"
#include "Lighting/MaterialProperties.hpp"
#include "Transform.hpp"

#include <string>

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
			std::string model_resource;
			std::string texture_resource;
			std::string normal_map_resource;

			RGBColor color;

			Transform transform;
			// these fields are just for useful getters, the real values that are used in the transform are encoded in the matrices
			Vector3 position;
			Angle rotation;
			Vector3 scale;

			MaterialProperties material_properties;

		public:
			AbstractMesh();

			[[nodiscard]] const std::string& GetModelResource() const;
			[[nodiscard]] const std::string& GetTextureResource() const;
			[[nodiscard]] const std::string& GetNormalMapResource() const;
			[[nodiscard]] RGBColor GetColor() const;
			[[nodiscard]] Vector3 GetPosition() const;
			[[nodiscard]] Angle GetRotation() const;
			[[nodiscard]] Vector3 GetScale() const;
			[[nodiscard]] MaterialProperties GetMaterialProperties();
			[[nodiscard]] const Transform& GetTransform() const;
			[[nodiscard]] bool IsTextured() const;
			[[nodiscard]] bool IsNormalMapped() const;

			[[nodiscard]] virtual bool IsAnimated() const = 0;

			AbstractMesh& SetModelResource(const std::string& model_resource);
			AbstractMesh& SetTextureResource(const std::string& texture_resource);
			AbstractMesh& SetNormalMapResource(const std::string& normal_map_resource);
			AbstractMesh& SetColor(const RGBColor& color);
			AbstractMesh& SetPosition(const Vector3& position);
			AbstractMesh& SetRotation(const Angle& rotation);
			AbstractMesh& SetScale(const Vector3& scale);
			AbstractMesh& SetMaterialProperties(const MaterialProperties& material_properties);
		};
	}
}

#endif
