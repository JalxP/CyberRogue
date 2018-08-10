#include "stdafx.h"

const int SAVEGAME_VERSION = 0x1100;

void Map::save(TCODZip &zip) {
	zip.putInt(seed);
	// save as int because TCODZip cant save bools
	for (int i = 0; i < width*height; i++) {
		zip.putInt(tiles[i].explored);
	}
}

void Map::load(TCODZip &zip) {
	seed = zip.getInt();
	init(false); // create map without actors

	for (int i = 0; i < width*height; i++) {
		tiles[i].explored = zip.getInt();
	}
}

void Actor::save(TCODZip &zip) {
	zip.putInt(x);
	zip.putInt(y);
	zip.putInt(ch);
	zip.putColor(&col);
	zip.putString(name);
	zip.putInt(blocks);
	zip.putInt(fovOnly);
	// save a boolean for each feature
	zip.putInt(attacker != NULL);
	zip.putInt(destructible != NULL);
	zip.putInt(ai != NULL);
	zip.putInt(pickable != NULL);
	zip.putInt(container != NULL);
	// save the features themselves
	if (attacker) attacker->save(zip);
	if (destructible) destructible->save(zip);
	if (ai) ai->save(zip);
	if (pickable) pickable->save(zip);
	if (container) container->save(zip);
}

void Actor::load(TCODZip &zip) {
	x = zip.getInt();
	y = zip.getInt();
	ch = zip.getInt();
	col = zip.getColor();
	name = _strdup(zip.getString());
	blocks = zip.getInt();
	fovOnly = zip.getInt();
	// load the features booleans
	bool hasAttacker = zip.getInt();
	bool hasDestructible = zip.getInt();
	bool hasAi = zip.getInt();
	bool hasPickable = zip.getInt();
	bool hasContainer = zip.getInt();
	// load the features themselves
	if (hasAttacker) {
		attacker = new Attacker(0.0f);
		attacker->load(zip);
	}
	if (hasDestructible) {
		destructible = Destructible::create(zip);
	}
	if (hasAi) {
		ai = Ai::create(zip);
	}
	if (hasPickable) {
		pickable = Pickable::create(zip);
	}
	if (hasContainer) {
		container = new Container(0);
		container->load(zip);
	}
}

void Container::save(TCODZip &zip) {
	zip.putInt(size);
	zip.putInt(inventory.size());
	for (Actor **iterator = inventory.begin();
		iterator != inventory.end(); iterator++) {
		(*iterator)->save(zip);
	}
}

void Container::load(TCODZip &zip) {
	size = zip.getInt();
	int numberOfActors = zip.getInt();
	while (numberOfActors > 0) {
		Actor *actor = new Actor(0, 0, 0, NULL, TCODColor::white);
		actor->load(zip);
		inventory.push(actor);
		numberOfActors--;
	}
}

void Destructible::save(TCODZip &zip) {
	zip.putFloat(maxHp);
	zip.putFloat(hp);
	zip.putFloat(defense);
	zip.putString(corpseName);
	zip.putInt(xp);
}

void Destructible::load(TCODZip &zip) {
	maxHp = zip.getFloat();
	hp = zip.getFloat();
	defense = zip.getFloat();
	corpseName = _strdup(zip.getString());
	xp = zip.getInt();
}

void PlayerDestructible::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	Destructible::save(zip);
}

void MonsterDestructible::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	Destructible::save(zip);
}

Destructible *Destructible::create(TCODZip &zip) {
	DestructibleType type = (DestructibleType)zip.getInt();
	Destructible *destructible = NULL;
	switch (type) {
	case MONSTER:
		destructible = new MonsterDestructible(0, 0, NULL, 0);
		break;
	case PLAYER:
		destructible = new PlayerDestructible(0, 0, NULL);
		break;
	default: break;
	}
	destructible->load(zip);
	return destructible;
}

void Attacker::save(TCODZip &zip) {
	zip.putFloat(power);
}

void Attacker::load(TCODZip &zip) {
	power = zip.getFloat();
}

void MonsterAi::save(TCODZip &zip) {
	zip.putInt(MONSTER);
	zip.putInt(moveCount);
}

void MonsterAi::load(TCODZip &zip) {
	moveCount = zip.getInt();
}

void ConfusedMonsterAi::save(TCODZip &zip) {
	zip.putInt(CONFUSED_MONSTER);
	zip.putInt(numberOfTurns);
	oldAi->save(zip);
}

void ConfusedMonsterAi::load(TCODZip &zip) {
	numberOfTurns = zip.getInt();
	oldAi = Ai::create(zip);
}

void PlayerAi::save(TCODZip &zip) {
	zip.putInt(PLAYER);
	zip.putInt(xpLevel);
}

void PlayerAi::load(TCODZip &zip) {
	xpLevel = zip.getInt();
}

Ai *Ai::create(TCODZip &zip) {
	AiType type = (AiType)zip.getInt();
	Ai *ai = NULL;
	switch (type) {
	case MONSTER:
		ai = new MonsterAi();
		break;
	case CONFUSED_MONSTER:
		ai = new ConfusedMonsterAi(0, NULL);
		break;
	case PLAYER:
		ai = new PlayerAi();
		break;
	default: break;
	}
	ai->load(zip);
	return ai;
}

void Healer::save(TCODZip &zip) {
	zip.putInt(HEALER);
	zip.putFloat(amount);
}

void Healer::load(TCODZip &zip) {
	amount = zip.getFloat();
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

void EmpPulse::save(TCODZip &zip) {
	zip.putInt(EMP_PULSE);
	zip.putInt(numberOfTurns);
	zip.putFloat(range);
}

void EmpPulse::load(TCODZip &zip) {
	numberOfTurns = zip.getInt();
	range = zip.getFloat();
}

void Molotov::save(TCODZip &zip) {
	zip.putInt(MOLOTOV);
	zip.putFloat(range);
	zip.putFloat(damage);
}

Pickable *Pickable::create(TCODZip &zip) {
	PickableType type = (PickableType)zip.getInt();
	Pickable *pickable = NULL;
	switch (type) {
	case HEALER:
		pickable = new Healer(0);
		break;
	case TASER:
		pickable = new Taser(0, 0);
		break;
	case MOLOTOV:
		pickable = new Molotov(0, 0);
		break;
	case EMP_PULSE:
		pickable = new EmpPulse(0, 0);
		break;
	default: break;
	}
	pickable->load(zip);
	return pickable;
}

void Gui::save(TCODZip &zip) {
	zip.putInt(log.size());
	for (Message **iterator = log.begin();
		iterator != log.end(); iterator++) {
		zip.putString((*iterator)->text);
		zip.putColor(&(*iterator)->col);
	}
}

void Gui::load(TCODZip &zip) {
	int numberOfMessages = zip.getInt();
	while (numberOfMessages > 0) {
		const char *text = zip.getString();
		TCODColor col = zip.getColor();
		message(col, text);
		numberOfMessages--;
	}
}

void Engine::save() {
	if (player->destructible->isDead()) {
		TCODSystem::deleteFile("game.sav");
	}
	else {
		TCODZip zip;
		zip.putInt(SAVEGAME_VERSION);
		zip.putInt(level);
		// save the map first
		zip.putInt(map->width);
		zip.putInt(map->height);
		map->save(zip);
		// save the player
		player->save(zip);
		// save the stairs
		stairs->save(zip);
		// save the other actors
		zip.putInt(actors.size() - 2); // player + stairs
		for (Actor **iterator = actors.begin();
			iterator != actors.end(); iterator++) {
			if (*iterator != player && *iterator != stairs) {
				(*iterator)->save(zip);
			}
		}
		// save the message log
		gui->save(zip);
		zip.saveToFile("game.sav");
	}
}

void Engine::load(bool pause) {
	TCODZip zip;
	engine.gui->menu.clear();
	engine.gui->menu.addItem(Menu::NEW_GAME, "New game");
	if (TCODSystem::fileExists("game.sav")) {
		zip.loadFromFile("game.sav");
		int version = zip.getInt();
		if (version == SAVEGAME_VERSION) {
			engine.gui->menu.addItem(Menu::CONTINUE, "Continue");
		}
	}

	engine.gui->menu.addItem(Menu::EXIT, "Exit");

	Menu::MenuItemCode menuItem = engine.gui->menu.pick(
		pause ? Menu::PAUSE : Menu::MAIN);
	if (menuItem == Menu::EXIT || menuItem == Menu::NONE) {
		// exit or window closed
		exit(0);
	}
	else if (menuItem == Menu::NEW_GAME) {
		// new game
		engine.term();
		engine.init();
	}
	else {
		// continue a saved game
		engine.term();
		// load the map
		level = zip.getInt();
		int width = zip.getInt();
		int height = zip.getInt();
		map = new Map(width, height);
		map->load(zip);
		// load the player
		player = new Actor(0, 0, 0, NULL, TCODColor::white);
		player->load(zip);
		actors.push(player);
		// load the stairs
		stairs = new Actor(0, 0, 0, NULL, TCODColor::lighterSepia);
		stairs->load(zip);
		actors.push(stairs);
		// load the other actors
		int numberOfActors = zip.getInt();
		while (numberOfActors > 0) {
			Actor *actor = new Actor(0, 0, 0, NULL, TCODColor::white);
			actor->load(zip);
			actors.push(actor);
			numberOfActors--;
		}
		// load the message log
		gui->load(zip);
		gameStatus = STARTUP;
	}
}