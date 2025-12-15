#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "4. 2D Game");

    std::ifstream file("level.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku level.txt\n";
        return 1;
    }

    const float tileSize = 40.f;
    sf::Texture wallTex, woodTex, starTex;
    if (!wallTex.loadFromFile("textures/pinkwool.jpg")) std::cerr << "Nie uda³o siê wczytaæ wall.png\n";
    if (!woodTex.loadFromFile("textures/lavenderwool.jpg")) std::cerr << "Nie uda³o siê wczytaæ wood.png\n";
    if (!starTex.loadFromFile("textures/treat.png")) std::cerr << "Nie uda³o siê wczytaæ star.png\n";

    std::vector<std::pair<sf::RectangleShape, sf::Texture*>> objects;

    std::string line;
    int row = 0;
    int maxCols = 0;
    int maxRows = 0;
    while (std::getline(file, line)) {
        if (line.size() > maxCols) maxCols = line.size();
        for (size_t col = 0; col < line.size(); ++col) {
            char c = line[col];
            sf::Texture* tex = nullptr;
            if (c == '|') tex = &wallTex;
            else if (c == '-') tex = &woodTex;
            else if (c == '*') tex = &starTex;
            if (tex) {
                sf::RectangleShape rect(sf::Vector2f(tileSize, tileSize));
                rect.setPosition({ col * tileSize, row * tileSize });
                rect.setTexture(tex);
                objects.push_back({ rect, tex });
            }
        }
        ++row;
    }
    maxRows = row;

    sf::Texture player1Tex;
    if (!player1Tex.loadFromFile("textures/dog.jpg")) std::cerr << "Nie uda³o siê wczytaæ dog.jpg\n";
    sf::RectangleShape player1(sf::Vector2f(60.f, 60.f));
    player1.setTexture(&player1Tex);
    player1.setPosition({ 100.f, 100.f });

    sf::Texture player2Tex;
    if (!player2Tex.loadFromFile("textures/cat.png")) std::cerr << "Nie uda³o siê wczytaæ cat.png\n";
    player2Tex.setSmooth(true);
    sf::Sprite player2(player2Tex);
    player2.setScale({ 0.15f, 0.15f });
    player2.setPosition({ 200.f, 200.f });

    float step = 10.f;
    float smoothPlayer2 = 0.1f;
    sf::View view = window.getDefaultView();
    float levelWidth = tileSize * maxCols;
    float levelHeight = tileSize * maxRows;

    const float pushZoneWidth = 300.f;
    const float pushZoneHeight = 200.f;
    const float targetFocusSmooth = 0.05f;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) player1.move({ 0.f, -step });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) player1.move({ 0.f, step });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) player1.move({ -step, 0.f });
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) player1.move({ step, 0.f });

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f catCenter = player2.getGlobalBounds().getCenter();
        sf::Vector2f toMouse = mousePos - catCenter;
        player2.move(toMouse * smoothPlayer2);

        sf::Vector2f viewCenter = view.getCenter();
        sf::Vector2f halfSize = view.getSize() / 2.f;

        float left = viewCenter.x - pushZoneWidth / 2;
        float right = viewCenter.x + pushZoneWidth / 2;
        float top = viewCenter.y - pushZoneHeight / 2;
        float bottom = viewCenter.y + pushZoneHeight / 2;

        sf::Vector2f playerCenter = player1.getPosition() + player1.getSize() / 2.f;

        if (playerCenter.x < left) viewCenter.x -= (left - playerCenter.x);
        if (playerCenter.x > right) viewCenter.x += (playerCenter.x - right);
        if (playerCenter.y < top) viewCenter.y -= (top - playerCenter.y);
        if (playerCenter.y > bottom) viewCenter.y += (playerCenter.y - bottom);

        if (viewCenter.x - halfSize.x < 0) viewCenter.x = halfSize.x;
        if (viewCenter.y - halfSize.y < 0) viewCenter.y = halfSize.y;
        if (viewCenter.x + halfSize.x > levelWidth) viewCenter.x = levelWidth - halfSize.x;
        if (viewCenter.y + halfSize.y > levelHeight) viewCenter.y = levelHeight - halfSize.y;

        sf::Vector2f targetFocus = player2.getGlobalBounds().getCenter();
        viewCenter += (targetFocus - viewCenter) * targetFocusSmooth;

        view.setCenter(viewCenter);
        window.setView(view);

        window.clear(sf::Color(255, 241, 203));
        for (auto& obj : objects)
            window.draw(obj.first);

        window.draw(player1);
        window.draw(player2);

        window.display();
    }

    return 0;
}
