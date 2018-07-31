#include "stdafx.h"

Attacker::Attacker(float power) : power(power) {
}

void Attacker::attack(Actor *owner, Actor *target) {

	float damage = power - target->destructible->defense;
	if (target->destructible && !target->destructible->isDead()) {
		if (damage > 0) {
			engine.gui->message(owner == engine.player ? TCODColor::red : TCODColor::white,
				"%s attacks %s for %g hit points.\n", owner->name, target->name, damage);
		}
		else {
			engine.gui->message(TCODColor::white,
				"%s attacks %s but it has no effect!\n", owner->name, target->name);
		}
		target->destructible->takeDamage(target, power);
	}
	else {
		engine.gui->message(TCODColor::white,
			"%s attacks %s in vain.\n", owner->name, target->name);
	}
}