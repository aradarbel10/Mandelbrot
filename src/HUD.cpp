#include "HUD.h"

#include <iostream>
#include <algorithm>
#include <format>
#include <SFML/Graphics.hpp>

HUD::HUD() {
	if (!font.loadFromFile("resources/cour.ttf")) {
		std::cerr << "ERROR: failed to load font!\n";
	}

	text.setFont(font);
	text.setCharacterSize(default_size);
	text.setFillColor(sf::Color::Black);

	back.setFillColor(sf::Color(255, 255, 255, 190));
}

void HUD::setVisible(bool s) {
	visible = s;
}

bool HUD::isVisible() {
	return visible;
}

void HUD::setSize(int s) {
	text.setCharacterSize(std::clamp(s, min_size, max_size));
}

int HUD::getSize() {
	return text.getCharacterSize();
}

void HUD::setValues(float x, float y, float scale) {
	text.setString(std::format("pos: {}, {}\nscale: {}", x, y, scale));
}

void HUD::draw(sf::RenderWindow& window) {
	auto b = text.getGlobalBounds();
	back.setPosition(b.left, b.top);
	back.setSize({ b.width, b.height });
	window.draw(back);

	window.draw(text);
}