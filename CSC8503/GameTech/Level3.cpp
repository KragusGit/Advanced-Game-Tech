#include "Level3.h"



LevelThree::LevelThree(GameTechRenderer* renderer) :Level(renderer) {
	CreateWorld();
	playerObj = SpawnPlayer(Vector3(0, 5, 0));
	playerObj->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
	playerObj->SetCustomGravity((physics->GetGravity()) * -1);
	cameraDistanceFromPlayer = Vector3(0, 5, 8);
}

void LevelThree::CreateWorld() {

	AddFloorToWorld(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(25, 0, 50), 3);

	fusRoDah = AddTriggerToWorld(Vector3(0, 2, -25), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), 0, true);

	fusRoDah->SubscribeOnEnter([&](GameObject* object)-> void {
		if (object == playerObj)
			CollectBonus(fusRoDah);
		});
	//Heavy object
	AddAABBCubeToWorld(Vector3(0, 2, -45), Vector3(0, 0, 0), Vector3(1, 1, 1), 9999)->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));

	//Winning Door
	AddAABBCubeToWorld(Vector3(3, 4, -50), Vector3(0, 0, 0), Vector3(0.5, 4, 0.5), 0)->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
	AddAABBCubeToWorld(Vector3(-3, 4, -50), Vector3(0, 0, 0), Vector3(0.5, 4, 0.5), 0)->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	//AddTrigger
	Trigger* trigger2 = AddTriggerToWorld(Vector3(0, 4, -50.22), Vector3(0, 0, 0), Vector3(2.5, 4, 3), 0);
	trigger2->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj)OnGameCompleted(); });

	//Add Fall Triggers
	Trigger* trigger3 = AddTriggerToWorld(Vector3(0, -10, -100), Vector3(0, 0, 0), Vector3(100, 1, 150), 0);
	Trigger* trigger4 = AddTriggerToWorld(Vector3(0, 80, -100), Vector3(0, 0, 0), Vector3(100, 1, 150), 0);
	trigger3->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj)OnGameFailed(); });
	trigger3->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj)OnGameFailed(); });


}

void LevelThree::CollectBonus(Trigger* bonus) {
	fusRoDah->SetActive(false);
	playerObj->GetPhysicsObject()->AddForce((playerObj->GetTransform().GetOrientation() * Vector3(0, 0, -1)) * 50000);
}


LevelThree::~LevelThree() {

}

void LevelThree::UpdateGame(float dt) {
	//world->GetMainCamera()->UpdateCamera(dt);
	CameraFollow(playerObj, dt);


	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();
	Debug::FlushRenderables(dt);
	if (playerObj != nullptr)
		playerObj->Update(dt);

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {


		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			((GameObject*)closestCollision.node)->OnClick();
		}
	}
	physics->Update(dt);
}

void LevelThree::CameraFollow(GameObject* obj, float dt) {
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

PlayerObject* LevelThree::SpawnPlayer(const Vector3& position) {
	float meshSize = 1.5f;
	float inverseMass = 0.5f;

	PlayerObject* character = new PlayerObject();

	AABBVolume* volume = new AABBVolume(Vector3(meshSize * 0.5f, meshSize * 0.5f, meshSize * 0.5f));

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);


	character->SetRenderObject(new RenderObject(&character->GetTransform(), cubeMesh, basicTex, basicShader));

	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
	character->GetPhysicsObject()->SetFriction(1);
	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}
