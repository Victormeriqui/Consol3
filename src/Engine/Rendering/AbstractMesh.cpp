#include "AbstractMesh.hpp"

#include "../../Display/Color.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Angle.hpp"

namespace Engine
{
	namespace Rendering
	{
		AbstractMesh::AbstractMesh() :
			model(Model()),
			color(Color(255, 255, 255)),
			transform(Transform()),
			position(Vector3(0.0f, 0.0f, 0.0f)),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position) :
			model(model),
			color(Color(255, 255, 255)),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Color& color) :
			model(model),
			color(color),
			transform(Transform()),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation) :
			model(model),
			color(Color(255, 255, 255)),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation, const Color& color) :
			model(model),
			color(color),
			transform(Transform()),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		Model AbstractMesh::GetModel() const
		{
			return model;
		}

		Color AbstractMesh::GetColor() const
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

		AbstractMesh& AbstractMesh::SetModel(const Model& model)
		{
			this->model = Model(model);

			return *this;
		}

		AbstractMesh& AbstractMesh::SetColor(const Color& color)
		{
			this->color = Color(color);

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
