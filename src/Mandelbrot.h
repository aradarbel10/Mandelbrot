#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

class Mandelbrot {
public:

	Mandelbrot(sf::Vector2i size);

	Mandelbrot(const Mandelbrot&) = delete;
	Mandelbrot(const Mandelbrot&&) = delete;
	Mandelbrot& operator =(const Mandelbrot&) = delete;
	Mandelbrot& operator =(const Mandelbrot&&) = delete;

	void eventUpdate(const sf::Event& event);
	void show(sf::RenderWindow& window);

private:

	// Transformation
	sf::Vector2f panning_anchor{ -1, -1 }, panning_offset{ 0, 0 };
	float scale = 2, zoom_factor = 0.9f, scroll_speed = 0.02f;
	sf::Vector2f center{ -0.5, 0 };

	// Rendering
	int palette_steps = 50;
	sf::RenderTexture brot_texture;
	sf::Shader brot_shdr;
	sf::Vector2u view_size;

	// UI
	sf::Vector2f mouse;

	// Text
	std::unique_ptr<sf::Font> font;
	sf::Text coords_display;
	sf::RectangleShape text_back;

};