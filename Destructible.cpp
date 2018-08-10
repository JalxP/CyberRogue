#include "stdafx.h"

Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
	maxHp(maxHp), hp(maxHp), defense(defense) {
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

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName) {
}

void MonsterDestructible::die(Actor *owner) {
	// transform it into a corpse, doesn't block, can't be atacked and doesn't move
	engine.gui->message(TCODColor::white, "%s is dead", owner->name);
	Destructible::die(owner);
}

void MonsterDestructible::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	Destructible::save(zip);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp, defense, corpseName) {
}

void PlayerDestructible::die(Actor *owner) {
	engine.gui->message(TCODColor::red, "You died!");
	Destructible::die(owner);
	engine.gameStatus = Engine::DEFEAT;
}

void PlayerDestructible::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	Destructible::save(zip);
}

void Destructible::save(TCODZip &zip) {
	zip.putFloat(maxHp);
	zip.putFloat(hp);
	zip.putFloat(defense);
	zip.putString(corpseName);
}

void Destructible::load(TCODZip &zip) {
	maxHp = zip.getFloat();
	hp = zip.getFloat();
	defense = zip.getFloat();
	corpseName = _strdup(zip.getString());
}

Destructible *Destructible::create(TCODZip &zip) {
	DestructibleType type = (DestructibleType)zip.getInt();
	Destructible *destructible = NULL;
	switch (type) {
		case Destructible::MONSTER:
			destructible = new MonsterDestructible(0, 0, NULL);
			break;
		case Destructible::PLAYER:
			destructible = new PlayerDestructible(0, 0, NULL);
			break;
		default: break;
	}
	destructible->load(zip);
	return destructible;
}
