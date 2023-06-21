#include "Primitives.h"

#include "engine/managers/Manager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/graphics/VertexArray.h"
#include "engine/graphics/VertexBuffer.h"

namespace MYENGINE
{

	void Primitives::CreateVertArray(weak(Manager) _manager, shared(VertexBuffer) verts, shared(VertexBuffer) texCoords, shared(VertexBuffer) norms, const char* name)
	{
		shared(VertexArray) vertArray = _manager.lock()->GetResourceManager()->CreateVertexArray();

		vertArray->SetBuffer(0, verts);
		if (texCoords) vertArray->SetBuffer(1, texCoords);
		if (norms) vertArray->SetBuffer(2, norms);
		vertArray->SetFilePath(name);
	}

	void Primitives::CreatePlane(weak(Manager) _manager)
	{
		shared(VertexBuffer) planeVerts = make_shared(VertexBuffer)();
		planeVerts->Add(glm::vec3(-0.5f, 0.0f, -0.5f));
		planeVerts->Add(glm::vec3(-0.5f, 0.0f, 0.5f));
		planeVerts->Add(glm::vec3(0.5f, 0.0f, 0.5f));
		planeVerts->Add(glm::vec3(0.5f, 0.0f, 0.5f));
		planeVerts->Add(glm::vec3(0.5f, 0.0f, -0.5f));
		planeVerts->Add(glm::vec3(-0.5f, 0.0f, -0.5f));

		shared(VertexBuffer) planeTexCoords = make_shared(VertexBuffer)();
		planeTexCoords->Add(glm::vec2(0.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(0.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(0.0f, 0.0f));

		shared(VertexBuffer) planeNorms = make_shared(VertexBuffer)();
		planeNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		planeNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		planeNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		planeNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		planeNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		planeNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));

		CreateVertArray(_manager, planeVerts, planeTexCoords, planeNorms, "plane");
	}

	void Primitives::CreateVerticalPlane(weak(Manager) _manager)
	{
		shared(VertexBuffer) planeVerts = make_shared(VertexBuffer)();
		planeVerts->Add(glm::vec3(-0.5f, -0.5f, 0.0f));
		planeVerts->Add(glm::vec3(0.5f, -0.5f, 0.0f));
		planeVerts->Add(glm::vec3(0.5f, 0.5f, 0.0f));
		planeVerts->Add(glm::vec3(0.5f, 0.5f, 0.0f));
		planeVerts->Add(glm::vec3(-0.5f, 0.5f, 0.0f));
		planeVerts->Add(glm::vec3(-0.5f, -0.5f, 0.0f));

		shared(VertexBuffer) planeTexCoords = make_shared(VertexBuffer)();
		planeTexCoords->Add(glm::vec2(0.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(0.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(0.0f, 1.0f));

		shared(VertexBuffer) planeNorms = make_shared(VertexBuffer)();
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));

		CreateVertArray(_manager, planeVerts, planeTexCoords, planeNorms, "plane_vertical");
	}

	void Primitives::CreateTextureFlippedVerticalPlane(weak(Manager) _manager)
	{
		shared(VertexBuffer) planeVerts = make_shared(VertexBuffer)();
		planeVerts->Add(glm::vec3(-0.5f, -0.5f, 0.0f));
		planeVerts->Add(glm::vec3(0.5f, -0.5f, 0.0f));
		planeVerts->Add(glm::vec3(0.5f, 0.5f, 0.0f));
		planeVerts->Add(glm::vec3(0.5f, 0.5f, 0.0f));
		planeVerts->Add(glm::vec3(-0.5f, 0.5f, 0.0f));
		planeVerts->Add(glm::vec3(-0.5f, -0.5f, 0.0f));

		shared(VertexBuffer) planeTexCoords = make_shared(VertexBuffer)();
		planeTexCoords->Add(glm::vec2(0.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 0.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(1.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(0.0f, 1.0f));
		planeTexCoords->Add(glm::vec2(0.0f, 0.0f));

		shared(VertexBuffer) planeNorms = make_shared(VertexBuffer)();
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		planeNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));

		CreateVertArray(_manager, planeVerts, planeTexCoords, planeNorms, "plane_vertical_flip");
	}

	void Primitives::CreateBox(weak(Manager) _manager)
	{
		shared(VertexBuffer) boxVerts = make_shared(VertexBuffer)();
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, -0.5f)); // Up
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, -0.5f));
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, -0.5f));

		boxVerts->Add(glm::vec3(-0.5f, -0.5f, 0.5f)); // Down
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, 0.5f));

		boxVerts->Add(glm::vec3(0.5f, 0.5f, -0.5f)); // Front
		boxVerts->Add(glm::vec3(0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, -0.5f));

		boxVerts->Add(glm::vec3(-0.5f, 0.5f, 0.5f)); // Back
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, 0.5f));
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, 0.5f));

		boxVerts->Add(glm::vec3(-0.5f, 0.5f, -0.5f)); // Left
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(-0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, 0.5f));
		boxVerts->Add(glm::vec3(-0.5f, 0.5f, -0.5f));

		boxVerts->Add(glm::vec3(0.5f, 0.5f, 0.5f)); // Right
		boxVerts->Add(glm::vec3(0.5f, -0.5f, 0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, -0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, -0.5f));
		boxVerts->Add(glm::vec3(0.5f, 0.5f, 0.5f));


		shared(VertexBuffer) boxTexCoords = make_shared(VertexBuffer)();
		boxTexCoords->Add(glm::vec2(0.75f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.75f, 0.75f));
		boxTexCoords->Add(glm::vec2(1.0f, 0.75f));
		boxTexCoords->Add(glm::vec2(1.0f, 0.75f));
		boxTexCoords->Add(glm::vec2(1.0f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.75f, 0.5f));

		boxTexCoords->Add(glm::vec2(0.25f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.5f));

		boxTexCoords->Add(glm::vec2(0.25f, 0.25f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.25f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.25f));

		boxTexCoords->Add(glm::vec2(0.25f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.25f, 1.0f));
		boxTexCoords->Add(glm::vec2(0.5f, 1.0f));
		boxTexCoords->Add(glm::vec2(0.5f, 1.0f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.75f));

		boxTexCoords->Add(glm::vec2(0.0f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.0f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.25f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.0f, 0.5f));

		boxTexCoords->Add(glm::vec2(0.5f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.75f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.75f, 0.75f));
		boxTexCoords->Add(glm::vec2(0.75f, 0.5f));
		boxTexCoords->Add(glm::vec2(0.5f, 0.5f));


		shared(VertexBuffer) boxNorms = make_shared(VertexBuffer)();
		boxNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));

		boxNorms->Add(glm::vec3(0.0f, -1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, -1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, -1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, -1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, -1.0f, 0.0f));
		boxNorms->Add(glm::vec3(0.0f, -1.0f, 0.0f));

		boxNorms->Add(glm::vec3(0.0f, 0.0f, -1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, -1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, -1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, -1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, -1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, -1.0f));

		boxNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));
		boxNorms->Add(glm::vec3(0.0f, 0.0f, 1.0f));

		boxNorms->Add(glm::vec3(-1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(-1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(-1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(-1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(-1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(-1.0f, 0.0f, 0.0f));

		boxNorms->Add(glm::vec3(1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(1.0f, 0.0f, 0.0f));
		boxNorms->Add(glm::vec3(1.0f, 0.0f, 0.0f));

		CreateVertArray(_manager, boxVerts, boxTexCoords, boxNorms, "box");
	}

	void Primitives::CreateSkyBox(weak(Manager) _manager)
	{
		shared(VertexBuffer) boxVerts = make_shared(VertexBuffer)();
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, -1.0f));

		boxVerts->Add(glm::vec3(-1.0f, -1.0f, 1.0f));
		boxVerts->Add(glm::vec3(-1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(-1.0f, -1.0f, 1.0f));

		boxVerts->Add(glm::vec3(1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, -1.0f));

		boxVerts->Add(glm::vec3(-1.0f, -1.0f, 1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, 1.0f));
		boxVerts->Add(glm::vec3(-1.0f, -1.0f, 1.0f));

		boxVerts->Add(glm::vec3(-1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, 1.0f));
		boxVerts->Add(glm::vec3(-1.0f, 1.0f, -1.0f));

		boxVerts->Add(glm::vec3(-1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, -1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, -1.0f));
		boxVerts->Add(glm::vec3(-1.0f, -1.0f, 1.0f));
		boxVerts->Add(glm::vec3(1.0f, -1.0f, 1.0f));

		CreateVertArray(_manager, boxVerts, nullptr, nullptr, "skybox");
	}

	void Primitives::CreateTriangle(weak(Manager) _manager)
	{
		shared(VertexBuffer) triVerts = make_shared(VertexBuffer)();
		triVerts->Add(glm::vec3(-0.5f, 0.0f, -0.5f));
		triVerts->Add(glm::vec3(-0.5f, 0.0f, 0.5f));
		triVerts->Add(glm::vec3(0.5f, 0.0f, 0.5f));

		shared(VertexBuffer) triTexCoords = make_shared(VertexBuffer)();
		triTexCoords->Add(glm::vec2(0.0f, 0.0f));
		triTexCoords->Add(glm::vec2(0.0f, -1.0f));
		triTexCoords->Add(glm::vec2(1.0f, -1.0f));

		shared(VertexBuffer) triNorms = make_shared(VertexBuffer)();
		triNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		triNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		triNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));

		CreateVertArray(_manager, triVerts, triTexCoords, triNorms, "triangle");
	}

	void Primitives::CreateEqTriangle(weak(Manager) _manager)
	{
		shared(VertexBuffer) triVerts = make_shared(VertexBuffer)();
		triVerts->Add(glm::vec3(0.0f, 0.0f, 0.0f));
		triVerts->Add(glm::vec3(-0.5f, 0.0f, 0.5f));
		triVerts->Add(glm::vec3(0.5f, 0.0f, 0.5f));

		shared(VertexBuffer) triTexCoords = make_shared(VertexBuffer)();
		triTexCoords->Add(glm::vec2(0.5f, 0.5f));
		triTexCoords->Add(glm::vec2(-1.0f, 1.0f));
		triTexCoords->Add(glm::vec2(1.0f, 1.0f));

		shared(VertexBuffer) triNorms = make_shared(VertexBuffer)();
		triNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		triNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));
		triNorms->Add(glm::vec3(0.0f, 1.0f, 0.0f));

		CreateVertArray(_manager, triVerts, triTexCoords, triNorms, "triangle_eq");
	}

} // namespace MYENGINE
