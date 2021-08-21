#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

#include "HUD.h"

class Mandelbrot {
public:

	Mandelbrot(sf::Vector2i size);

	//remove all move & copy semantics, because I don't think it makes sense to move and copy this object.
	Mandelbrot(const Mandelbrot&) = delete;
	Mandelbrot(const Mandelbrot&&) = delete;
	Mandelbrot& operator =(const Mandelbrot&) = delete;
	Mandelbrot& operator =(const Mandelbrot&&) = delete;

	void eventUpdate(const sf::Event& event);
	void show(sf::RenderWindow& window);

	std::string calc_screenshot_name(int i);

private:

	// Transformations
	sf::Vector2f panning_anchor{ -1, -1 }, panning_offset{ 0, 0 };
	const float zoom_factor = 0.9f, scroll_speed = 0.02f;

	float scale = 2; // current transform -- scale and position
	sf::Vector2f center{ -0.5, 0 };

	// Rendering
	int palette_steps = 50;
	sf::RenderTexture brot_texture;
	sf::Shader brot_shdr;
	sf::Vector2u view_size;

	// User Input
	sf::Vector2f mouse;
	bool take_screenshot = false;
	bool hud_size_changed = false;

	HUD hud;

	// Screenshots
	const std::string screenshot_dir = "screenshots";

};