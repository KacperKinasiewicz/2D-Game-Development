#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <iomanip>
#include <sstream>
#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "GameOverScreen.h"

void updateInfoText(sf::Text& text, const Player& p) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2)
       << "Maksymalna wysokosc (1/2): " << p.hMax
       << " | Odleglosc do szczytu (3/4): " << p.dHalf
       << " | Czas Ghost Jump (5/6): " << p.ghostJumpMaxTime << "s"
       << " | Mnoznik opadania (7/8): " << p.fastFallMultiplier << "x"
       << " | Predkosc poczatkowa: " << p.jumpVelocity
       << " | Grawitacja: " << p.gravity;
    text.setString(ss.str());
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
    text.setPosition({ 800.f, 875.f });
}

void handleParameterInput(sf::Keyboard::Key code, Player& p1, Player& p2) {
    if (code == sf::Keyboard::Key::Num1) { p1.hMax = std::min(500.f, p1.hMax + 10.f); }
    else if (code == sf::Keyboard::Key::Num2) { p1.hMax = std::max(10.f, p1.hMax - 10.f); }
    else if (code == sf::Keyboard::Key::Num3) { p1.dHalf = std::min(500.f, p1.dHalf + 10.f); }
    else if (code == sf::Keyboard::Key::Num4) { p1.dHalf = std::max(10.f, p1.dHalf - 10.f); }
    else if (code == sf::Keyboard::Key::Num5) { p1.ghostJumpMaxTime = std::min(1.0f, p1.ghostJumpMaxTime + 0.05f); }
    else if (code == sf::Keyboard::Key::Num6) { p1.ghostJumpMaxTime = std::max(0.0f, p1.ghostJumpMaxTime - 0.05f); }
    else if (code == sf::Keyboard::Key::Num7) { p1.fastFallMultiplier = std::min(10.0f, p1.fastFallMultiplier + 0.5f); }
    else if (code == sf::Keyboard::Key::Num8) { p1.fastFallMultiplier = std::max(1.0f, p1.fastFallMultiplier - 0.5f); }

    p2.hMax = p1.hMax;
    p2.dHalf = p1.dHalf;
    p2.ghostJumpMaxTime = p1.ghostJumpMaxTime;
    p2.fastFallMultiplier = p1.fastFallMultiplier;

    p1.calculatePhysicsConstants();
    p2.calculatePhysicsConstants();
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Zadanie 8 - Skoki");
    window.setFramerateLimit(60);

    Level level;
    if (!level.loadLevelFromFile("levels/level1.txt")) return 1;

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) return 1;

    sf::Text uiText(font);
    uiText.setCharacterSize(15);
    uiText.setFillColor(sf::Color::Black);

    sf::RectangleShape uiBackground({ 1600.f, 50.f });
    uiBackground.setFillColor(sf::Color(255, 241, 203));
    uiBackground.setOutlineThickness(-3.f);
    uiBackground.setOutlineColor(sf::Color(243, 135, 187));
    uiBackground.setPosition({ 0.f, 850.f });

    sf::RectangleShape divider({ 50.f, 850.f });
    divider.setFillColor(sf::Color(255, 241, 203));
    divider.setPosition({ 775.f, 0.f });

    float startY = level.getMapHeight() - (level.tileSize * 1.5f);
    Player pDog("textures/dog.png", { level.tileSize * 2.5f, startY }, sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D, CollisionShape::Rectangle);
    Player pCat("textures/cat.png", { level.getMapWidth() - (level.tileSize * 2.5f), startY }, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, CollisionShape::Circle);

    sf::Texture dogT, catT;
    dogT.loadFromFile("textures/dog.png"); catT.loadFromFile("textures/cat.png");
    sf::Sprite dogI(dogT), catI(catT);
    dogI.setScale({ 100.f / dogT.getSize().x, 100.f / dogT.getSize().x });
    catI.setScale({ 100.f / catT.getSize().x, 100.f / catT.getSize().x });

    Camera dogCam({ 1600.f, 850.f }, level.getMapWidth());
    Camera catCam({ 1600.f, 850.f }, level.getMapWidth());
    GameOverScreen gos;

    bool isGameFinished = false;
    bool showHitboxes = false;
    std::string winner = "";

    auto restart = [&]() {
        float sY = level.getMapHeight() - (level.tileSize * 1.5f);
        pDog.getSprite().setPosition({ level.tileSize * 2.5f, sY }); pDog.velocity = { 0.f, 0.f }; pDog.isGrounded = false;
        pCat.getSprite().setPosition({ level.getMapWidth() - (level.tileSize * 2.5f), sY }); pCat.velocity = { 0.f, 0.f }; pCat.isGrounded = false;
        isGameFinished = false; winner = "";
        dogCam = Camera({ 1600.f, 850.f }, level.getMapWidth());
        catCam = Camera({ 1600.f, 850.f }, level.getMapWidth());
        dogCam.gameView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 0.484375f, 850.f / 900.f }));
        catCam.gameView.setViewport(sf::FloatRect({ 0.515625f, 0.f }, { 0.484375f, 850.f / 900.f }));
        dogCam.currentCenter = { level.getMapWidth() / 2.f, level.getMapHeight() - dogCam.gameView.getSize().y / 2.f };
        catCam.currentCenter = { level.getMapWidth() / 2.f, level.getMapHeight() - catCam.gameView.getSize().y / 2.f };
    };

    restart();

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::R) restart();
                if (isGameFinished && keyPressed->code == sf::Keyboard::Key::Space) restart();
                if (keyPressed->code == sf::Keyboard::Key::T) showHitboxes = !showHitboxes;
                handleParameterInput(keyPressed->code, pDog, pCat);
            }
        }

        if (!isGameFinished) {
            float dt = 1.f / 60.f;
            pDog.handleInput(); pDog.applyMovementX(); level.resolveWallCollisions(pDog, true); pDog.applyMovementY(dt); level.resolveWallCollisions(pDog, false);
            pCat.handleInput(); pCat.applyMovementX(); level.resolveWallCollisions(pCat, true); pCat.applyMovementY(dt); level.resolveWallCollisions(pCat, false);
            dogCam.updateCameraPosition(pDog.getSprite(), level.getMapWidth(), level.getMapHeight());
            catCam.updateCameraPosition(pCat.getSprite(), level.getMapWidth(), level.getMapHeight());

            if (pDog.getSprite().getGlobalBounds().findIntersection(level.treatSprite.getGlobalBounds())) { winner = "Dog"; isGameFinished = true; }
            else if (pCat.getSprite().getGlobalBounds().findIntersection(level.treatSprite.getGlobalBounds())) { winner = "Cat"; isGameFinished = true; }
        }

        updateInfoText(uiText, pDog);

        window.clear(sf::Color(255, 241, 203));
        if (!isGameFinished) {
            window.setView(dogCam.gameView); level.draw(window); pDog.draw(window, showHitboxes); pCat.draw(window, showHitboxes);
            dogI.setPosition({ dogCam.gameView.getCenter().x - dogCam.gameView.getSize().x / 2.f, dogCam.gameView.getCenter().y - dogCam.gameView.getSize().y / 2.f }); window.draw(dogI);
            window.setView(catCam.gameView); level.draw(window); pDog.draw(window, showHitboxes); pCat.draw(window, showHitboxes);
            catI.setPosition({ catCam.gameView.getCenter().x + catCam.gameView.getSize().x / 2.f - 100.f, catCam.gameView.getCenter().y - catCam.gameView.getSize().y / 2.f }); window.draw(catI);
            window.setView(window.getDefaultView()); window.draw(divider); window.draw(uiBackground); window.draw(uiText);
        } else {
            window.setView(window.getDefaultView()); gos.draw(window, winner);
        }
        window.display();
    }
    return 0;
}