#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Mandelbrot.h"

#include <format>

int main() {
	sf::Vector2i win_size{600, 600};

	sf::RenderWindow window(sf::VideoMode(win_size.x, win_size.y), "The Mandelbrot Set", sf::Style::Close);
	sf::Event event;

	Mandelbrot myMandelbrot(win_size);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) window.close();
			}

			myMandelbrot.eventUpdate(event);
		}

		window.clear(sf::Color::White);

		myMandelbrot.show(window);

		window.display();
	}

	return 0;
}