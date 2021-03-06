// CyberRogue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

Engine engine(80, 50);

int main()
{
	engine.load();
	while (!TCODConsole::isWindowClosed()) {
		engine.update();
		engine.render();
		TCODConsole::flush();
	}
	engine.save();
    return 0;
}

