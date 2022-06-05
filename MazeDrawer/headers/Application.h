#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "SFML/Graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <list>
#include <string_view>

struct Cell;

class Application
{
public:
    Application();
    void Run();
    static sf::RenderWindow *GetWindow();
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

private:
    static sf::RenderWindow *m_Window;
    std::list<sf::FloatRect> m_ImGuiWindowRects;
    std::unordered_map<std::string, sf::Color> m_ClrMap;
    std::vector<std::pair<std::string, int32_t>> m_Cateogries;

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
};
