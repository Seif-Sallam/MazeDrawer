#include "Functions.h"
#include "Cell.h"
#include <list>

void PrintNegativeNumber(int value, std::ofstream &outputFile, int32_t numberOfPaddings);

std::list<Point> Low(Point start, Point end)
{
    std::list<Point> output;
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

std::list<Point> High(Point start, Point end)
{

    std::list<Point> output;
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

std::list<Point> Traverse(Point start, Point end)
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

void SaveData(sf::Vector2i saveUntil, int32_t mazeSize, const std::vector<Cell> &cells)
{
    int32_t saveTillX = saveUntil.x;
    int32_t saveTillY = saveUntil.y;
    std::ofstream output("output.txt");
    int counter = 0;
    int c = 0;
    for (int i = 0; i <= saveTillX; i++)
    {
        for (int j = 0; j <= saveTillY; j++)
        {
            int index = i * mazeSize + j;
            if (!cells[index].selected)
                c++;
        }
    }
    int numOfPaddings = 0;
    numOfPaddings = numOfPaddings + (c >= 10) + (c >= 100) + (c >= 1000);

    for (int i = 0; i <= saveTillX; i++)
    {
        for (int j = 0; j <= saveTillY; j++)
        {
            int index = i * mazeSize + j;
            if (cells[index].selected)
            {
                PrintNegativeNumber(cells[index].value, output, numOfPaddings);
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

void HelpMarker(std::string_view desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc.data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

sf::Vector2i GetMousePositionInt(sf::RenderWindow *window, const sf::Vector2f &scale)
{
    auto [mouseX, mouseY] = GetMousePosition(window);
    int32_t x = mouseX / scale.x;
    int32_t y = mouseY / scale.y;
    return {x, y};
}

sf::Vector2f GetMousePosition(sf::RenderWindow *window)
{
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}

void PrintNegativeNumber(int value, std::ofstream &outputFile, int32_t padding)
{
    uint32_t absValue = std::abs(value);
    int c = (absValue >= 10) + (absValue >= 100) + (absValue >= 1000);

    outputFile << '-';
    for (int i = 0; i < padding - c - 1; i++)
        outputFile << '0';
    outputFile << value;
}