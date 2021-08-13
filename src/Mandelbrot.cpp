#include "Mandelbrot.h"

#include <iostream>
#include <format>
#include <filesystem>

//std::unique_ptr<sf::Font> Mandelbrot::font;

Mandelbrot::Mandelbrot(sf::Vector2i size) {
	// check for fatal errors
	if (!sf::Shader::isAvailable()) std::cerr << "ERROR: system does not support shaders!\n";
	if (!brot_texture.create(size.x, size.y)) std::cerr << "ERROR: failed to create render texture!\n";

	view_size = (sf::Vector2u)size;

	// set up shader
	brot_shdr.loadFromFile("src/Mandelbrot.glsl", sf::Shader::Fragment);
	brot_shdr.setUniform("view_size", (sf::Vector2f)size);

	// handle font & text
	if (!font.loadFromFile("resources/cour.ttf")) {
		std::cerr << "ERROR: failed to load font!\n";
	}

	coords_display.setFont(font);
	coords_display.setCharacterSize(def_hud_size);
	coords_display.setFillColor(sf::Color::Black);

	text_back.setFillColor(sf::Color(255, 255, 255, 190));
}

void Mandelbrot::eventUpdate(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		// keyboard movement
		case sf::Keyboard::Right:
			center.x += scale * scroll_speed;
			break;
		case sf::Keyboard::Left:
			center.x -= scale * scroll_speed;
			break;
		case sf::Keyboard::Up:
			center.y += scale * scroll_speed;
			break;
		case sf::Keyboard::Down:
			center.y -= scale * scroll_speed;
			break;

		// take screenshot next time rendering
		case sf::Keyboard::S:
			take_screenshot = true;
			break;
		}
	}

	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
		case sf::Keyboard::H:
			if (!hud_size_changed) {
				show_hud ^= true;
			}
			hud_size_changed = false;
		}
	}

	if (event.type == sf::Event::MouseWheelMoved) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { // scrolling with C alters color palette
			palette_steps += event.mouseWheel.delta;

			if (palette_steps < 1) palette_steps = 1;

			brot_shdr.setUniform("palette_steps", palette_steps);

		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) { // scrolling with H resizes HUD
			hud_size_changed = true;
			if (show_hud) { // resize HUD ont if it's active
				int curr_size = coords_display.getCharacterSize();
				curr_size += event.mouseWheel.delta;

				coords_display.setCharacterSize(std::clamp(curr_size, min_hud_size, max_hud_size));
			}
		} else {
			float fctr = event.mouseWheel.delta > 0 ? zoom_factor : 1.f / zoom_factor;

			scale *= fctr;
			// moving is necessary to zoom around the mouse position
			center += sf::Vector2f(mouse.x / view_size.x - 0.5f, 0.5f - mouse.y / view_size.y) * (1.f / fctr - 1.f) * scale;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			panning_anchor = mouse;
		}
	}

	if (event.type == sf::Event::MouseMoved) {
		mouse = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			panning_offset = sf::Vector2f(
				(mouse.x - panning_anchor.x) / view_size.x * scale,
				(panning_anchor.y - mouse.y) / view_size.y * scale
			);
		}
	}

	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			center -= panning_offset;
			panning_offset = { 0, 0 };
		}
	}
}

// helper functions for saving screenshots
std::string calc_screenshot_name(int i) {
	return screenshot_dir + std::format("/screenshot{}.png", i == 0 ? "" : ("(" + std::to_string(i) + ")"));
}

void Mandelbrot::show(sf::RenderWindow& window) {
	brot_shdr.setUniform("center", center - panning_offset);
	brot_shdr.setUniform("scale", scale);

	sf::Sprite spr(brot_texture.getTexture());
	window.draw(spr, &brot_shdr);

	if (show_hud) {
		coords_display.setString(std::format("pos: {}, {}\nscale: {}", center.x - panning_offset.x, center.y - panning_offset.y, scale));

		auto b = coords_display.getGlobalBounds();
		text_back.setPosition(b.left, b.top);
		text_back.setSize({ b.width, b.height });
		window.draw(text_back);

		window.draw(coords_display);
	}

	if (take_screenshot) {
		if (!std::filesystem::exists(screenshot_dir)) { // create screenshots folder if it doesn't already exist
			std::filesystem::create_directory(screenshot_dir);
		}

		int screenshot_index = 0;
		std::string result_name;
		while (std::filesystem::exists(result_name = calc_screenshot_name(screenshot_index))) screenshot_index++; // rename new screenshot so existing ones don't get overwritten

		sf::Texture t;
		t.create(view_size.x, view_size.y);
		t.update(window);

		if (t.copyToImage().saveToFile(std::format(result_name))) {
			std::cout << "Screenshot saved successfully! check screenshots folder.\n";
		} else {
			std::cout << "Failed to save screenshot...\n";
		}

		take_screenshot = false;
	}
}