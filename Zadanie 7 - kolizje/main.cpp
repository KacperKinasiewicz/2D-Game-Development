#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <optional>
#include <algorithm>
#include <random>

// --- MATEMATYKA (Dla kamery i wskaźnika) ---

float length(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(sf::Vector2f v) {
    float l = length(v);
    if (l != 0.f) return v / l;
    return v;
}

// --- STRUKTURA GRY ---

struct Game {
    sf::RenderWindow& window;

    // Zasoby
    sf::Texture wallTex, dogTex, catTex, starTex;

    // Obiekty
    std::vector<sf::RectangleShape> walls;
    std::vector<sf::Vector2f> emptySpots;

    sf::RectangleShape dog; // Pies (Kwadrat)
    sf::CircleShape cat;    // Kot (Koło)
    sf::Sprite goal;        // Cel

    // Kamera i UI
    sf::View view;
    sf::ConvexShape arrow;
    sf::Vector2f cameraCenter;
    float currentZoom = 1.0f;
    float tileSize = 50.f;

    Game(sf::RenderWindow& win) : window(win) {
        // 1. Ładowanie tekstur
        if (!wallTex.loadFromFile("textures/pinkwool.jpg")) {}
        if (!dogTex.loadFromFile("textures/dog.png")) {}
        if (!catTex.loadFromFile("textures/cat.png")) {}
        if (!starTex.loadFromFile("textures/treat.png")) {}

        dogTex.setSmooth(true);
        catTex.setSmooth(true);

        // 2. Konfiguracja Psa
        dog.setSize({ 30.f, 30.f });
        dog.setOrigin({ 15.f, 15.f });
        dog.setTexture(&dogTex);

        // 3. Konfiguracja Kota
        cat.setRadius(15.f);
        cat.setOrigin({ 15.f, 15.f });
        cat.setTexture(&catTex);

        // 4. Konfiguracja Celu
        goal.setTexture(starTex);
        sf::Vector2f texSize(starTex.getSize());
        goal.setOrigin(texSize / 2.f);
        goal.setScale({ 0.5f, 0.5f });

        // 5. Konfiguracja Wskaźnika (Strzałki)
        arrow.setPointCount(3);
        arrow.setPoint(0, { 0.f, 0.f });
        arrow.setPoint(1, { -10.f, 20.f });
        arrow.setPoint(2, { 10.f, 20.f });
        arrow.setFillColor(sf::Color::Red);
        arrow.setOrigin({ 0.f, 10.f });

        view = window.getDefaultView();
        cameraCenter = view.getCenter();
    }

    // Funkcja ładująca poziom z pliku
    bool loadLevel(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        walls.clear();
        emptySpots.clear();

        std::string line;
        int row = 0;
        while (std::getline(file, line)) {
            for (size_t col = 0; col < line.size(); ++col) {
                sf::Vector2f pos(col * tileSize, row * tileSize);

                if (line[col] == 'O') {
                    // Tworzenie ściany
                    sf::RectangleShape w({ tileSize, tileSize });
                    w.setPosition(pos);
                    w.setTexture(&wallTex);
                    walls.push_back(w);
                }
                else if (line[col] != '\r' && line[col] != '\n') {
                    // Zapisanie wolnego miejsca (środek kratki)
                    emptySpots.push_back(pos + sf::Vector2f(tileSize / 2.f, tileSize / 2.f));
                }
            }
            row++;
        }

        if (emptySpots.size() < 3) return false;

        // Losowanie pozycji startowych
        std::shuffle(emptySpots.begin(), emptySpots.end(), std::mt19937(std::random_device()()));

        dog.setPosition(emptySpots[0]);
        cat.setPosition(emptySpots[1]);
        goal.setPosition(emptySpots[2]);

        return true;
    }

    // Funkcja aktualizująca (Ruch + Kamera)
    void update() {
        float speed = 5.f;

        // --- RUCH PSA (WSAD) ---
        sf::Vector2f dogMove(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dogMove.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dogMove.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dogMove.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dogMove.x += speed;

        dog.move(dogMove); // Brak sprawdzania kolizji!

        // --- RUCH KOTA (STRZAŁKI) ---
        sf::Vector2f catMove(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) catMove.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) catMove.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) catMove.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) catMove.x += speed;

        cat.move(catMove); // Brak sprawdzania kolizji!

        // --- KAMERA (Zoom-to-fit + Lerp) ---
        sf::Vector2f p1 = dog.getPosition();
        sf::Vector2f p2 = cat.getPosition();
        sf::Vector2f targetCenter = (p1 + p2) * 0.5f;

        sf::Vector2f baseSize(800.f, 600.f);
        float padding = 150.f;
        float width = std::abs(p1.x - p2.x) + padding;
        float height = std::abs(p1.y - p2.y) + padding;

        float zx = width / baseSize.x;
        float zy = height / baseSize.y;
        float targetZoom = std::clamp(std::max(zx, zy), 0.6f, 2.5f);

        // Wygładzanie ruchu (Lerp)
        float lerp = 0.1f;
        cameraCenter += (targetCenter - cameraCenter) * lerp;
        currentZoom += (targetZoom - currentZoom) * lerp;

        view.setSize(baseSize * currentZoom);
        view.setCenter(cameraCenter);
        window.setView(view);
    }

    // Funkcja rysująca
    void draw() {
        window.clear(sf::Color(255, 241, 203)); // Tło kremowe

        // Rysowanie ścian (tylko widocznych na ekranie)
        sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
        for (const auto& w : walls) {
            if (w.getGlobalBounds().findIntersection(viewRect))
                window.draw(w);
        }

        window.draw(goal);
        window.draw(dog);
        window.draw(cat);

        // Rysowanie strzałki wskaźnika (jeśli cel poza ekranem)
        if (!viewRect.contains(goal.getPosition())) {
            sf::Vector2f dir = goal.getPosition() - view.getCenter();
            float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
            sf::Vector2f arrowPos = view.getCenter() + normalize(dir) * (view.getSize().y / 2.3f);

            arrow.setPosition(arrowPos);
            arrow.setRotation(sf::degrees(angle + 90.f));
            arrow.setScale({ currentZoom, currentZoom });
            window.draw(arrow);
        }

        window.display();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Projekt Labirynt - Podstawy");
    window.setFramerateLimit(60);

    Game game(window);

    // Wczytujemy tylko pierwszy poziom dla testu
    if (!game.loadLevel("level1.txt")) {
        std::cerr << "Nie udalo sie wczytac level1.txt. Upewnij sie, ze plik istnieje!\n";
        return 1;
    }

    while (window.isOpen()) {
        // Obsługa zdarzeń SFML 3.0
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        game.update();
        game.draw();
    }

    return 0;
}