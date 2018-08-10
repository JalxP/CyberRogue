#include "stdafx.h"

Destructible::Destructible(float maxHp, float defense, const char *corpseName, int xp) :
	maxHp(maxHp), hp(maxHp), defense(defense), xp(xp) {
	this->corpseName = _strdup(corpseName);
}

Destructible::~Destructible() {
	free((char*)corpseName);
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

float Destructible::heal(float amount) {
	hp += amount;
	if (hp > maxHp) {
		amount -= hp - maxHp; // so it returns the amount healed
		hp = maxHp;
	}
	return amount;
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

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName, int xp) :
	Destructible(maxHp, defense, corpseName, xp) {
}

void MonsterDestructible::die(Actor *owner) {
	// transform it into a corpse, doesn't block, can't be atacked and doesn't move
	engine.gui->message(TCODColor::white, "%s is dead. You gain %d xp",
		owner->name, xp);
	engine.player->destructible->xp += xp;
	Destructible::die(owner);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName, 0) {
}

void PlayerDestructible::die(Actor *owner) {
	engine.gui->message(TCODColor::red, "You died!");
	Destructible::die(owner);
	engine.gameStatus = Engine::DEFEAT;
}
