#pragma once

#include <SFML/Graphics.hpp>

class HUD {
public:

	HUD();

	void setVisible(bool s);
	bool isVisible();

	void setSize(int s);
	int getSize();

	void setValues(float x, float y, float scale);
	void draw(sf::RenderWindow& window);

private:

	sf::Font font;
	sf::Text text;
	sf::RectangleShape back;

	bool visible = true;

	int default_size = 16;
	int min_size = 10;
	int max_size = 35;

};