#include "menu.h"
#include <iostream>
#include "beatmap.h"
bool Main_menu(sf::RenderWindow & window) {
	sf::Texture menuTexture1, menuTexture2, menuTexture3, menuTexture4, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/menu/new_game.png");
	menuTexture2.loadFromFile("images/menu/properties.png");
	menuTexture3.loadFromFile("images/menu/about.png");
	menuTexture4.loadFromFile("images/menu/exit.png");
	menuBackground.loadFromFile("images/start.jpg");
	sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), menu4(menuTexture4), about(aboutTexture), menuBg(menuBackground);
	int menuNum = 0;
	menu1.setPosition(260, 400);
	menu2.setPosition(295, 450);
	menu3.setPosition(330, 500);
	menu4.setPosition(365, 550);
	menuBg.setPosition(0, 0);
	sf::Event event;
	List osu;

	while (1)
	{
		window.setMouseCursorVisible(true);
		window.pollEvent(event);
		if (event.type == sf::Event::Closed) {
			window.close();
			return 0;
		}
			
		menu1.setColor(sf::Color::White);
		menu2.setColor(sf::Color::White);
		menu3.setColor(sf::Color::White);
		menu4.setColor(sf::Color::White);
		menuNum = 0;
		
		sf::Vector2f r_size(window.getSize());
		r_size.y /= 1920;
		r_size.y /= 1080;

		auto temp = sf::Mouse::getPosition(window);
		if ( temp.y < (int)( (41 * temp.x + 12) / 26) &&  temp.y >(int)( (41 * temp.x - 12790) / 26)
			&&  temp.y > (int)( (-15 * temp.x + 12587) / 22) &&  temp.y < (int)( (-15 * temp.x + 13924) / 22)) {
			menu1.setColor({ 244, 162, 185, 255 }); 
			menuNum = 1;
		}
		if ( temp.y < (int)( (41 * temp.x + 12) / 26) &&  temp.y >(int)( (41 * temp.x - 12400) / 26)
			&&  temp.y > (int)( (-15 * temp.x + 14300) / 22) &&  temp.y < (int)( (-15 * temp.x + 15500) / 22)) {
			menu2.setColor({ 244, 162, 185, 255 }); 
			menuNum = 2; 
		}
		if ( temp.y < (int)( 41 * temp.x / 26) &&  temp.y >(int)( (41 * temp.x - 9400) / 26)
			&&  temp.y > (int)( (-15 * temp.x + 15900) / 22) &&  temp.y < (int)( (-15 * temp.x + 17200) / 22)) {
			menu3.setColor({ 244, 162, 185, 255 }); 
			menuNum = 3; 
		}
		if ( temp.y < (int)( 41 * temp.x / 26) &&  temp.y >(int)( (41 * temp.x - 8000) / 26)
			&&  temp.y > (int)( (-15 * temp.x + 17600) / 22) &&  temp.y < (int)( (-15 * temp.x + 18900) / 22)) {
			menu4.setColor({ 244, 162, 185, 255 }); 
			menuNum = 4; 
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (menuNum == 1) { Beatmap(osu.songlist[1], window); }
			else if (menuNum == 2) {/* window.draw(about); window.display(); while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));*/ }
			else if (menuNum == 3) { /*window.close(); isMenu = false; exit(0)*/ }
			else if (menuNum == 4) { return 0; }

		}
		window.clear();
		menu1.setRotation(-36);
		menu2.setRotation(-36);
		menu3.setRotation(-36);
		menu4.setRotation(-36);
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(menu4);

		window.display();
	}
}

/*void Songpage(List& list) {
	if (list.songlist.size() < 1) {
		std::cout << "Wrong list" << std::endl;
		return;
	}
	int selected = 0;
}*/