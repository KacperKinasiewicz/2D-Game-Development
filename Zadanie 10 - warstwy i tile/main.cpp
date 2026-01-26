#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "Level.h"
#include "Player.h"
#include "Camera.h"

const float winW = 1600.f;
const float winH = 900.f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)winW, (unsigned int)winH }), "Zadanie - 100x100");
    window.setFramerateLimit(60);

    Level level;

    Player player("textures/Pies.png", { 0.f, 500.f }, sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D, CollisionShape::Rectangle);

    player.getSprite().setScale({0.2f, 0.2f});

    Camera camera(winW, winH);

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        player.handleInput();
        player.applyMovementX(dt);
        player.applyMovementY(dt);
        level.resolveCollision(player);

        camera.update({player.getSprite().getPosition().x, winH / 2.f});

        window.clear(sf::Color(255, 241, 203));

        window.setView(camera.view);

        level.draw(window, camera.view);
        player.draw(window, false);

        window.display();
    }
    return 0;
}