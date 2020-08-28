#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "menu.h"
#include "Song.h"
using namespace sf;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(sf::VideoMode(1920, 1080), "Demo osu)", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	if (!Main_menu(window)) {
		window.close();
	}
	return 0;
}