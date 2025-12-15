#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Ball {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

float dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

sf::Vector2f reflect(sf::Vector2f N, sf::Vector2f V) {
    return V - 2.0f * dot(N, V) * N;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Laboratorium 5: Kolizje miedzy okregami");
    window.setFramerateLimit(60);

    sf::Texture catTex;
    if (!catTex.loadFromFile("textures/cat.png")) return -1;

    std::vector<Ball> balls;
    for (int i = 0; i < 10; i++) {
        Ball b;
        b.shape.setRadius(40.f);
        b.shape.setOrigin({ 40.f, 40.f });
        b.shape.setPosition({ 100.f + i * 120.f, 300.f });
        b.shape.setTexture(&catTex);

        b.shape.setOutlineThickness(-3.f);
        b.shape.setOutlineColor(sf::Color(128, 0, 128));

        b.velocity = { (i % 2 == 0 ? 6.f : -6.f), (i % 2 == 0 ? 6.f : -6.f) };

        balls.push_back(b);
    }

    bool separationOn = true;
    bool reflectionOn = true;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::S) separationOn = !separationOn;
                if (key->code == sf::Keyboard::Key::B) reflectionOn = !reflectionOn;
            }
        }

        for (size_t i = 0; i < balls.size(); ++i) {
            balls[i].shape.move(balls[i].velocity);

            sf::Vector2f pos = balls[i].shape.getPosition();
            float r = balls[i].shape.getRadius();

            if (pos.x - r < 0) { balls[i].shape.setPosition({ r, pos.y });          balls[i].velocity.x *= -1; }
            if (pos.x + r > 800) { balls[i].shape.setPosition({ 800 - r, pos.y });  balls[i].velocity.x *= -1; }
            if (pos.y - r < 0) { balls[i].shape.setPosition({ pos.x, r });          balls[i].velocity.y *= -1; }
            if (pos.y + r > 600) { balls[i].shape.setPosition({ pos.x, 600 - r });  balls[i].velocity.y *= -1; }

            for (size_t j = i + 1; j < balls.size(); ++j) {
                sf::Vector2f delta = balls[j].shape.getPosition() - balls[i].shape.getPosition();
                float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
                float minDetail = balls[i].shape.getRadius() + balls[j].shape.getRadius();

                if (distance < minDetail) {
                    sf::Vector2f normal = delta / distance;

                    if (reflectionOn) {
                        balls[i].velocity = reflect(-normal, balls[i].velocity);
                        balls[j].velocity = reflect(normal, balls[j].velocity);
                    }

                    if (separationOn) {
                        float overlap = (minDetail - distance) / 2.f;
                        balls[i].shape.move(-normal * overlap);
                        balls[j].shape.move(normal * overlap);
                    }
                }
            }
        }

        window.clear(sf::Color(255, 241, 203));
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
        window.display();
    }
    return 0;
}