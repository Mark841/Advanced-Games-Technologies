#include "StateGameObject.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject(int layer, string name) : GameObject(layer, name)
{
	//InitMoving();
	InitRotating();
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
	
}

void StateGameObject::MoveLeft(float dt)
{
	GetPhysicsObject()->AddForce({ -50,0,0 });
	counter += dt;
}
void StateGameObject::MoveRight(float dt)
{
	GetPhysicsObject()->AddForce({ 50,0,0 });
	counter -= dt;
}
void StateGameObject::RotateLeft(float dt)
{
	GetPhysicsObject()->ApplyAngularImpulse({ 0,1,0 });
}
void StateGameObject::RotateRight(float dt)
{
	GetPhysicsObject()->ApplyAngularImpulse({ 0,-1,0 });
}