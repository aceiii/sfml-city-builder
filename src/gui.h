//
// Created by Borin Ouch on 2016-03-23.
//

#ifndef SFML_TEST_GUI_H
#define SFML_TEST_GUI_H


#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <string>


class GuiStyle {

public:
    sf::Color bodyColor;
    sf::Color bodyHighlightColor;
    sf::Color borderColor;
    sf::Color borderHighlightColor;
    sf::Color textColor;
    sf::Color textHighlightColor;

    sf::Font* font;

    float borderSize;

public:
    GuiStyle();
    GuiStyle(sf::Font* font, float borderSize,
             sf::Color bodyColor, sf::Color borderColor, sf::Color textColor,
             sf::Color bodyHighlightColor, sf::Color borderHighlightColor, sf::Color textHighlightColor);

};


class GuiEntry {

public:
    sf::RectangleShape shape;
    sf::Text text;

    std::string message;

public:
    GuiEntry();
    GuiEntry(const std::string& message, sf::RectangleShape shape, sf::Text text);
};


class Gui : public sf::Transformable, public sf::Drawable {

private:
    bool horizontal;
    int padding;
    sf::Vector2f dimensions;
    GuiStyle style;

public:
    std::vector<GuiEntry> entries;
    bool visible;

public:
    sf::Vector2f getSize();

    int getEntry(const sf::Vector2f mousePos);
    void setEntryText(int entry, std::string text);
    void setDiemnsions(sf::Vector2f dimensions);

    void show();
    void hide();

    void highlight(const int entry);

    std::string activate(const int entry);
    std::string activate(const sf::Vector2f mousePos);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    Gui(sf::Vector2f dimensions, int padding, bool horizontal,
        GuiStyle& style, std::vector<std::pair<std::string, std::string> > entries);
};


#endif //SFML_TEST_GUI_H
