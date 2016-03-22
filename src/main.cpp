

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


int main(int argc, char* argv[]) {
    std::cout << "Hello World!!" << std::endl;

    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "City Builder");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("data/fonts/arial.ttf");

    sf::Text message;
    message.setFont(font);
    message.setString("Hello World");
    message.setCharacterSize(100);
    message.setColor(sf::Color::White);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Blue);

        window.draw(message);

        window.display();
    }

    return 0;
}


