#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

struct Cell
{
    std::string category = "DEFAULT";
    sf::Vector2i position{};
    int32_t value = -1;
    bool selected = false;

    void Draw(sf::RectangleShape &shape, const sf::Vector2f &cellSize, sf::RenderTarget &target, sf::Text *txtptr = nullptr, sf::Color clr = sf::Color(68, 68, 68));

private:
    void DrawText(sf::Text *txtptr, const sf::Vector2f &cellSize, sf::RenderTarget &target, sf::Color clr);
};
