class Engine {
public:
	enum GameStatus {
		STARTUP,  // 0 first frame of the game
		IDLE,     // 1 no new turn. Redraw the same screen
		NEW_TURN, // 2 update the monsters position
		VICTORY,  // 3 the player won
		DEFEAT    // 4 the player was killed
	} gameStatus;

	int screenWidth;
	int screenHeight;
	Gui *gui;
	TCOD_key_t lastKey;
	TCOD_mouse_t mouse;
	TCODList<Actor *> actors;
	Actor *player;
	Actor *stairs;
	Map * map;
	int fovRadius;
	int level;

	Engine(int screenWidth, int screenHeight);
	~Engine();
	Actor *getClosestMonster(int x, int y, float range) const;
	Actor *getActor(int x, int y) const;
	bool pickATile(int*x, int *y, float maxRange = 0.0f);
	void init();
	void term();
	void load(bool pause = false);
	void save();
	void update();
	void sendToBack(Actor *actor);
	void nextLevel();
	void render();

private:
	bool computeFov;
};

extern Engine engine;