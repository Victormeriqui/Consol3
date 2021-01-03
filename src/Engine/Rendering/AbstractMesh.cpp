#include "AbstractMesh.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		AbstractMesh::AbstractMesh() :
			model(Model()),
			texture(Texture()),
			has_texture(false),
			hsvcolor(HSVColor(0.0f, 0.0f, 1.0f)),
			rgbcolor(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(Vector3(0.0f, 0.0f, 0.0f)),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position) :
			model(model),
			texture(Texture()),
			has_texture(false),
			hsvcolor(HSVColor(0.0f, 0.0f, 1.0f)),
			rgbcolor(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const RGBColor& color) :
			model(model),
			texture(Texture()),
			has_texture(false),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation) :
			model(model),
			texture(Texture()),
			has_texture(false),
			hsvcolor(HSVColor(0.0f, 0.0f, 1.0f)),
			rgbcolor(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation, const RGBColor& color) :
			model(model),
			has_texture(false),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}


		AbstractMesh::AbstractMesh(const Model& model, const Texture& texture, const Vector3& position) :
			model(model),
			texture(texture),
			has_texture(true),
			hsvcolor(HSVColor(0.0f, 0.0f, 1.0f)),
			rgbcolor(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Texture& texture, const Vector3& position, const RGBColor& color) :
			model(model),
			texture(texture),
			has_texture(true),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Texture& texture, const Vector3& position, const Angle& rotation) :
			model(model),
			texture(texture),
			has_texture(true),
			hsvcolor(HSVColor(0.0f, 0.0f, 1.0f)),
			rgbcolor(RGBColor(255, 255, 255)),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Texture& texture, const Vector3& position, const Angle& rotation, const RGBColor& color) :
			model(model),
			texture(texture),
			has_texture(true),
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		const Model& AbstractMesh::GetModel() const
		{
			return model;
		}

		const Texture& AbstractMesh::GetTexture() const
		{
			return texture;
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

		AbstractMesh& AbstractMesh::SetModel(const Model& model)
		{
			this->model = Model(model);

			return *this;
		}

		AbstractMesh& AbstractMesh::SetTexture(const Texture& texture)
		{
			this->texture = Texture(texture);

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
			transform.SetRotation(rotation);

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