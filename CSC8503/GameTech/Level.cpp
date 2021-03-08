#include "Level.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Assets.h"

#include <fstream>;

using namespace NCL;
using namespace CSC8503;
Level::Level(GameTechRenderer* renderer) {
	
	this->renderer = renderer;
	world = renderer->GetWorld();
	
	physics = new PhysicsSystem(*world);
	useGravity = true;
	SetGlobalGravity(Vector3(0, -9.8 * 5, 0));
	physics->UseGravity(useGravity);
	Debug::SetRenderer(renderer);

	InitialiseAssets();
	InitBasicSystems();
}

Level::~Level() {
	delete cubeMesh;
	delete sphereMesh;
	delete basicTex;
	delete basicShader;
	delete physics;
	world->ClearAndErase();
}

GameObject* Level::AddOBBFloorToWorld(const Vector3& position, const Vector3& rotation, const Vector3& size, float friction) {
	GameObject* floor = new GameObject();
	OBBVolume* volume = new OBBVolume(size);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(size * 2)
		.SetPosition(position)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->SetTag(Tag::Floor);
	floor->GetPhysicsObject()->SetFriction(friction);
	world->AddGameObject(floor);

	return floor;
}
GameObject* Level::AddFloorToWorld(const Vector3& position, const Vector3& rotation, const Vector3& size, float friction) {
	GameObject* floor = new GameObject();


	AABBVolume* volume = new AABBVolume(size);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(size * 2)
		.SetPosition(position)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->SetTag(Tag::Floor);
	floor->GetPhysicsObject()->SetFriction(friction);
	world->AddGameObject(floor);

	return floor;
}

GameObject* Level::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

Button* Level::AddButtonToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass) {
	Button* cube = new Button();

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

Trigger* Level::AddTriggerToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass, bool drawMesh) {
	Trigger* cube = new Trigger();

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));
	if (drawMesh)
		cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* Level::AddOBBCubeToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	OBBVolume* volume = new OBBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* Level::AddAABBCubeToWorld(const Vector3& position, const Vector3& rotation, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}



vector<GameObject*> Level::AddCubesFromFile(const std::string& filename, VolumeType volType) {

	std::ifstream infile(Assets::DATADIR + filename);
	vector<GameObject*> objs;

	int staticObjCount;
	int totalObjects;
	Vector3 pos;
	Vector3 size;
	Vector3 rot;
	infile >> staticObjCount;
	infile >> totalObjects;
	vector<Vector3> objProperties;

	for (int i = 0; i < totalObjects; i++) {
		objProperties.clear();
		ReadObjCoordinates(infile, objProperties);
		GameObject* cube = nullptr;
		if (volType == VolumeType::AABB)
			cube = AddAABBCubeToWorld(objProperties[0], objProperties[2], objProperties[1] * 0.5f, 0);
		else
			cube = AddOBBCubeToWorld(objProperties[0], objProperties[2], objProperties[1] * 0.5f, 0);//switch to obb

		if (i < staticObjCount) {
			cube->GetPhysicsObject()->SetFriction(2);
			cube->SetTag(Tag::Floor);
		}
		objs.push_back(cube);
	}

	return objs;
}




OscillatingObject* Level::SpawnOscillatingObject(const Vector3& position, const Vector3& size, vector<Vector3> positions, bool startOnSpawn, float timeToTravelOneLeg, float timeToWait, OGLMesh* mesh, float inverseMass) {
	OscillatingObject* oscillator = new OscillatingObject(positions, startOnSpawn, timeToTravelOneLeg, timeToWait);
	if (mesh == nullptr)
		mesh = cubeMesh;
	AABBVolume* volume = new AABBVolume(size);

	oscillator->SetBoundingVolume((CollisionVolume*)volume);

	oscillator->GetTransform()
		.SetPosition(position)
		.SetScale(size * 2);


	oscillator->SetRenderObject(new RenderObject(&oscillator->GetTransform(), mesh, basicTex, basicShader));
	oscillator->SetPhysicsObject(new PhysicsObject(&oscillator->GetTransform(), oscillator->GetBoundingVolume()));

	oscillator->GetPhysicsObject()->SetInverseMass(inverseMass);
	oscillator->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(oscillator);

	return oscillator;
}

void Level::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

void Level::InitBasicSystems() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-20);
	world->GetMainCamera()->SetYaw(0);
	world->GetMainCamera()->SetPosition(Vector3(0, 20, 60));
	world->ClearAndErase();
	physics->Clear();
}

void Level::ReadObjCoordinates(std::ifstream& file, vector<Vector3>& element) {
	for (int i = 0; i < 3; ++i) {
		Vector3 temp;
		file >> temp.x;
		file >> temp.y;
		file >> temp.z;
		element.emplace_back(temp);
	}
}

void Level::ToggleGravity() {
	useGravity = !useGravity;
	physics->UseGravity(useGravity);
}

void Level::InverseGravity() {
	SetGlobalGravity(globalGravity * Vector3(1, -1, 1));
}

void Level::SetGlobalGravity(Vector3 gravity) {
	globalGravity = gravity;
	physics->SetGravity(gravity);
}
