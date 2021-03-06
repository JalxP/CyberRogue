#include "stdafx.h"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP),
	player(NULL), map(NULL), fovRadius(10),
	screenWidth(screenWidth), screenHeight(screenHeight), level(1) {
	TCODConsole::initRoot(screenWidth, screenHeight, "CyberRogue 0.1", false);
	gui = new Gui();
}

Engine::~Engine() {
	term();
	delete gui;
}

Actor *Engine::getClosestMonster(int x, int y, float range) const {
	Actor *closest = NULL;
	float bestDistance = 1E6f;

	for (Actor **iterator = actors.begin(); iterator != actors.end();
		iterator++) {
		Actor *actor = *iterator;
		if (actor != player && actor->destructible &&
			!actor->destructible->isDead()) {
			float distance = actor->getDistance(x, y);
			if (distance < bestDistance &&
				(distance <= range || range == 0.0f)) {
				bestDistance = distance;
				closest = actor;
			}
		}
	}
	return closest;
}

Actor *Engine::getActor(int x, int y) const {
	for (Actor **iterator = actors.begin(); iterator != actors.end();
		iterator++) {
		Actor *actor = *iterator;
		if (actor->x == x && actor->y == y && actor->destructible &&
			!actor->destructible->isDead()) {
			return actor;
		}
	}
	return NULL;
}

bool Engine::pickATile(int *x, int *y, float maxRange) {
	while (!TCODConsole::isWindowClosed()) {
		render();

		// Highlight tiles in range
		for (int cx = 0; cx < map->width; cx++) {
			for (int cy = 0; cy < map->height; cy++) {
				if (map->isInFOV(cx, cy) &&
					(maxRange == 0 || player->getDistance(cx, cy) <= maxRange)) {
					TCODColor col = TCODConsole::root->getCharBackground(cx, cy);
					col = col * 1.5f;
					TCODConsole::root->setCharBackground(cx, cy, col);
				}
			}
		}
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);
		// make tile under mouse a different color
		// and get coordinates where player clicks
		if (map->isInFOV(mouse.cx, mouse.cy) &&
			(maxRange == 0 || player->getDistance(mouse.cx, mouse.cy) <= maxRange)) {
			TCODConsole::root->setCharBackground(mouse.cx, mouse.cy,
				TCODColor::white);
			if (mouse.lbutton_pressed) {
				*x = mouse.cx;
				*y = mouse.cy;
				return true;
			}
		}
		// if the player the right click we cancel selection
		if (mouse.rbutton_pressed) {
			return false;
		}
		TCODConsole::flush();
	}
	return false;
}

void Engine::init() {
	player = new Actor(40, 25, '@', "player", TCODColor::white);
	player->destructible = new PlayerDestructible(30, 2, "your cadaver");
	player->attacker = new Attacker(5);
	player->ai = new PlayerAi();
	player->container = new Container(26); // one slot each letter (ez access)
	actors.push(player);
	stairs = new Actor(0, 0, '>', "stairs", TCODColor::lighterSepia);
	stairs->blocks = false;
	stairs->fovOnly = false;
	actors.push(stairs);
	map = new Map(80, 43);
	map->init(true);
	gui->message(TCODColor::darkerCrimson,
		"Welcome stranger!\nPrepare to perish in the Proxy outskirts.");
	gameStatus = STARTUP;
}

void Engine::term() {
	actors.clearAndDelete();
	if (map) delete map;
	gui->clear();
}

void Engine::update() {
	if (gameStatus == STARTUP) map->computeFov();
	gameStatus = IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE, &lastKey, &mouse);
	if (lastKey.vk == TCODK_ESCAPE) {
		save();
		load();
	}
	player->update();
	if (gameStatus == NEW_TURN) {
		for (Actor **iterator = actors.begin(); iterator != actors.end();
			iterator++) {
			Actor *actor = *iterator;
			if (actor != player) {
				actor->update();
			}
		}
	}
}

void Engine::sendToBack(Actor *actor) {
	actors.remove(actor);
	actors.insertBefore(actor, 0);
}

void Engine::nextLevel() {
	level++;

	gui->message(TCODColor::darkerPink,
		"You take a moment to rest, and recover your vitality.");
	player->destructible->heal(player->destructible->maxHp / 2);
	gui->message(TCODColor::lighterSky, "After a rare moment of peace, you descend\ndeeper into the slums of the Proxy...");

	delete map;
	// delete all actors but the player and stairs
	for (Actor **iterator = actors.begin();
		iterator != actors.end(); iterator++) {
		if (*iterator != player && *iterator != stairs) {
			delete *iterator;
			iterator = actors.remove(iterator); // some list voodoo here
		}
	}
	// create a new map
	map = new Map(80, 43);
	map->init(true);
	gameStatus = STARTUP;
}

void Engine::render() {
	TCODConsole::root->clear();
	// draw the map
	map->render();

	// draw the actors
	for (Actor **iterator = actors.begin();
		iterator != actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor != player &&
			((!actor->fovOnly && map->isExplored(actor->x, actor->y)) ||
				map->isInFOV(actor->x, actor->y))) {
			actor->render();
		}
	}
	player->render();

	// draw the gui with player stats and log
	gui->render();
}