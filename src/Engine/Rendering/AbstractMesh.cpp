#include "AbstractMesh.hpp"

#include "../../Display/RGBColor.hpp"
#include "../../Math/Vector3.hpp"
#include "../../Math/Angle.hpp"

using namespace Math;

namespace Engine
{
	namespace Rendering
	{
		AbstractMesh::AbstractMesh() :
			model(Model()),
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
			hsvcolor(HSVColor(color)),
			rgbcolor(color),
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

		AbstractMesh& AbstractMesh::SetModel(const Model& model)
		{
			this->model = Model(model);

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
