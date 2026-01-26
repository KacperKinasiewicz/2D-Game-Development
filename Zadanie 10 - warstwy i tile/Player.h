#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class CollisionShape { Rectangle, Circle };

class Player {
public:
    Player(const std::string& textureFilename, sf::Vector2f initialPosition,
           sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
           sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight, CollisionShape shape);

    void handleInput();
    void applyMovementX(float dt);
    void applyMovementY(float dt);
    void draw(sf::RenderWindow& window, bool drawHitbox);

    sf::Sprite& getSprite();
    void calculatePhysicsConstants();
    CollisionShape getCollisionShape() const;

    float hMax = 200.0f;
    float dHalf = 180.0f;
    float moveSpeed = 500.0f;
    float ghostJumpMaxTime = 0.0f;
    float fastFallMultiplier = 1.0f;

    float gravity = 0.f;
    float jumpVelocity = 0.f;

    sf::Vector2f velocity{ 0.f, 0.f };
    bool isGrounded = false;
    bool isDropping = false;

    int jumpCount = 0;
    float coyoteTimer = 0.f;
    bool jumpKeyWasPressed = false;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float targetPixelSize = 50.0f;

    sf::Keyboard::Key kUp;
    sf::Keyboard::Key kDown;
    sf::Keyboard::Key kLeft;
    sf::Keyboard::Key kRight;

    CollisionShape collisionShape;
    sf::RectangleShape debugRect;
    sf::CircleShape debugCircle;
};