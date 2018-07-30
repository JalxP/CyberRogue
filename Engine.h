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
	TCOD_key_t lastKey;

	TCODList<Actor *> actors;
	Actor *player;
	Map * map;
	int fovRadius;

	Engine(int screenWidth, int screenHeight);
	~Engine();
	void update();
	void sendToBack(Actor *actor);
	void render();

private:
	bool computeFov;
};

extern Engine engine;