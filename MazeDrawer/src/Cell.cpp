#include "Cell.h"

void Cell::Draw(sf::RectangleShape &shape, const sf::Vector2f &cellSize, sf::RenderTarget &target, sf::Text *txtptr, sf::Color clr)
{
    if (!selected)
        shape.setFillColor(sf::Color(68, 68, 68));
    else
        shape.setFillColor(clr);

    target.draw(shape);
    DrawText(txtptr, cellSize, target, clr);
}

void Cell::DrawText(sf::Text *txtptr, const sf::Vector2f &cellSize, sf::RenderTarget &target, sf::Color clr)
{
    if (txtptr)
    {
        if (clr == sf::Color::Black)
            txtptr->setFillColor(sf::Color::White);
        else
            txtptr->setFillColor(sf::Color::Black);
        txtptr->setCharacterSize(cellSize.x - 5);
        txtptr->setPosition(position.x * cellSize.x, position.y * cellSize.y);
        txtptr->setString(std::to_string(value));
        target.draw(*txtptr);
    }
}