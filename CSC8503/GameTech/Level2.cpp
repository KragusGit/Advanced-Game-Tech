#include "Level2.h"

LevelTwo::LevelTwo(GameTechRenderer* renderer) :Level(renderer) {
	playerObj = SpawnPlayer(NodeIndexToWorldPos(1, 1) + Vector3(-3, 12, -1));
	playerObj->GetRenderObject()->SetColour(Vector4(0, 1, 1, 1));
	playerObj->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(1, 0, 0));
	world->GetMainCamera()->SetPosition(playerObj->GetTransform().GetPosition() + cameraDistanceFromPlayer);
	cameraDistanceFromPlayer = Vector3(0, 5, 8);
	CreateWorld();
	AI = SpawnAI(NodeIndexToWorldPos(1, 1) + Vector3(-3, 12, 1));
	AI->GetTransform().SetOrientation(Quaternion::EulerAnglesToQuaternion(1, 0, 0));

	BehaviourAction* GetBonus = new BehaviourAction("Getting Bonus",
		[&](float dt, BehaviourState state)-> BehaviourState {
			if (state == Initialise) {
				if (AI->GetCurrentTarget() != nullptr) {
					(AI->GetCurrentTarget())->SubscribeOnEnter([&](GameObject* object)-> void {
						if (object == AI) {
							AI->GetCurrentTarget()->SetActive(false);
							AI->SetSpeed(7000);
						}
						state = Success;
						});

					state = Ongoing;
				}
				else
					state = Failure;
			}
			else if (state == Ongoing) {

				if (AI->GetCurrentTarget()->IsActive())
					AI->Goto(AI->GetCurrentTarget()->GetTransform().GetPosition(), dt);
				else
					state = Success;
			}

			else if (state == Success) {
				AI->SetCurrentTarget(nullptr);
				Vector3 curPos = AI->GetTransform().GetPosition();
				NavigationPath path;
				grid->FindPath(NodeIndexToWorldPos(curPos.x / nodeSize, curPos.z / nodeSize), raceEndPos, path);
				vector<Vector3> points;
				Vector3 point;
				while (path.PopWaypoint(point)) {
					points.push_back(GridposToWorldPos(point));
				}
				AI->SetPathNodes(points);

			}
			return state; // will be ’ongoing ’ until success
		}
	);


	BehaviourAction* AIBehaviour = new BehaviourAction("Root Race Behaviour",
		[&](float dt, BehaviourState state)-> BehaviourState {
			if (state == Initialise) {

				NavigationPath path;
				grid->FindPath(raceStartPos, raceEndPos, path);
				vector<Vector3> points;
				Vector3 point;
				while (path.PopWaypoint(point)) {
					points.push_back(GridposToWorldPos(point));
				}
				AI->SetPathNodes(points);

				state = Ongoing;
			}
			else if (state == Ongoing) {

				elapsedBonusTimer += dt;
				if (elapsedBonusTimer >= bonusCheckTimer) {
					elapsedBonusTimer = 0;
					Trigger* bonus;
					bool bonusInSight = CheckForBonus(AI->GetTransform().GetPosition(), bonus);
					if (bonusInSight) {
						AI->SetCurrentTarget(bonus);
					}
				}
				if (AI->GetCurrentTarget() != nullptr && AI->GetCurrentTarget()->IsActive()) {
					AI->bonusBehaviour->Execute(dt);
				}
				else {
					AI->FollowPath(dt);
					if (AI->HasReachedDestination())

						state = Success;
				}
			}
			return state; // will be ’ongoing ’ until success
		}
	);

	AI->mainBehaviour = AIBehaviour;
	AI->bonusBehaviour = GetBonus;
	AI->move = true;
}

bool LevelTwo::CheckForBonus(Vector3 actorPos, Trigger*& bonus)

{
	if (bonuses.size() > 0) {
		int closestIndex = 0;
		int closestDistance = FLT_MAX;
		int dist;
		for (int i = 0; i < bonuses.size(); i++) {
			dist = (bonuses[i]->GetTransform().GetPosition() - actorPos).LengthSquared();
			if (dist < closestDistance) {
				closestDistance = dist;
				closestIndex = i;
			}
		}

		if (closestDistance < 3) {
			Ray ray = Ray(actorPos, (bonuses[closestIndex]->GetTransform().GetPosition() - actorPos).Normalised());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				if (((GameObject*)closestCollision.node) == bonuses[closestIndex]) {
					bonus = bonuses[closestIndex];
					return true;
				}
			}
		}
	}
	return false;
}

void LevelTwo::CreateWorld() {


	LoadNavigationGrid("TestGrid1.txt");
	AddCubesFromGrid(grid, VolumeType::AABB);


	bonuses.push_back(AddTriggerToWorld(GridposToWorldPos(Vector3(20, 0, 60)) + Vector3(0, 2.5, 0), Vector3(0, 0, 0), Vector3(0.5, 2, 0.5), 0, true));
	bonuses[0]->SubscribeOnEnter([&](GameObject* object)-> void {if (object == playerObj) { playerObj->speed = 7000; bonuses[0]->SetActive(false); } });

	//Winning Door
	AddAABBCubeToWorld(Vector3(75, 5, -108), Vector3(0, 0, 0), Vector3(0.5, 5, 0.5), 0)->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
	AddAABBCubeToWorld(Vector3(75, 5, -122), Vector3(0, 0, 0), Vector3(0.5, 5, 0.5), 0)->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));

	//AddTrigger
	Trigger* trigger2 = AddTriggerToWorld(Vector3(76, 2, -115), Vector3(0, 0, 0), Vector3(2, 5, 14), 0, true);
	trigger2->SubscribeOnEnter([&](GameObject* object)-> void {
		if (object == playerObj)
			OnGameCompleted();
		else if (object == AI)
			OnGameFailed();
		});

}


LevelTwo::~LevelTwo() {
	delete playerObj;
	delete AI;
	world->ClearAndErase();
}

void LevelTwo::UpdateGame(float dt) {
	//world->GetMainCamera()->UpdateCamera(dt);
	CameraFollow(playerObj, dt);

	if (initialCountdown > 0.0f) {
		initialCountdown -= dt;
		Debug::Print("Race Starts In", Vector2(45, 50), Vector4(1, 1, 1, 1), 30);
		Debug::Print(std::to_string(initialCountdown), Vector2(50, 60), Vector4(0, 0, 0, 1), 40);
		Debug::FlushRenderables(dt);
		renderer->Update(dt);
		world->UpdateWorld(dt);
		renderer->Render();
		physics->Update(dt);

		return;
	}


	if (AI != nullptr)
		AI->Update(dt);
	Debug::FlushRenderables(dt);
	if (playerObj != nullptr)
		playerObj->Update(dt);

	renderer->Update(dt);
	world->UpdateWorld(dt);
	renderer->Render();
	physics->Update(dt);


}

void LevelTwo::CameraFollow(GameObject* obj, float dt) {
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

PlayerObject* LevelTwo::SpawnPlayer(const Vector3& position) {
	float meshSize = 1.5f;
	float inverseMass = 0.5f;

	PlayerObject* character = new PlayerObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.5, 0.5, 0.5) * meshSize);

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
AIAgent* LevelTwo::SpawnAI(const Vector3& position) {
	float meshSize = 1.5f;
	float inverseMass = 0.5f;

	AIAgent* character = new AIAgent();

	AABBVolume* volume = new AABBVolume(Vector3(0.5, 0.5, 0.5) * meshSize);

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


void LevelTwo::LoadNavigationGrid(const std::string& filename) {
	grid = new NavigationGrid(filename);
}


vector<GameObject*> LevelTwo::AddCubesFromGrid(NavigationGrid* grid, VolumeType volType) {

	vector<GameObject*> objs;

	int gridWIdth = grid->GetGridWidth();
	int gridHeight = grid->GetGridHeight();
	int height = 0;

	Vector3 floorDimention = Vector3(1, 0.2, 1) * nodeSize;
	Vector3 wallDimention = Vector3(1, 2, 1) * nodeSize;
	for (int i = 0; i < grid->GetGridWidth() * grid->GetGridHeight(); i++) {

		int col = i / gridWIdth;
		int row = i % gridWIdth;
		GameObject* obj;
		Vector3 pos = gridStartPosition + Vector3((nodeSize * row), height, -(nodeSize * col));
		switch (grid->GetRoot()[i].type) {
		case  '.': {
			obj = AddFloorToWorld(pos + Vector3(0, floorDimention.y * 0.5f, 0), Vector3(0, 0, 0), floorDimention * 0.5f, 3);
			obj->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1)); break;
		}
		case  'x': {
			obj = AddAABBCubeToWorld(pos + Vector3(0, wallDimention.y * 0.5f, 0), Vector3(0, 0, 0), wallDimention * 0.5f, 0);
			obj->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1)); break;
		}
		}

	}
	return objs;
}

Vector3 NCL::CSC8503::LevelTwo::NodeIndexToWorldPos(Vector2 pos) { return NodeIndexToWorldPos(pos.x, pos.y); }

Vector3 NCL::CSC8503::LevelTwo::NodeIndexToWorldPos(int nodex, int nodey) {
	Vector3 position = Vector3(0, 0, 0);
	position.x = gridStartPosition.x + (nodex * nodeSize);
	position.y = gridStartPosition.y;
	position.z = -(gridStartPosition.z + (nodey * nodeSize));
	return position;
}

Vector2 NCL::CSC8503::LevelTwo::NodeIndexToGridPos(int nodex, int nodey) {
	return Vector2(nodex * grid->GetNodeSize(), nodey * grid->GetNodeSize());
}

Vector3 LevelTwo::GridposToWorldPos(Vector3 gridPos) {
	Vector3 pos = Vector3(0, 0, 0);
	gridPos /= grid->GetNodeSize();
	pos = NodeIndexToWorldPos(gridPos.x, gridPos.z);
	return pos;
}