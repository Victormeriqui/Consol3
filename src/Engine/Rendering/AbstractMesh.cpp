#include "AbstractMesh.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		AbstractMesh::AbstractMesh() :
			has_texture(false),
			hsvcolor(HSVColor(0.0f, 0.0f, 1.0f)),
			rgbcolor(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(Vector3(0.0f, 0.0f, 0.0f)),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const std::string& model_resource, const Vector3& position, const RGBColor& color) :
			model_resource(model_resource),
			has_texture(false),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const std::string& model_resource, const Vector3& position, const Angle& rotation, const RGBColor& color) :
			model_resource(model_resource),
			has_texture(false),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const std::string& model_resource,
								   const std::string& texture_resource,
								   const Vector3& position,
								   const RGBColor& color) :
			model_resource(model_resource),
			texture_resource(texture_resource),
			has_texture(true),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const std::string& model_resource,
								   const std::string& texture_resource,
								   const Vector3& position,
								   const Angle& rotation,
								   const RGBColor& color) :
			model_resource(model_resource),
			texture_resource(texture_resource),
			has_texture(true),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
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

		RGBColor AbstractMesh::GetColor() const
		{
			return rgbcolor;
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

		const Transform& AbstractMesh::GetTransform() const
		{
			return transform;
		}

		bool AbstractMesh::IsTextured() const
		{
			return has_texture;
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

		AbstractMesh& AbstractMesh::SetColor(const RGBColor& color)
		{
			rgbcolor = RGBColor(color);
			hsvcolor = HSVColor(color);

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
			transform.SetRotation(rotation.GetInRadians());

			return *this;
		}

		AbstractMesh& AbstractMesh::SetScale(const Vector3& scale)
		{
			this->scale = Vector3(scale);
			transform.SetScale(scale);

			return *this;
		}
	}
}
