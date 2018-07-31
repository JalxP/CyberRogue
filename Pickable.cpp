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