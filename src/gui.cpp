//
// Created by Borin Ouch on 2016-03-23.
//

#include "gui.h"


GuiStyle::GuiStyle() {
}

GuiStyle::GuiStyle(sf::Font *font, float borderSize, sf::Color bodyColor, sf::Color borderColor, sf::Color textColor,
                   sf::Color bodyHighlightColor, sf::Color borderHighlightColor, sf::Color textHighlightColor)
{
    this->bodyColor = bodyColor;
    this->borderColor = borderColor;
    this->textColor = textColor;
    this->bodyHighlightColor = bodyHighlightColor;
    this->borderHighlightColor = borderHighlightColor;
    this->textHighlightColor = textHighlightColor;
    this->font = font;
    this->borderSize = borderSize;
}

GuiEntry::GuiEntry() {
}

GuiEntry::GuiEntry(const std::string &message, sf::RectangleShape shape, sf::Text text) {
    this->message = message;
    this->shape = shape;
    this->text = text;
}

sf::Vector2f Gui::getSize() {
    return sf::Vector2f(dimensions.x, dimensions.y * entries.size());
}

int Gui::getEntry(const sf::Vector2f mousePos) {
    if (entries.size() == 0) {
        return -1;
    }
    if (!visible) {
        return -1;
    }

    for (int i = 0; i < entries.size(); i += 1) {
        sf::Vector2f point = mousePos;
        point += entries[i].shape.getOrigin();
        point -= entries[i].shape.getPosition();

        if (point.x < 0 || point.x > entries[i].shape.getScale().x * dimensions.x) {
            continue;
        }
        if (point.y < 0 || point.y > entries[i].shape.getScale().y * dimensions.y) {
            continue;
        }

        return i;
    }

    return -1;
}

void Gui::setEntryText(int entry, std::string text) {
    if (entry >= entries.size() || entry < 0) {
        return;
    }

    entries[entry].text.setString(text);
}

void Gui::setDiemnsions(sf::Vector2f dimensions) {
    this->dimensions = dimensions;

    for (auto& entry : entries) {
        entry.shape.setSize(dimensions);
        entry.text.setCharacterSize((unsigned int)(dimensions.y - style.borderSize) - padding);
    }
}

void Gui::show() {
    sf::Vector2f offset(0.0f, 0.0f);

    visible = true;

    for (auto& entry : entries) {
        sf::Vector2f origin = getOrigin();
        origin -= offset;

        entry.shape.setOrigin(origin);
        entry.text.setOrigin(origin);

        entry.shape.setPosition(getPosition());
        entry.text.setPosition(getPosition());

        if (horizontal) {
            offset.x += dimensions.x;
        } else {
            offset.y += dimensions.y;
        }
    }
}

void Gui::hide() {
    visible = false;
}

void Gui::highlight(const int entry) {
    for (int i = 0; i < entries.size(); i += 1) {
        if (i == entry) {
            entries[i].shape.setFillColor(style.bodyHighlightColor);
            entries[i].shape.setOutlineColor(style.borderHighlightColor);
            entries[i].text.setColor(style.textHighlightColor);
        } else {
            entries[i].shape.setFillColor(style.bodyColor);
            entries[i].shape.setOutlineColor(style.borderColor);
            entries[i].text.setColor(style.textColor);
        }
    }
}

std::string Gui::activate(const int entry) {
    if (entry == -1) {
        return "null";
    }

    return entries[entry].message;
}

std::string Gui::activate(const sf::Vector2f mousePos) {
    int entry = getEntry(mousePos);
    return activate(entry);
}

void Gui::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (!visible) {
        return;
    }

    for (auto entry : entries) {
        target.draw(entry.shape);
        target.draw(entry.text);
    }
}

Gui::Gui(sf::Vector2f dimensions, int padding, bool horizontal, GuiStyle &style,
         std::vector<std::pair<std::string, std::string> > entries)
{
    visible = false;

    this->horizontal = horizontal;
    this->style = style;
    this->dimensions = dimensions;
    this->padding = padding;

    sf::RectangleShape shape;
    shape.setSize(dimensions);
    shape.setFillColor(style.bodyColor);
    shape.setOutlineThickness(-style.borderSize);
    shape.setOutlineColor(style.borderColor);

    for (auto entry : entries) {
        sf::Text text;
        text.setString(entry.first);
        text.setFont(*style.font);
        text.setColor(style.textColor);
        text.setCharacterSize(dimensions.y - style.borderSize - padding);

        this->entries.push_back(GuiEntry(entry.second, shape, text));
    }
}
