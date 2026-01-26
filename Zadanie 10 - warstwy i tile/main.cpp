#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "ParallaxLayer.h"

const float winW = 1600.f;
const float winH = 900.f;

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)winW, (unsigned int)winH }), "Zadanie 10 - warstwy i tile");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("textures/arial.ttf")) {
    }

    sf::Text guiText(font, "", 22);
    guiText.setFillColor(sf::Color::Black);
    guiText.setPosition({ 25.f, 25.f });

    sf::RectangleShape guiBg({ 500.f, 160.f });
    guiBg.setFillColor(sf::Color(255, 255, 255, 200));
    guiBg.setOutlineThickness(2.f);
    guiBg.setOutlineColor(sf::Color::Black);
    guiBg.setPosition({ 15.f, 15.f });

    Level level;
    float floorY = 800.f;

    Player player("textures/Pies.png", { 400.f, floorY - 50.f }, sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D, CollisionShape::Rectangle);
    player.getSprite().setScale({0.2f, 0.2f});

    ParallaxLayer layer3({"textures/Chmura.png"}, 0.9f, 50.f, 400.f, 20, 5000.f, 150.f, false);
    ParallaxLayer layer2({"textures/Klebek.png", "textures/Legowisko.png"}, 0.6f, 800.f, 820.f, 10, 4000.f, 550.f, true);
    ParallaxLayer layer1({"textures/Kosc.png", "textures/Smaczek.png"}, 0.3f, 800.f, 800.f, 15, 3000.f, 100.f, true);

    int selected = 3;
    Camera camera(winW, winH);
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Num1) selected = 1;
                if (keyPressed->code == sf::Keyboard::Key::Num2) selected = 2;
                if (keyPressed->code == sf::Keyboard::Key::Num3) selected = 3;

                float step = 0.05f;
                if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::Down) {
                    float sign = (keyPressed->code == sf::Keyboard::Key::Up) ? 1.f : -1.f;
                    if (selected == 1) layer1.setFactor(std::max(0.f, std::min(1.f, layer1.getFactor() + step * sign)));
                    if (selected == 2) layer2.setFactor(std::max(0.f, std::min(1.f, layer2.getFactor() + step * sign)));
                    if (selected == 3) layer3.setFactor(std::max(0.f, std::min(1.f, layer3.getFactor() + step * sign)));
                }
            }
        }

        player.handleInput();
        player.applyMovementX(dt);
        player.applyMovementY(dt);
        level.resolveCollision(player);
        camera.update({player.getSprite().getPosition().x, winH / 2.f});

        std::stringstream ss;
        ss << "WARSTWA: " << selected << " [1, 2, 3] | ZMIANA: GORA/DOL\n";
        ss << "--------------------------------------\n";
        ss << "W1 (Smaczki) Factor: " << std::fixed << std::setprecision(2) << layer1.getFactor() << (selected == 1 ? " <" : "") << "\n";
        ss << "W2 (Klebki)  Factor: " << std::fixed << std::setprecision(2) << layer2.getFactor() << (selected == 2 ? " <" : "") << "\n";
        ss << "W3 (Chmury)  Factor: " << std::fixed << std::setprecision(2) << layer3.getFactor() << (selected == 3 ? " <" : "") << "\n";
        guiText.setString(ss.str());

        window.clear(sf::Color(255, 241, 203));

        window.setView(camera.view);
        layer3.draw(window, camera.view);
        layer2.draw(window, camera.view);
        layer1.draw(window, camera.view);
        level.draw(window, camera.view);
        player.draw(window, false);

        window.setView(window.getDefaultView());
        window.draw(guiBg);
        window.draw(guiText);

        window.display();
    }
    return 0;
}