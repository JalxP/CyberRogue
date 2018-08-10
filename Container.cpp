#include "stdafx.h"

Container::Container(int size) : size(size) {
}

Container::~Container() {
	inventory.clearAndDelete();
}

bool Container::add(Actor *actor) {
	if (size > 0 && inventory.size() >= size) {
		// the inventory is full
		return false;
	}
	inventory.push(actor);
	return true;
}

void Container::remove(Actor *actor) {
	inventory.remove(actor);
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