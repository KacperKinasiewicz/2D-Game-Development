#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class CollisionShape {
    Square,
    Circle
};

class Player {
public:
    Player(const std::string& textureFilename, CollisionShape shape, sf::Vector2f initialPosition,
           sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
           sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight);

    void handleInput();

    void applyMovementX();
    void applyMovementY();

    void updateDirectionArrow(sf::Vector2f targetPosition);
    void draw(sf::RenderWindow& window);

    sf::Sprite& getSprite();
    CollisionShape getCollisionShape() const;

    sf::Vector2f velocity{ 0.f, 0.f };

private:
    const float movementSpeed = 4.0f;
    const float targetPixelSize = 40.0f;
    const float arrowScale = 0.05f;
    const float arrowDistance = 30.0f;
    const float arrowShowThreshold = 10000.f;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::Texture arrowTexture;
    sf::Sprite arrowSprite;
    bool isArrowVisible = false;

    CollisionShape collisionShape;

    sf::Keyboard::Key kUp;
    sf::Keyboard::Key kDown;
    sf::Keyboard::Key kLeft;
    sf::Keyboard::Key kRight;
};