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