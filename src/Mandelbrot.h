#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

//std::unique_ptr<sf::Font> Mandelbrot::font;

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
	sf::Vector2f panning_anchor{ -1, -1 }, panning_offset{ 0, 0 }; // used for mouse panning
	const float zoom_factor = 0.9f, scroll_speed = 0.02f; // movement consts

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

	// HUD
	sf::Font font;
	sf::Text coords_display;
	sf::RectangleShape text_back;
	bool show_hud = true;
	bool hud_size_changed = false;

	const int def_hud_size = 16, min_hud_size = 10, max_hud_size = 35;

	// Screenshots
	const std::string screenshot_dir = "screenshots";

};