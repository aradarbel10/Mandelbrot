#include "Mandelbrot.h"

#include <iostream>
#include <format>

//std::unique_ptr<sf::Font> Mandelbrot::font;

Mandelbrot::Mandelbrot(sf::Vector2i size) {
	if (!sf::Shader::isAvailable()) std::cerr << "ERROR: system does not support shaders!\n";
	if (!brot_texture.create(size.x, size.y)) std::cerr << "ERROR: failed to create render texture!\n";

	view_size = (sf::Vector2u)size;

	brot_shdr.loadFromFile("src/Mandelbrot.glsl", sf::Shader::Fragment);
	brot_shdr.setUniform("view_size", (sf::Vector2f)size);
	brot_shdr.setUniform("palette_steps", palette_steps);

	// handle font
	if (!font) {
		font = std::make_unique<sf::Font>();
		font->loadFromFile("resources/cour.ttf");
	}
	coords_display.setFont(*font);
	coords_display.setCharacterSize(16);
	coords_display.setFillColor(sf::Color::Black);

	text_back.setFillColor(sf::Color(255, 255, 255, 190));
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
		}
	}

	if (event.type == sf::Event::MouseWheelMoved) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			palette_steps += event.mouseWheel.delta;

			if (palette_steps < 0) palette_steps = 0;

			brot_shdr.setUniform("palette_steps", palette_steps);

		} else {
			float fctr = event.mouseWheel.delta > 0 ? zoom_factor : 1.f / zoom_factor;

			scale *= fctr;
			center += sf::Vector2f(mouse.x / view_size.x - 0.5f, 0.5f - mouse.y / view_size.y) * (1.f / fctr - 1.f) * scale / 2.f;
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
				(mouse.x - panning_anchor.x) / view_size.x * scale / 2.f,
				(panning_anchor.y - mouse.y) / view_size.y * scale / 2.f
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

void Mandelbrot::show(sf::RenderWindow& window) {
	brot_shdr.setUniform("center", center - panning_offset);
	brot_shdr.setUniform("scale", scale);

	sf::Sprite spr(brot_texture.getTexture());
	window.draw(spr, &brot_shdr);

	coords_display.setString(std::format("pos: {}, {}\nscale: {}", center.x - panning_offset.x, center.y - panning_offset.y, scale));

	auto b = coords_display.getGlobalBounds();
	text_back.setPosition(b.left, b.top);
	text_back.setSize({ b.width, b.height });
	window.draw(text_back);
	
	window.draw(coords_display);
}