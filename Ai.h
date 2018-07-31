class Ai{
public:
	//virtual ~Ai() {}; ?
	virtual void update(Actor *owner) = 0; // pure virtual method / abstract method
};

class PlayerAi : public Ai {
public:
	void update(Actor *owner);

protected:
	bool moveOrAttack(Actor *owner, int targetx, int targety);
	void handleActionKey(Actor *owner, int ascii);
	Actor *choseFromInventory(Actor *owner);
};

class MonsterAi : public Ai {
public:
	void update(Actor *owner);

protected:
	int moveCount;
	void moveOrAttack(Actor *owner, int targetx, int targety);
};