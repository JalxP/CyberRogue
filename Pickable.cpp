#include "stdafx.h"

bool Pickable::pick(Actor *owner, Actor *wearer) {
	if (wearer->container && wearer->container->add(owner)) {
		engine.actors.remove(owner);
		return true;
	}
	return false;
}

void Pickable::drop(Actor *owner, Actor *wearer) {
	if (wearer->container) {
		wearer->container->remove(owner);
		engine.actors.push(owner);
		owner->x = wearer->x;
		owner->y = wearer->y;
		engine.gui->message(TCODColor::lightGrey,
			"%s drops a %s.", wearer->name, owner->name);
	}
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

void Healer::save(TCODZip &zip) {
	zip.putInt(HEALER);
	zip.putFloat(amount);
}

void Healer::load(TCODZip &zip) {
	amount = zip.getFloat();
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

void Taser::save(TCODZip &zip) {
	zip.putInt(TASER);
	zip.putFloat(range);
	zip.putFloat(damage);
}

void Taser::load(TCODZip &zip) {
	range = zip.getFloat();
	damage = zip.getFloat();
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

void Molotov::save(TCODZip &zip) {
	zip.putInt(MOLOTOV);
	zip.putFloat(range);
	zip.putFloat(damage);
}

EmpPulse::EmpPulse(int numberOfTurns, float range) :
	numberOfTurns(numberOfTurns), range(range) {
}

bool EmpPulse::use(Actor *owner, Actor *wearer) {
	engine.gui->message(TCODColor::darkCyan,
		"Left-click an enemy to disrupt it,\nor right-click to cancel.");
	int x, y;
	if (!engine.pickATile(&x, &y, range)) {
		engine.gui->message(TCODColor::white,
			"You safely discharge the emp pulse and put it away.");
		return false;
	}
	Actor *actor = engine.getActor(x, y);
	if (!actor) {
		engine.gui->message(TCODColor::lightAmber,
			"There's no enemy there to use an emp pulse on.");
		return false;
	}
	// confuse the monster for <numberOfTurns> turns
	Ai *confusedAi = new ConfusedMonsterAi(numberOfTurns, actor->ai);
	actor->ai = confusedAi;
	engine.gui->message(TCODColor::lighterCyan,
		"The %s emits a buzzing noise,\nas he starts to stumble around!",
		actor->name);
	// check that emp pulse is consumed on use
	return Pickable::use(owner, wearer);
}

void EmpPulse::save(TCODZip &zip) {
	zip.putInt(EMP_PULSE);
	zip.putInt(numberOfTurns);
	zip.putFloat(range);
}

void EmpPulse::load(TCODZip &zip) {
	numberOfTurns = zip.getInt();
	range = zip.getFloat();
}

Pickable *Pickable::create(TCODZip &zip) {
	PickableType type = (PickableType)zip.getInt();
	Pickable *pickable = NULL;
	switch (type) {
		case Pickable::HEALER:
			pickable = new Healer(0);
			break;
		case Pickable::TASER:
			pickable = new Taser(0, 0);
			break;
		case Pickable::MOLOTOV:
			pickable = new Molotov(0, 0);
			break;
		case Pickable::EMP_PULSE:
			pickable = new EmpPulse(0, 0);
			break;
		default: break;
	}
	pickable->load(zip);
	return pickable;
}