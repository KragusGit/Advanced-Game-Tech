#include "Level1.h"
#include "../CSC8503Common/Debug.cpp"
#include "../CSC8503Common/PositionConstraint.h"


LevelOne::LevelOne(GameTechRenderer* renderer) :Level(renderer) {
	CreateWorld();
	playerObj = SpawnPlayer(Vector3(0, 5, 0));
	playerObj->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
	playerObj->SetCustomGravity((physics->GetGravity()) * -1);
	cameraDistanceFromPlayer = Vector3(0, 5, 8);
}

void LevelOne::CreateWorld() {

	AddCubesFromFile("Coordinates.txt");

	//Lift
	vector<Vector3> liftPositions;
	liftPositions.push_back(Vector3(0, 0, -21.5));
	liftPositions.push_back(Vector3(0, 12, -21.5));
	lift = SpawnOscillatingObject(Vector3(0, -1, -21.5), Vector3(1.5, 0.2, 1.5), liftPositions, true, 5, 2, nullptr, 0);
	lift->SetTag(Tag::Floor);
	lift->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));

	//Button1
	Button* button = AddButtonToWorld(Vector3(5, 13, -44), Vector3(0, 0, 0), Vector3(0.25, 0.5, 0.05), 0);
	button->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	//ButtonStand
	AddAABBCubeToWorld(Vector3(5, 12, -44.5), Vector3(0, 0, 0), Vector3(0.5, 2.5, 0.5), 0)->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));

	//Stairs
	AddStairs(button);

	//Pendulum1
	GameObject* cube1;
	GameObject* cube2;
	cube1 = AddAABBCubeToWorld(Vector3(-4, 33, -59), Vector3(0, 0, 0), Vector3(1, 1, 1), 0);
	cube2 = AddAABBCubeToWorld(Vector3(-4, 30, -59), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), 1);
	PositionConstraint* c1 = new PositionConstraint(cube1, cube2, 3);
	pendulum1 = AddAABBCubeToWorld(Vector3(-4, 28, -59), Vector3(0, 0, 0), Vector3(0.5, 1, 0.5), 1);
	PositionConstraint* c2 = new PositionConstraint(pendulum1, cube2, 3);
	world->AddConstraint(c1);
	world->AddConstraint(c2);


	//Pendulum2
	GameObject* cube3;
	GameObject* cube4;
	cube3 = AddAABBCubeToWorld(Vector3(-4, 33, -62), Vector3(0, 0, 0), Vector3(1, 1, 1), 0);
	cube4 = AddAABBCubeToWorld(Vector3(-4, 30, -62), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), 1);
	PositionConstraint* c3 = new PositionConstraint(cube3, cube4, 3);
	pendulum2 = AddAABBCubeToWorld(Vector3(-4, 28, -62), Vector3(0, 0, 0), Vector3(0.5, 1, 0.5), 1);
	PositionConstraint* c4 = new PositionConstraint(pendulum2, cube4, 3);
	world->AddConstraint(c3);
	world->AddConstraint(c4);



	//Pendulum3
	GameObject* cube5;
	GameObject* cube6;
	cube5 = AddAABBCubeToWorld(Vector3(-4, 33, -65), Vector3(0, 0, 0), Vector3(1, 1, 1), 0);
	cube6 = AddAABBCubeToWorld(Vector3(-4, 30, -65), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), 1);
	PositionConstraint* c5 = new PositionConstraint(cube5, cube6, 3);
	pendulum3 = AddAABBCubeToWorld(Vector3(-4, 28, -65), Vector3(0, 0, 0), Vector3(0.5, 1, 0.5), 1);
	PositionConstraint* c6 = new PositionConstraint(pendulum3, cube6, 3);
	world->AddConstraint(c5);
	world->AddConstraint(c6);

	AddSphereToWorld(Vector3(0, 29, -90.5), 1, 1)->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));
	AddSphereToWorld(Vector3(0, 29, -92.5), 1, 1)->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));

	//Button2
	Button* button1 = AddButtonToWorld(Vector3(5, 29, -90.58), Vector3(0, 0, 0), Vector3(0.25, 0.5, 0.05), 0);
	button1->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	//ButtonStand2
	AddAABBCubeToWorld(Vector3(5, 28, -90.05), Vector3(0, 0, 0), Vector3(0.5, 2.5, 0.5), 0)->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));

	//AddPressurePlate
	GameObject* pp = AddAABBCubeToWorld(Vector3(5, 25.8, -93.61), Vector3(0, 0, 0), Vector3(1.5, 0.5, 1.5), 0);
	pp->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
	pp->GetPhysicsObject()->SetFriction(2);
	pp->SetTag(Tag::Floor);

	//AddTrigger
	Trigger* trigger1 = AddTriggerToWorld(Vector3(5, 26, -93.61), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5), 0, false);

	trigger1->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj) playerOnTrigger1 = true; });
	trigger1->SubscribeOnExit([&](GameObject* object)-> void {if (object == playerObj) playerOnTrigger1 = false; });
	button1->SubscribeOnPress([&]()-> void {
		if (playerOnTrigger1) {
			playerObj->ToggleUseOfCustomGravity();
			playerObj->GetPhysicsObject()->AddForce(Vector3(0, 1, 0) * (5000));
			cameraDistanceFromPlayer.y *= -1;
		}});

	//FinalObs
	AddFinalObstacle();

	//Winning Door
	AddAABBCubeToWorld(Vector3(3, 50, -167), Vector3(0, 0, 0), Vector3(0.5, 4, 0.5), 0)->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
	AddAABBCubeToWorld(Vector3(-3, 50, -167), Vector3(0, 0, 0), Vector3(0.5, 4, 0.5), 0)->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	//AddTrigger
	Trigger* trigger2 = AddTriggerToWorld(Vector3(0, 50, -168.22), Vector3(0, 0, 0), Vector3(2.5, 4, 0.5), 0);
	trigger2->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj)OnGameCompleted(); });

	//Add Fall Triggers
	Trigger* trigger3 = AddTriggerToWorld(Vector3(0, -10, -100), Vector3(0, 0, 0), Vector3(100, 1, 150), 0);
	Trigger* trigger4 = AddTriggerToWorld(Vector3(0, 80, -100), Vector3(0, 0, 0), Vector3(100, 1, 150), 0);
	trigger3->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj)OnGameFailed(); });
	trigger3->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj)OnGameFailed(); });


}

void LevelOne::AddFinalObstacle() {

	int obCount = 8;
	float obHDist = 10;

	Vector3 initialPos = Vector3(5, 50.78, -95);
	vector<Vector3> obState;
	for (int i = 0; i < obCount; i++) {
		obState.clear();
		obState.push_back(initialPos + Vector3(30 * ((i % 2 == 0) ? -1 : 1), 0, -(i * obHDist)));
		obState.push_back(initialPos + Vector3(30 * ((i % 2 == 0) ? 1 : -1), 0, -(i * obHDist)));
		finalObstacles.push_back(SpawnOscillatingObject(initialPos + Vector3(25 * ((i % 2 == 0) ? -1 : 1), 0, -(i * obHDist)), Vector3(0.5, 2.5, 0.5), obState, true, 2, 0, nullptr, 0));
	}
}

void LevelOne::AddStairs(Button* toggle) {

	int stairCount = 8;
	float stairHDist = 3;
	int stairVDist = 2.2;

	Vector3 initialStairPos = Vector3(-5, 25, -59);
	vector<Vector3> stairStatePositions;
	for (int i = 0; i < stairCount; i++) {
		stairStatePositions.clear();
		stairStatePositions.push_back(Vector3(-5, 25, -59 + (i * stairHDist)));
		stairStatePositions.push_back(Vector3(-5, 25 - (i * stairVDist), -59 + (i * stairHDist)));
		stairs.push_back(SpawnOscillatingObject(Vector3(-5, 25 - (i + stairVDist), -59 + (i * stairHDist)), Vector3(1.5, 1, 1.5), stairStatePositions, false, 2, 0, nullptr, 0));
		stairs[stairs.size() - 1]->SetTag(Tag::Floor);
		stairs[stairs.size() - 1]->GetPhysicsObject()->SetFriction(4);
	}
	toggle->SubscribeOnPress([&]()-> void {
		for (int i = 0; i < stairs.size(); i++)
			stairs[i]->GoToNextState();
		});

}

LevelOne::~LevelOne() {

}

void LevelOne::UpdateGame(float dt) {
	//world->GetMainCamera()->UpdateCamera(dt);
	CameraFollow(playerObj, dt);

	pendulum1->GetPhysicsObject()->AddForce(Vector3(1, 0, 0) * 50);
	pendulum2->GetPhysicsObject()->AddForce(Vector3(-1, 0, 0) * 50);
	pendulum3->GetPhysicsObject()->AddForce(Vector3(1, 0, 0) * 50);
	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();
	Debug::FlushRenderables(dt);
	if (playerObj != nullptr)
		playerObj->Update(dt);
	if (lift != nullptr)
		lift->Update(dt);

	for (int i = 0; i < stairs.size(); i++) {
		stairs[i]->Update(dt);
	}


	for (int i = 0; i < finalObstacles.size(); i++) {
		finalObstacles[i]->Update(dt);
	}

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {


		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			((GameObject*)closestCollision.node)->OnClick();
		}
	}
	physics->Update(dt);
}

void LevelOne::CameraFollow(GameObject* obj, float dt) {
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

PlayerObject* LevelOne::SpawnPlayer(const Vector3& position) {
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
