#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "SFML/Graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <list>
#include <string_view>
#include <map>
#include <sstream>

struct Cell;

class Application
{
public:
    Application();
    void Run();
    ~Application();

private:
    void ProcessEevnts();
    void Update(const sf::Time &time);
    void ImGuiLayer();
    void Render();

    bool IntersectsWithImGuiWindows(const sf::FloatRect &rect);
    sf::Color GetClrFromMap(const std::string &category);
    void DrawTable();

    bool CreateImGuiWindow(std::string_view title, bool *p_open = nullptr, ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None);
    void CreatePopUp(bool &opened, std::string_view title = "PopupMessage", std::string_view message = "Some Message");
    void EndImGuiWindow();
    void StartImGuiFrame();
    void RewriteCells(int x = 1, int y = 1);

    void LoadMap(const std::vector<std::vector<int32_t>> &map, const std::map<int32_t, std::string> catMap);
    void LoadMap(const std::string &filePath);

private:
    sf::RenderWindow *m_Window;
    std::list<sf::FloatRect> m_ImGuiWindowRects;
    std::unordered_map<std::string, std::pair<sf::Color, int32_t>> m_Categories;

    std::vector<Cell> m_Cells;
    sf::Vector2i m_LastMousePos = {-1, -1};
    sf::Vector2f m_Scale;
    sf::Clock m_Clk;
    bool m_Removing, m_Drawing;
    sf::View m_View;
    int32_t m_NumberOfCells;
    sf::Text m_Text;
    sf::Font m_Font;
    bool m_ShowText;
    sf::Vector2i m_SaveUntil;
    std::string m_CurrentCat = "DEFAULT";
    char m_FileName[1024] = "output";
};
