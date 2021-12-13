#include "StateGameObject.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject(ObjectMovement movement, int layer, string name) : GameObject(layer, name)
{
	counter = 0.0f;
	switch (movement)
	{
	case(ObjectMovement::MOVING): 
		InitMoving();
		break;
	case(ObjectMovement::ROTATING): 
		InitRotating();
		break;
	case(ObjectMovement::SPIN): 
		InitSpinning();
		break;
	case(ObjectMovement::DESTINATION): 
		InitDestination();
		break;
	case(ObjectMovement::TELEPORTER): 
		InitTeleporter();
		break;
	}
}

StateGameObject::~StateGameObject()
{
	delete stateMachine;
}

void StateGameObject::Update(float dt)
{
	stateMachine->Update(dt);
}
void StateGameObject::OnCollisionBegin(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL")
	{
		collisionWithPlayerBall = otherObject;
	}
}
void StateGameObject::OnCollisionEnd(GameObject* otherObject)
{
	if (otherObject->GetName() == "PLAYER BALL")
	{
		collisionWithPlayerBall = nullptr;
	}
}

void StateGameObject::InitMoving()
{
	counter = 0.5f;
	stateMachine = new StateMachine();

	State* goLeft = new State([&](float dt)->void
		{
			this->MoveLeft(dt);
		});
	State* goRight = new State([&](float dt)->void
		{
			this->MoveRight(dt);
		});

	stateMachine->AddState(goLeft);
	stateMachine->AddState(goRight);

	stateMachine->AddTransition(new StateTransition(goLeft, goRight, [&](void)->bool
		{
			return this->counter > 1.0f;
		}));
	stateMachine->AddTransition(new StateTransition(goRight, goLeft, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}
void StateGameObject::InitRotating()
{
	stateMachine = new StateMachine();

	State* antiClockwise = new State([&](float dt)->void
		{
			this->RotateAnticlockwise(dt);
		});
	State* clockwise = new State([&](float dt)->void
		{
			this->RotateClockwise(dt);
		});

	stateMachine->AddState(antiClockwise);
	stateMachine->AddState(clockwise);

	stateMachine->AddTransition(new StateTransition(antiClockwise, clockwise, [&](void)->bool
		{
			return this->counter > 5.0f;
		}));
	stateMachine->AddTransition(new StateTransition(clockwise, antiClockwise, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}
void StateGameObject::InitSpinning()
{
	stateMachine = new StateMachine();

	State* antiClockwise = new State([&](float dt)->void
		{
			this->SpinAnticlockwise(dt);
		});
	State* clockwise = new State([&](float dt)->void
		{
			this->SpinClockwise(dt);
		});
	State* collected = new State([&](float dt)->void
		{
			if (collisionWithPlayerBall == nullptr)
			{
				return;
			}
			collisionWithPlayerBall->GetPhysicsObject()->SetFriction(0.1f);
			this->SetActive(false);
		});
	State* inactive = new State([&](float dt)->void
		{
			this->Inactive();
			this->SetActive(false);
		});

	stateMachine->AddState(antiClockwise);
	stateMachine->AddState(clockwise);
	stateMachine->AddState(collected);
	stateMachine->AddState(inactive);

	stateMachine->AddTransition(new StateTransition(antiClockwise, clockwise, [&](void)->bool
		{
			return this->counter > 10.0f;
		}));
	stateMachine->AddTransition(new StateTransition(clockwise, antiClockwise, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
	stateMachine->AddTransition(new StateTransition(antiClockwise, collected, [&](void)->bool
		{
			return !(collisionWithPlayerBall == nullptr);
		}));
	stateMachine->AddTransition(new StateTransition(clockwise, collected, [&](void)->bool
		{
			return !(collisionWithPlayerBall == nullptr);
		}));
	stateMachine->AddTransition(new StateTransition(collected, inactive, [&](void)->bool
		{
			return (collisionWithPlayerBall == nullptr);
		}));
}
void StateGameObject::InitDestination()
{
	stateMachine = new StateMachine();

	State* inactive = new State([&](float dt)->void
		{
			this->Inactive();
		});
	State* reached = new State([&](float dt)->void
		{
			this->Reached();
		});

	stateMachine->AddState(inactive);
	stateMachine->AddState(reached);

	stateMachine->AddTransition(new StateTransition(inactive, reached, [&](void)->bool
		{
			return !(collisionWithPlayerBall == nullptr);
		}));
}
void StateGameObject::InitTeleporter()
{
	stateMachine = new StateMachine();

	State* inactive = new State([&](float dt)->void
		{
			this->Inactive();
		});
	State* reached = new State([&](float dt)->void
		{
			this->Reached();
		});

	stateMachine->AddState(inactive);
	stateMachine->AddState(reached);

	stateMachine->AddTransition(new StateTransition(inactive, reached, [&](void)->bool
		{
			return !(collisionWithPlayerBall == nullptr);
		}));
	stateMachine->AddTransition(new StateTransition(reached, inactive, [&](void)->bool
		{
			return (collisionWithPlayerBall == nullptr);
		}));
}

void StateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(GetPhysicsObject()->GetLinearVelocity() + Vector3(-1, 0, 0));
	this->SetState(States::MOVING_LEFT);
	counter += dt;
}
void StateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->SetLinearVelocity(GetPhysicsObject()->GetLinearVelocity() + Vector3(1, 0, 0));
	this->SetState(States::MOVING_RIGHT);
	counter -= dt;
}
void StateGameObject::RotateAnticlockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity(GetPhysicsObject()->GetAngularVelocity() + Vector3(0, 0.05f, 0));
	this->SetState(States::ROTATING_ANTICLOCKWISE);
	counter += dt;
}
void StateGameObject::RotateClockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity(GetPhysicsObject()->GetAngularVelocity() + Vector3(0, -0.05f, 0));
	this->SetState(States::ROTATING_CLOCKWISE);
	counter -= dt;
}
void StateGameObject::SpinAnticlockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,1,0 });
	this->SetState(States::SPINNING_ANTICLOCKWISE);
	counter += dt;
}
void StateGameObject::SpinClockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,-1,0 });
	this->SetState(States::SPINNING_CLOCKWISE);
	counter -= dt;
}
void StateGameObject::Inactive()
{
	this->SetState(States::INACTIVE);
}
void StateGameObject::Reached()
{
	this->SetState(States::REACHED);
}