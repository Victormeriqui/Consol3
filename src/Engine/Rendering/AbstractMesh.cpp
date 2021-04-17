#include "AbstractMesh.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		AbstractMesh::AbstractMesh() :
			color(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(Vector3()),
			rotation(Angle()),
			scale(Vector3(1.0f, 1.0f, 1.0f)),
			material_properties(MaterialProperties())
		{
		}

		const std::string& AbstractMesh::GetModelResource() const
		{
			return model_resource;
		}

		const std::string& AbstractMesh::GetTextureResource() const
		{
			return texture_resource;
		}

		const std::string& AbstractMesh::GetNormalMapResource() const
		{
			return normal_map_resource;
		}

		RGBColor AbstractMesh::GetColor() const
		{
			return color;
		}

		Vector3 AbstractMesh::GetPosition() const
		{
			return position;
		}

		Angle AbstractMesh::GetRotation() const
		{
			return rotation;
		}

		Vector3 AbstractMesh::GetScale() const
		{
			return scale;
		}

		MaterialProperties AbstractMesh::GetMaterialProperties()
		{
			return material_properties;
		}

		const Transform& AbstractMesh::GetTransform() const
		{
			return transform;
		}

		bool AbstractMesh::IsTextured() const
		{
			return !texture_resource.empty();
		}

		bool AbstractMesh::IsNormalMapped() const
		{
			return !normal_map_resource.empty();
		}

		AbstractMesh& AbstractMesh::SetModelResource(const std::string& model_resource)
		{
			this->model_resource = model_resource;

			return *this;
		}

		AbstractMesh& AbstractMesh::SetTextureResource(const std::string& texture_resource)
		{
			this->texture_resource = texture_resource;

			return *this;
		}

		AbstractMesh& AbstractMesh::SetNormalMapResource(const std::string& normal_map_resource)
		{
			this->normal_map_resource = normal_map_resource;

			return *this;
		}

		AbstractMesh& AbstractMesh::SetColor(const RGBColor& color)
		{
			this->color = color;

			return *this;
		}

		AbstractMesh& AbstractMesh::SetPosition(const Vector3& position)
		{
			this->position = Vector3(position);
			transform.SetTranslation(position);

			return *this;
		}

		AbstractMesh& AbstractMesh::SetRotation(const Angle& rotation)
		{
			this->rotation = Angle(rotation);
			transform.SetRotation(rotation);

			return *this;
		}

		AbstractMesh& AbstractMesh::SetScale(const Vector3& scale)
		{
			this->scale = Vector3(scale);
			transform.SetScale(scale);

			return *this;
		}
		AbstractMesh& AbstractMesh::SetMaterialProperties(const MaterialProperties& material_properties)
		{
			this->material_properties = material_properties;

			return *this;
		}
	}
}
