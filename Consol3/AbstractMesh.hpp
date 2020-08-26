#ifndef MESH_HPP
#define MESH_HPP

#include "Color.hpp"
#include "Model.hpp"
#include "Vector3.hpp"
#include "Angle.hpp"

using namespace Display;

namespace Engine
{
	namespace Rendering
	{
		class AbstractMesh
		{
		private:
			Model model;
			Color color;

			Vector3 position;
			Angle rotation;
			Vector3 scale;

		protected:
			AbstractMesh(const Model& model, const Vector3& position);
			AbstractMesh(const Model& model, const Vector3& position, const Color& color);
			AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation);
			AbstractMesh(const Model& model, const Vector3& position, const Angle& rotation, const Color& color);
			
			[[nodiscard]] Model GetModel() const;
			[[nodiscard]] Color GetColor() const;
			[[nodiscard]] Vector3 GetPosition() const;
			[[nodiscard]] Angle GetRotation() const;
			[[nodiscard]] Vector3 GetScale() const;

			AbstractMesh& SetModel(const Model& model);
			AbstractMesh& SetColor(const Color& color);
			AbstractMesh& SetPosition(const Vector3& position);
			AbstractMesh& SetRotation(const Angle& rotation);
			AbstractMesh& SetScale(const Vector3& scale);

			virtual void DrawMesh() const = 0;
		};
	}
}

#endif
