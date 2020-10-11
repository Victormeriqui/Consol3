#ifndef MESH_HPP
#define MESH_HPP

#include "../../Display/RGBColor.hpp"
#include "../../Display/HSVColor.hpp"
#include "Model.hpp"
#include "../../Math/Vector3.hpp"
#include "../../Math/Angle.hpp"
#include "Transform.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		using namespace Math;

		class AbstractMesh
		{
		protected:
			Model model;
			
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
			
			[[nodiscard]] Model GetModel() const;
			[[nodiscard]] RGBColor GetColor() const;
			[[nodiscard]] Vector3 GetPosition() const;
			[[nodiscard]] Angle GetRotation() const;
			[[nodiscard]] Vector3 GetScale() const;

			AbstractMesh& SetModel(const Model& model);
			AbstractMesh& SetColor(const RGBColor& color);
			AbstractMesh& SetPosition(const Vector3& position);
			AbstractMesh& SetRotation(const Angle& rotation);
			AbstractMesh& SetScale(const Vector3& scale);

			virtual void DrawMesh(Rasterizer& rasterizer) const = 0;
		};
	}
}

#endif
