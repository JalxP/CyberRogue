#include "stdafx.h"

bool Pickable::pick(Actor *owner, Actor *wearer) {
	if (wearer->container && wearer->container->add(owner)) {
		engine.actors.remove(owner);
		return true;
	}
	return false;
}

bool Pickable::use(Actor *owner, Actor *wearer) {
	if (wearer->container) {
		wearer->container->remove(owner);
		delete owner;
		return true;
	}
	return false;
}

Healer::Healer(float amount) : amount(amount) {
}

bool Healer::use(Actor *owner, Actor *wearer) {
	if (wearer->destructible) {
		float amountHealed = wearer->destructible->heal(amount);
		if (amountHealed > 0) {
			engine.gui->message(TCODColor::darkChartreuse,
				"Healed %g HP.", amountHealed);
			return Pickable::use(owner, wearer);
		}
		else {
			engine.gui->message(TCODColor::lightAmber,
				"You are already at full health!");
		} 
	}
	return false;
}

Taser::Taser(float range, float damage) :
	range(range), damage(damage) {
}

bool Taser::use(Actor *owner, Actor *wearer) {
	Actor *closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);
	if (!closestMonster) {
		engine.gui->message(TCODColor::lightAmber,
			"No enemy is close enough to zap.");
		return false;
	}
	// hit the closest monster for <damage> hit points
	engine.gui->message(TCODColor::lighterSky,
		"A lightning zaps the %s with %g damage!", closestMonster->name,
		damage);
	closestMonster->destructible->takeDamage(closestMonster, damage);
	// check that the item is consumed on use
	return Pickable::use(owner, wearer);
}

Molotov::Molotov(float range, float damage) : Taser(range, damage) {
}

bool Molotov::use(Actor *owner, Actor *wearer) {
	engine.gui->message(TCODColor::lightSepia,
		"Left-click a target tile to use the molotov,\nor right-click to cancel.");
	int x, y;
	if (!engine.pickATile(&x, &y)) {
		engine.gui->message(TCODColor::white,
			"You put your cocktail molotov away.");
		return false;
	}
	// burn everything in <range> (including player)
	engine.gui->message(TCODColor::flame,
		"The molotov explodes, burning everything within %g tiles!", range);
	for (Actor **iterator = engine.actors.begin();
		iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->destructible && !actor->destructible->isDead() &&
			actor->getDistance(x, y) <= range) {
			engine.gui->message(TCODColor::flame,
				"The %s gets burned for %g hit points.",
				actor->name, damage);
			actor->destructible->takeDamage(actor, damage);
		}
	}
	// check that molotov is consumed on use
	return Pickable::use(owner, wearer);
}