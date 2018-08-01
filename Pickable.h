class Pickable {
public:
	// since the only actor that will pick up items will be the player
	// there is no need to make the pick function virtual
	bool pick(Actor *owner, Actor *wearer);
	virtual bool use(Actor *owner, Actor *wearer); // is dependent on the item
};

// for anything that heals
class Healer : public Pickable {
public:
	float amount; // how many hit points

	Healer(float amount);
	bool use(Actor *owner, Actor *wearer);
};

class Taser : public Pickable {
public:
	float range, damage;
	Taser(float range, float damage);
	bool use(Actor *owner, Actor *wearer);
};

// since molotoves require a range and a damage amount
// we can inherit from the Taser class
class Molotov : public Taser {
public:
	Molotov(float range, float damage);
	bool use(Actor *owner, Actor *wearer);
};