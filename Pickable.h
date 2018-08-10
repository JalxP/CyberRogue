class Pickable : public Persistent{
public:
	// since the only actor that will pick up items will be the player
	// there is no need to make the pick function virtual
	bool pick(Actor *owner, Actor *wearer);
	void drop(Actor *owner, Actor *wearer);
	virtual bool use(Actor *owner, Actor *wearer); // is dependent on the item
	static Pickable *create(TCODZip &zip);
protected:
	enum PickableType {
		HEALER, TASER, MOLOTOV, EMP_PULSE
	};
};

// for anything that heals
class Healer : public Pickable {
public:
	float amount; // how many hit points

	Healer(float amount);
	bool use(Actor *owner, Actor *wearer);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};

class Taser : public Pickable {
public:
	float range, damage;
	Taser(float range, float damage);
	bool use(Actor *owner, Actor *wearer);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};

// since molotoves require a range and a damage amount
// we can inherit from the Taser class
class Molotov : public Taser {
public:
	Molotov(float range, float damage);
	bool use(Actor *owner, Actor *wearer);
	void save(TCODZip &zip);
};

class EmpPulse : public Pickable {
public:
	int numberOfTurns;
	float range;
	EmpPulse(int numberOfTurns, float range);
	bool use(Actor *owner, Actor *wearer);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};