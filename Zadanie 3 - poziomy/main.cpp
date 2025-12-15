#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

struct GameObject {
    sf::RectangleShape shape;
};

sf::Texture& loadTexture(const std::string& filename, std::map<std::string, sf::Texture>& textures) {
    if (textures.find(filename) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Nie uda³o siê wczytaæ tekstury: " << filename << std::endl;
        }
        textures[filename] = texture;
    }
    return textures[filename];
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "3. 2D Game");

    std::ifstream file("level.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku level.txt\n";
        return 1;
    }

    std::string line;
    std::vector<GameObject> objects;
    std::map<std::string, sf::Texture> textures;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        float x, y, w, h;
        std::string textureFile;

        if (!(ss >> x >> y >> w >> h >> textureFile)) {
            std::cerr << "B³¹d w linii: " << line << std::endl;
            continue;
        }

        GameObject obj;
        obj.shape = sf::RectangleShape(sf::Vector2f(w, h));
        obj.shape.setPosition({ x, y });
        obj.shape.setTexture(&loadTexture(textureFile, textures));

        objects.push_back(obj);
    }

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(222, 184, 135));

        for (auto& obj : objects)
            window.draw(obj.shape);

        window.display();
    }

    return 0;
}
