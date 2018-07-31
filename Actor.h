class Actor{
public:
	int x, y;                   // position on map
	int ch;                     // ascii code
	const char *name;           // actor's name
	TCODColor col;              // color
	bool blocks;                // can we walk through this actor?
	Attacker *attacker;         // something that deals damage
	Destructible *destructible; // something that can be damaged
	Ai *ai;                     // something self-updating
	Pickable *pickable;         // something that can be picked and used
	Container *container;       // something that can contain actors

	Actor(int x, int y, int ch, const char *name, const TCODColor &col);
	~Actor();
	void update();
	void render() const;
};