#include <SFML/Graphics.hpp>
#include <string>
#include "Level.h"
#include "Camera.h"
#include "Player.h"
#include "GameOverScreen.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Zadanie 7 - kolizje");
    window.setFramerateLimit(60);

    int dogScore = 0;
    int catScore = 0;
    int currentLevelIndex = 1;
    bool isGameFinished = false;

    GameOverScreen gameOverScreen;
    Level currentLevel;
    Camera gameCamera((sf::Vector2f)window.getSize());

    Player playerDog("textures/dog.png", CollisionShape::Square, { 0,0 },
        sf::Keyboard::Key::W, sf::Keyboard::Key::S,
        sf::Keyboard::Key::A, sf::Keyboard::Key::D);

    Player playerCat("textures/cat.png", CollisionShape::Circle, { 0,0 },
        sf::Keyboard::Key::Up, sf::Keyboard::Key::Down,
        sf::Keyboard::Key::Left, sf::Keyboard::Key::Right);

    auto loadNextLevel = [&]() {
        if (currentLevelIndex > 3) {
            isGameFinished = true;
            return;
        }

        std::string filename = "levels/level" + std::to_string(currentLevelIndex) + ".txt";
        if (!currentLevel.loadLevelFromFile(filename)) {
            isGameFinished = true;
            return;
        }

        currentLevel.respawnTreat();

        playerDog.getSprite().setPosition(currentLevel.findRandomEmptyPosition());
        playerCat.getSprite().setPosition(currentLevel.findRandomEmptyPosition());

        playerDog.velocity = { 0.f, 0.f };
        playerCat.velocity = { 0.f, 0.f };

        gameCamera = Camera((sf::Vector2f)window.getSize());
        gameCamera.updateCameraPosition(playerDog.getSprite(), playerCat.getSprite());
    };

    auto restartGame = [&]() {
        dogScore = 0;
        catScore = 0;
        currentLevelIndex = 1;
        isGameFinished = false;
        loadNextLevel();
    };

    loadNextLevel();

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (isGameFinished && event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
                    restartGame();
                }
            }
        }

        window.clear(sf::Color(255, 241, 203));

        if (!isGameFinished) {
            playerDog.handleInput();
            playerCat.handleInput();

            playerDog.applyMovementX();
            currentLevel.resolveWallCollisions(playerDog, true);
            playerCat.applyMovementX();
            currentLevel.resolveWallCollisions(playerCat, true);

            playerDog.applyMovementY();
            currentLevel.resolveWallCollisions(playerDog, false);
            playerCat.applyMovementY();
            currentLevel.resolveWallCollisions(playerCat, false);

            currentLevel.constrainPlayerToMap(playerDog.getSprite());
            currentLevel.constrainPlayerToMap(playerCat.getSprite());

            sf::FloatRect treatBounds = currentLevel.treatSprite.getGlobalBounds();
            if (playerDog.getSprite().getGlobalBounds().findIntersection(treatBounds)) {
                dogScore++;
                currentLevelIndex++;
                loadNextLevel();
            }
            else if (playerCat.getSprite().getGlobalBounds().findIntersection(treatBounds)) {
                catScore++;
                currentLevelIndex++;
                loadNextLevel();
            }

            gameCamera.keepPlayerInView(playerDog.getSprite());
            gameCamera.keepPlayerInView(playerCat.getSprite());

            playerDog.updateDirectionArrow(currentLevel.getTreatPosition());
            playerCat.updateDirectionArrow(currentLevel.getTreatPosition());

            gameCamera.updateCameraPosition(playerDog.getSprite(), playerCat.getSprite());

            window.setView(gameCamera.gameView);
            currentLevel.draw(window);
            playerDog.draw(window);
            playerCat.draw(window);
        }
        else {
            window.setView(window.getDefaultView());
            gameOverScreen.draw(window, dogScore, catScore);
        }

        window.display();
    }
    return 0;
}