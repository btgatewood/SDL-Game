#pragma once


class Actor
{

};


class Component
{
public:
	Component(Actor* actor, int update_order = 100);
	virtual ~Component() {}
	virtual void Update(float) {}

protected:
	Actor* actor_;  // parent entity
	int update_order_;  // lower order = earlier update
};