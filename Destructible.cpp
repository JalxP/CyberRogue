#include "stdafx.h"

Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
	maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName) {
}

float Destructible::takeDamage(Actor *owner, float damage) {
	damage -= defense;
	if (damage > 0) {
		hp -= damage;
		if (hp <= 0) {
			hp = 0; // to make sure hp is not negative
			die(owner);
		}
	}
	else {
		damage = 0;
	}
	return damage;
}

void Destructible::die(Actor *owner) {
	// transform the actor into a corpse
	owner->ch = '%';
	owner->col = TCODColor::darkestGreen;
	owner->name = corpseName;
	owner->blocks = false;
	// make sure the corpses are drawn before living actors
	engine.sendToBack(owner);
}

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName) {
}

void MonsterDestructible::die(Actor *owner) {
	// transform it into a corpse, doesn't block, can't be atacked and doesn't move
	engine.gui->message(TCODColor::white, "%s is dead", owner->name);
	Destructible::die(owner);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName) {
}

void PlayerDestructible::die(Actor *owner) {
	engine.gui->message(TCODColor::red, "You died!");
	Destructible::die(owner);
	engine.gameStatus = Engine::DEFEAT;
}