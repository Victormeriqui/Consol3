#include "AbstractMesh.hpp"

#include "Color.hpp"
#include "Vector3.hpp"
#include "Angle.hpp"

namespace Engine
{
	namespace Rendering
	{
		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position) :
			model(model),
			color(Color(255, 255, 255)),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Color& color) :
			model(model),
			color(color),
			position(position),
			rotation(Angle(0.0f, 0.0f, 0.0f)),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation) :
			model(model),
			color(Color(255, 255, 255)),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		AbstractMesh::AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation, const Color& color) :
			model(model),
			color(color),
			position(position),
			rotation(rotation),
			scale(Vector3(1.0f, 1.0f, 1.0f))
		{
		}

		const Model& AbstractMesh::GetModel() const
		{
			return model;
		}

		const Color& AbstractMesh::GetColor() const
		{
			return color;
		}

		const Vector3& AbstractMesh::GetPosition() const
		{
			return position;
		}

		const Angle& AbstractMesh::GetRotation() const
		{
			return rotation;
		}

		const Vector3& AbstractMesh::GetScale() const
		{
			return scale;
		}

		AbstractMesh& AbstractMesh::SetModel(const Model& model)
		{
			this->model = Model(model);
		}

		AbstractMesh& AbstractMesh::SetColor(const Color& color)
		{
			this->color = Color(color);
		}

		AbstractMesh& AbstractMesh::SetPosition(const Vector3& position)
		{
			this->position = Vector3(position);
		}

		AbstractMesh& AbstractMesh::SetRotation(const Angle& rotation)
		{
			this->rotation = Angle(rotation);
		}

		AbstractMesh& AbstractMesh::SetScale(const Vector3& scale)
		{
			this->scale = Vector3(scale);
		}

	}
}
