

#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <string_view>
#include <SFML/Graphics.hpp>
#include "imgui.h"

using Point = sf::Vector2i;
struct Cell;

void SaveData(sf::Vector2i saveUntil, int32_t bound, const std::vector<Cell> &cells);

std::list<Point> Traverse(Point start, Point end);

std::list<Point> High(Point start, Point end);

std::list<Point> Low(Point start, Point end);

void HelpMarker(std::string_view desc);

void CreatePopUp(bool &opened, std::string_view title = "PopupMessage", std::string_view message = "Some Message");

sf::Vector2i GetMousePositionInt(sf::RenderWindow *window, const sf::Vector2f &scale);

sf::Vector2f GetMousePosition(sf::RenderWindow *window);
