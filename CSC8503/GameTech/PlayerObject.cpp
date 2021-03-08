#include "PlayerObject.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"
#include "../CSC8503Common/Debug.h"

using namespace NCL::CSC8503;


Vector3  GetForward(Quaternion rotation) {
	return rotation * Vector3(0, 0, 1);
}

void PlayerObject::Update(float dt) {
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::UP)) {
		MoveFwd(dt);
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::LEFT)) {
		TurnLeft(dt);
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::RIGHT)) {
		TurnRight(dt);
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::DOWN)) {
		MoveBack(dt);
	}

	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::SPACE)) {
		Jump(dt);
	}
}

void PlayerObject::Jump(float dt) {
	if (!inAir) {
		GetPhysicsObject()->AddForce(Vector3(0, 1, 0) * 5000);
		inAir = true;
	}
}


void PlayerObject::TurnLeft(float dt) {
	Quaternion rotation = GetTransform().GetOrientation();
	//	GetTransform().SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), rotation.ToEuler().y + (20 * dt * 5)));
	GetPhysicsObject()->AddTorque(Vector3(0, 30, 0));
}

void PlayerObject::TurnRight(float dt) {
	Quaternion rotation = GetTransform().GetOrientation();
	GetPhysicsObject()->AddTorque(Vector3(0, -30, 0));
	//GetTransform().SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), rotation.ToEuler().y - (20 * dt * 5)));
}

void PlayerObject::MoveFwd(float dt) {
	GetPhysicsObject()->AddForce(GetForward(transform.GetOrientation()) * -(speed * dt));
}

void PlayerObject::MoveBack(float dt) {
	GetPhysicsObject()->AddForce(GetForward(transform.GetOrientation()) * (speed * dt));
}

void PlayerObject::OnCollisionBegin(GameObject* colObj) {
	if (colObj->GetTag() == Tag::Floor) {
		inAir = false;
		GetPhysicsObject()->UpdateExternalFriction(colObj->GetPhysicsObject()->GetFriction());
	}
}


