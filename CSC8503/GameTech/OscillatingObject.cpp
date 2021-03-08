#include "OscillatingObject.h"

using namespace NCL;
using namespace CSC8503;
OscillatingObject::OscillatingObject(vector<Vector3>positions, bool startOscillatingOnSpawn, float timeToTravelOneLeg, float waitAfterRest) {
	this->positions = positions;
	this->timeToTravelOneLeg = timeToTravelOneLeg;
	autoOscillate = startOscillatingOnSpawn;
	secondsToWaitBeforeMoving = waitAfterRest;
	restingTimer = secondsToWaitBeforeMoving;
	stateMachine = new StateMachine();
	State* movingState = new State([&](float dt)-> void {
		Move(dt);
		});

	State* restingState = new State([&](float dt)-> void {//do nothing
		if (restingTimer >= 0)
			restingTimer -= dt;
		});

	StateTransition* restToMotion = new StateTransition(restingState, movingState, [&](void)->bool {
		return (autoOscillate || moving) && restingTimer <= 0;
		});

	StateTransition* motionToRest = new StateTransition(movingState, restingState, [&](void)->bool {
		return !moving;
		});

	stateMachine->onStateChange = [&](State* A, State* B) {
		restingTimer = secondsToWaitBeforeMoving;
	};


	stateMachine->AddState(movingState);
	stateMachine->AddState(restingState);
	stateMachine->AddTransition(restToMotion);
	stateMachine->AddTransition(motionToRest);

}



OscillatingObject::~OscillatingObject() {
	delete stateMachine;
}

void OscillatingObject::Update(float dt) {
	stateMachine->Update(dt);
}

void NCL::CSC8503::OscillatingObject::GoToNextState() {
	if (!moving) {
		moving = true;
		elapsedTime = 0;
	}
}

void OscillatingObject::ToggleAutoOscillate() {
	autoOscillate = !autoOscillate;
}

void OscillatingObject::Move(float dt) {

	Vector3 toPos = positions[currentPositionIndex];
	Vector3 fromPos = positions[currentPositionIndex > 0 ? currentPositionIndex - 1 : positions.size() - 1];
	if ((toPos - transform.GetPosition()).LengthSquared() < 0.05f) {
		transform.SetPosition(toPos);
		moving = false;
		elapsedTime = 0;
		currentPositionIndex = currentPositionIndex < positions.size() - 1 ? currentPositionIndex + 1 : 0;
	}
	else {
		moving = true;
		float t = elapsedTime / timeToTravelOneLeg;
		transform.SetPosition(Lerp(fromPos, toPos, t));
		elapsedTime += dt;
	}
}
