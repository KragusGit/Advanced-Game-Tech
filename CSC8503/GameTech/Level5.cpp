#include "Level5.h"
#include "../CSC8503Common/PositionConstraint.h"



LevelFive::LevelFive(GameTechRenderer* renderer) :Level(renderer) {
	CreateWorld();
	//physics->usePenaltyResolution = true;
}

void LevelFive::CreateWorld() {

	AddOBBFloorToWorld(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(25, 2, 50), 0.85)->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));
	AddOBBCubeToWorld(Vector3(5, 20, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), 1)->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
	AddSphereToWorld(Vector3(-5, 20, 0), 2, 1)->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
}

LevelFive::~LevelFive() {

}

void LevelFive::UpdateGame(float dt) {
	world->GetMainCamera()->UpdateCamera(dt);
	//CameraFollow(playerObj, dt);

	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();

	Debug::Print("You cannot do much in this level.", Vector2(1, 6),Vector4(0,0,0,0),15);
	Debug::Print("It is just to demonstrate the attempt to simulate ", Vector2(1, 8),Vector4(0,0,0,0), 15);
	Debug::Print("OBB->OBB ,OBB->Sphere & Penalty Resolution! ", Vector2(1, 10),Vector4(0,0,0,0), 15);
	Debug::Print("This does not work every time so press F5 and hope ", Vector2(1, 12), Vector4(0, 0, 0, 0), 15);
	Debug::Print("that it works after objects fall down through the floor.", Vector2(1, 14), Vector4(0, 0, 0, 0), 15);
	Debug::Print("Press ESC to quit!", Vector2(1, 16), Vector4(0, 0, 0, 0), 15);
	Debug::Print("Press F5 to reset!", Vector2(1, 18), Vector4(0, 0, 0, 0), 15);
	Debug::Print("Press F to toggle penalty resolution!", Vector2(1, 20), Vector4(0, 0, 0, 0), 15);
	Debug::Print("Using penalty resolution:", Vector2(1, 22), Vector4(0, 0, 0, 0), 15);
	Debug::Print(physics->usePenaltyResolution ? "True" : "False", Vector2(60, 22), Vector4(0, 0, 0, 0), 15);

	Debug::FlushRenderables(dt);
	/*if (playerObj != nullptr)
		playerObj->Update(dt);*/

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F)) {
		physics->usePenaltyResolution = !physics->usePenaltyResolution;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F5))

	{
		world->ClearAndErase();
		CreateWorld();
	}



	/*if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {

		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			((GameObject*)closestCollision.node)->OnClick();
		}
	}*/
	physics->Update(dt);
}

void LevelFive::CameraFollow(GameObject* obj, float dt) {
	Vector3 objPos = obj->GetTransform().GetPosition();
	Vector3 camPos = objPos + ((obj->GetTransform().GetOrientation() * cameraDistanceFromPlayer));

	Matrix4 temp = Matrix4::BuildViewMatrix(world->GetMainCamera()->GetPosition(), objPos, Vector3(0, 1, 0));

	Matrix4 modelMat = temp.Inverse();

	Quaternion q(modelMat);
	Vector3 angles = q.ToEuler(); //nearly there now!

	Vector3 camMoveDir = camPos - world->GetMainCamera()->GetPosition();
	if (camMoveDir.LengthSquared() > 0.2f) {
		world->GetMainCamera()->SetPosition(world->GetMainCamera()->GetPosition() + (camMoveDir * dt));
	}

	world->GetMainCamera()->SetPitch(angles.x);
	world->GetMainCamera()->SetYaw(angles.y);
}

PlayerObject* LevelFive::SpawnPlayer(const Vector3& position) {
	float meshSize = 1;
	float inverseMass = 0.5f;

	PlayerObject* character = new PlayerObject();

	SphereVolume* volume = new SphereVolume(meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);


	character->SetRenderObject(new RenderObject(&character->GetTransform(), sphereMesh, basicTex, basicShader));

	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetPhysicsObject()->SetFriction(1);
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}
