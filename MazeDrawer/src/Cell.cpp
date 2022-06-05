#include "Cell.h"

void Cell::Draw(sf::RectangleShape &shape, const sf::Vector2f &cellSize, sf::RenderTarget &target, sf::Text *txtptr, sf::Color clr)
{
    if (!selected)
        shape.setFillColor(sf::Color(68, 68, 68));
    else
        shape.setFillColor(clr);

    target.draw(shape);
    DrawText(txtptr, shape.getPosition(), cellSize, target, clr);
}

void Cell::DrawText(sf::Text *txtptr, const sf::Vector2f &position, const sf::Vector2f &cellSize, sf::RenderTarget &target, sf::Color clr)
{
    if (txtptr)
    {
        if (clr == sf::Color::Black)
            txtptr->setFillColor(sf::Color::White);
        else
            txtptr->setFillColor(sf::Color::Black);
        txtptr->setCharacterSize(cellSize.x - 10);
        txtptr->setPosition(position);
        txtptr->setString(std::to_string(value));
        target.draw(*txtptr);
    }
}