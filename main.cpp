// CyberRogue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

Engine engine(80, 50);

int main()
{
	while (!TCODConsole::isWindowClosed()) {
		engine.update();
		engine.render();
		TCODConsole::flush();
	}

    return 0;
}

