class Destructible : public Persistent {
public:
	float maxHp;            // maximum health points
	float hp;               // current health points
	float defense;          // hit points deflected
	const char *corpseName; // the actor's name once dead/destroyed
	int xp;					// XP gained when killing this monster

	Destructible(float maxHp, float defense, const char *corpseName, int xp);
	virtual ~Destructible();
	inline bool isDead() { return hp <= 0; }
	float takeDamage(Actor *owner, float damage);
	float heal(float amount);
	virtual void die(Actor *owner); // virtual because it can be overloaded
	void save(TCODZip &zip);
	void load(TCODZip &zip);
	static Destructible *create(TCODZip &zip);
protected:
	enum DestructibleType {
		MONSTER, PLAYER
	};
};


class MonsterDestructible : public Destructible {
public:
	MonsterDestructible(float maxHp, float defense, const char *corpseName, int xp);
	void die(Actor *owner);
	void save(TCODZip & zip);
};

class PlayerDestructible : public Destructible {
public:
	PlayerDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
	void save(TCODZip & zip);
};