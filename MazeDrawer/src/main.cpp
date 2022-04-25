#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <fstream>

const int Bound = 50;
sf::RectangleShape shapes[Bound * Bound];
bool selected[Bound * Bound];
int saveTillX = Bound - 1, saveTillY = Bound - 1;
using Point = sf::Vector2i;

std::vector<Point> Traverse(Point start, Point end);

void SaveData();

int main()
{
    sf::Vector2i lastMousePos = {-1, -1};
    for (int i = 0; i < Bound; i++)
    {
        for (int j = 0; j < Bound; j++)
        {
            int index = i * Bound + j;
            shapes[index].setFillColor(sf::Color(168, 168, 168, 255));
            shapes[index].setSize(sf::Vector2f(32.0f, 32.0f));
            shapes[index].setPosition(sf::Vector2f(32.0f * i, 32.0f * j));
            shapes[index].setOutlineThickness(1.0f);
            shapes[index].setOutlineColor(sf::Color::Black);
            selected[index] = false;
        }
    }
    sf::View view;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Maze Drawer", sf::Style::Default);
    view = window.getView();
    view.setCenter(view.getSize().x / 2.f, view.getSize().y / 2.0f);
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window, true);
    sf::Clock clk;
    ImVec2 windowPos;
    ImVec2 windowSize;
    bool drawing = false;
    bool removing = false;
    while (window.isOpen())
    {
        sf::Time dt = clk.restart();

        sf::Event event;
        int maxIndex_X = std::min(Bound, int(view.getSize().x / 32.0f));
        int maxIndex_Y = std::min(Bound, int(view.getSize().y / 32.0f));

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        int x = mousePos.x / 32.0f;
        int y = mousePos.y / 32.0f;
        ImGui::SFML::Update(window, dt);

        ImGui::Begin("Test Window");
        {
            windowPos = ImGui::GetWindowPos();
            windowSize = ImGui::GetWindowSize();
            ImGui::TextUnformatted("Till which index do you want to save?");
            ImGui::TextUnformatted("x:");
            ImGui::SameLine();
            ImGui::SliderInt("###1", &saveTillX, 0, Bound - 1);
            ImGui::TextUnformatted("y:");
            ImGui::SameLine();
            ImGui::SliderInt("###2", &saveTillY, 0, Bound - 1);
            ImGui::Text("x: %d, y: %d", x, y);
            ImGui::TextUnformatted("Click on Save to save the data");
            ImGui::SameLine();
            if (ImGui::Button("Save Data"))
            {
                SaveData();
            }
        }
        ImGui::End();

        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                view.setSize(window.getSize().x, window.getSize().y);
                view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.0f);
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                drawing = false;
                removing = false;
                lastMousePos = {-1, -1};
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                drawing = true;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                removing = true;
            }
        }

        if (drawing || removing)
        {
            sf::FloatRect r1, r2;
            r1.left = mousePos.x;
            r1.top = mousePos.y;
            r1.height = 2.0f;
            r1.width = 2.0f;
            r2.left = windowPos.x;
            r2.top = windowPos.y;
            r2.width = windowSize.x;
            r2.height = windowSize.y;
            if (r1.intersects(r2) == false)
            {
                if (x >= 0 && y >= 0 && x <= maxIndex_X && y <= maxIndex_Y)
                {
                    std::vector<sf::Vector2i> path;
                    if (lastMousePos.x != -1)
                    {
                        path = Traverse(sf::Vector2i(x, y), lastMousePos);
                    }
                    else
                    {
                        path.push_back(sf::Vector2i(x, y));
                    }

                    for (int i = 0; i < path.size(); i++)
                    {
                        int index = path[i].x * Bound + path[i].y;
                        selected[index] = (drawing && !removing);
                    }
                }
            }

            lastMousePos = sf::Vector2i(x, y);
        }

        window.clear(sf::Color(136, 135, 85, 255));
        window.setView(view);
        for (int i = 0; i <= maxIndex_X; i++)
        {
            for (int j = 0; j <= maxIndex_Y; j++)
            {
                int index = i * Bound + j;
                if (selected[index])
                    shapes[index].setFillColor(sf::Color(25, 165, 25, 255));
                else
                    shapes[index].setFillColor(sf::Color(168, 168, 168, 255));

                window.draw(shapes[index]);
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
void SaveData()
{
    std::ofstream output("output.txt");
    int counter = 0;
    int c = 0;
    for (int i = 0; i <= saveTillX; i++)
    {
        for (int j = 0; j <= saveTillY; j++)
        {
            int index = i * Bound + j;
            if (!selected[index])
                c++;
        }
    }
    int numOfPaddings = 0;
    numOfPaddings = numOfPaddings + (c >= 10) + (c >= 100) + (c >= 1000);

    for (int i = 0; i <= saveTillX; i++)
    {
        for (int j = 0; j <= saveTillY; j++)
        {
            int index = i * Bound + j;
            if (selected[index])
            {
                output << "-";
                for (int k = 0; k < numOfPaddings - 1; k++)
                    output << "0";

                output << "1";
            }
            else
            {
                if (counter < 10)
                    for (int k = 0; k < numOfPaddings; k++)
                        output << "0";

                else if (counter < 100)
                    for (int k = 0; k < numOfPaddings - 1; k++)
                        output << "0";
                else if (counter < 1000)
                    for (int k = 0; k < numOfPaddings - 2; k++)
                        output << "0";

                output << counter;
                counter++;
            }
            if (j != saveTillY)
                output << " ";
        }
        output << '\n';
    }
    output.close();
}

std::vector<Point> Low(Point start, Point end)
{
    std::vector<Point> output;
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = start.y;
    for (int x = start.x; x <= end.x; x++)
    {
        output.push_back({x, y});
        if (D > 0)
        {
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else
        {
            D = D + 2 * dy;
        }
    }
    return output;
}

std::vector<Point> High(Point start, Point end)
{

    std::vector<Point> output;
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int xi = 1;
    if (dy < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = start.x;
    for (int y = start.y; y <= end.y; y++)
    {
        output.push_back({x, y});
        if (D > 0)
        {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
        {
            D = D + 2 * dx;
        }
    }
    return output;
}

std::vector<Point> Traverse(Point start, Point end)
{
    if (std::abs(end.y - start.y) < std::abs(end.x - start.x))
    {
        if (start.x > end.x)
            return Low(end, start);
        else
            return Low(start, end);
    }
    else
    {
        if (start.y > end.y)
            return High(end, start);
        else
            return High(start, end);
    }
}
