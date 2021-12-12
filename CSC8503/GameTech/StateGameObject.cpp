#include "StateGameObject.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject(ObjectMovement movement, int layer, string name) : GameObject(layer, name)
{
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

void StateGameObject::InitMoving()
{
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* stateA = new State([&](float dt)->void
		{
			this->MoveLeft(dt);
		});
	State* stateB = new State([&](float dt)->void
		{
			this->MoveRight(dt);
		});

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&](void)->bool
		{
			return this->counter > 3.0f;
		}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}
void StateGameObject::InitRotating()
{
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* stateA = new State([&](float dt)->void
		{
			this->RotateAnticlockwise(dt);
		});
	State* stateB = new State([&](float dt)->void
		{
			this->RotateClockwise(dt);
		});

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&](void)->bool
		{
			return this->counter > 5.0f;
		}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}
void StateGameObject::InitSpinning()
{
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* stateA = new State([&](float dt)->void
		{
			this->SpinAnticlockwise(dt);
		});
	State* stateB = new State([&](float dt)->void
		{
			this->SpinClockwise(dt);
		});

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new StateTransition(stateA, stateB, [&](void)->bool
		{
			return this->counter > 10.0f;
		}));
	stateMachine->AddTransition(new StateTransition(stateB, stateA, [&](void)->bool
		{
			return this->counter < 0.0f;
		}));
}

void StateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->AddForce({ -50,0,0 });
	this->SetState(States::MOVING_LEFT);
	counter += dt;
}
void StateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->AddForce({ 50,0,0 });
	this->SetState(States::MOVING_RIGHT);
	counter -= dt;
}
void StateGameObject::RotateAnticlockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,4,0 });
	this->SetState(States::ROTATING_ANTICLOCKWISE);
	counter += dt;
}
void StateGameObject::RotateClockwise(float dt)
{
	GetPhysicsObject()->SetAngularVelocity({ 0,-4,0 });
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