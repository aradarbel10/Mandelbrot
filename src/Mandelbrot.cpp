#include "Mandelbrot.h"

#include <iostream>
#include <format>
#include <filesystem>

Mandelbrot::Mandelbrot(sf::Vector2i size) {
	if (!sf::Shader::isAvailable()) std::cerr << "ERROR: system does not support shaders!\n";
	if (!brot_texture.create(size.x, size.y)) std::cerr << "ERROR: failed to create render texture!\n";

	view_size = static_cast<sf::Vector2u>(size);

	brot_shdr.loadFromFile("resources/Mandelbrot.glsl", sf::Shader::Fragment);
	brot_shdr.setUniform("view_size", static_cast<sf::Vector2f>(size));
}

void Mandelbrot::eventUpdate(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
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
				hud.setVisible(!hud.isVisible());
			}
			hud_size_changed = false;
		}
	}

	if (event.type == sf::Event::MouseWheelMoved) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			palette_steps += event.mouseWheel.delta;

			if (palette_steps < 1) palette_steps = 1;

			brot_shdr.setUniform("palette_steps", palette_steps);

		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
			hud_size_changed = true;
			if (hud.isVisible()) { // resize HUD ont if it's active
				int curr_size = hud.getSize();
				curr_size += event.mouseWheel.delta;

				hud.setSize(curr_size);
			}
		} else {
			float fctr = event.mouseWheel.delta > 0 ? zoom_factor : 1.f / zoom_factor;

			scale *= fctr;
			// moving is necessary to zoom around the mouse position
			center += sf::Vector2f{ mouse.x / view_size.x - 0.5f, 0.5f - mouse.y / view_size.y } * (1.f / fctr - 1.f) * scale;
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
			panning_offset = {
				(mouse.x - panning_anchor.x) / view_size.x * scale,
				(panning_anchor.y - mouse.y) / view_size.y * scale
			};
		}
	}

	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			center -= panning_offset;
			panning_offset = { 0, 0 };
		}
	}
}

std::filesystem::path Mandelbrot::calc_screenshot_name(int i) {
	auto suffix = i == 0 ? "" : std::format(" ({})", i);
	return screenshot_dir / std::format("screenshot{}.png", suffix);
}

void Mandelbrot::show(sf::RenderWindow& window) {
	brot_shdr.setUniform("center", center - panning_offset);
	brot_shdr.setUniform("scale", scale);

	sf::Sprite spr{ brot_texture.getTexture() };
	window.draw(spr, &brot_shdr);

	if (hud.isVisible()) {
		hud.setValues(center.x - panning_offset.x, center.y - panning_offset.y, scale);
		hud.draw(window);
	}

	if (take_screenshot) {
		if (!std::filesystem::exists(screenshot_dir)) {
			std::filesystem::create_directory(screenshot_dir);
		}

		int screenshot_index = 0;
		std::filesystem::path result_name;

		do {
			result_name = calc_screenshot_name(screenshot_index);
			screenshot_index++;
		} while (std::filesystem::exists(result_name));

		sf::Texture t;
		t.create(view_size.x, view_size.y);
		t.update(window);

		if (t.copyToImage().saveToFile( result_name.string() )) {
			std::cout << "Screenshot saved successfully! check screenshots folder.\n";
		} else {
			std::cout << "Failed to save screenshot...\n";
		}

		take_screenshot = false;
	}
}