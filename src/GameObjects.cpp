#include "GameObjects.h"


Component::Component(Actor* actor, int update_order)
	: actor_(actor)
	, update_order_(update_order)
{

}