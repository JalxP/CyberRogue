class Ai : public Persistent{
public:
	//virtual ~Ai() {}; ?
	virtual void update(Actor *owner) = 0; // pure virtual method / abstract method
	static Ai *create(TCODZip &zip);
protected:
	enum AiType {
		MONSTER, CONFUSED_MONSTER, PLAYER
	};
};

class PlayerAi : public Ai {
public:
	int xpLevel;
	PlayerAi();
	int getNextLevelXp();
	void update(Actor *owner);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
protected:
	bool moveOrAttack(Actor *owner, int targetx, int targety);
	void handleActionKey(Actor *owner, int ascii);
	Actor *choseFromInventory(Actor *owner);
};

class MonsterAi : public Ai {
public:
	void update(Actor *owner);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
protected:
	int moveCount;
	void moveOrAttack(Actor *owner, int targetx, int targety);
};

class ConfusedMonsterAi : public Ai {
public:
	ConfusedMonsterAi(int numberOfTurns, Ai *oldAi);
	void update(Actor *owner);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
protected:
	int numberOfTurns;
	Ai *oldAi;
};