#include "Application.h"
#include "Functions.h"
#include "Cell.h"

sf::RenderWindow *Application::m_Window = nullptr;

sf::RenderWindow *Application::GetWindow()
{
    return m_Window;
}

Application::Application()
{
    auto fullScreen = sf::VideoMode::getFullscreenModes()[0];
    m_Window = new sf::RenderWindow(fullScreen, "Maze Drawer Extended", sf::Style::Default);
    ImGui::SFML::Init(*m_Window, true);
    m_Window->setFramerateLimit(60);

    m_View = m_Window->getView();
    m_View.setCenter(m_View.getSize().x / 2.f, m_View.getSize().y / 2.0f);

    m_Font.loadFromFile("rsc/ProggyClean.ttf");
    m_Text.setFont(m_Font);

    m_Cells.resize(40 * 40);
    m_NumberOfCells = 40 * 40;

    m_Cateogries.emplace_back("DEFAULT", -1);

    m_ClrMap["DEFAULT"] = sf::Color(25, 165, 25, 255);

    m_Scale = {32.0f, 32.0f};
}

Application::~Application()
{
    ImGui::SFML::Shutdown();
    delete m_Window;
    m_Window = nullptr;
}

void Application::Run()
{

    while (m_Window && m_Window->isOpen())
    {
        auto dt = m_Clk.restart();
        ProcessEevnts();
        Update(dt);

        ImGui::SFML::Update(*m_Window, dt);
        StartImGuiFrame();
        ImGuiLayer();

        Render();
    }
}
void Application::ProcessEevnts()
{
    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed)
            m_Window->close();
        if (event.type == sf::Event::Resized)
        {
            m_View.setSize(m_Window->getSize().x, m_Window->getSize().y);
            m_View.setCenter(m_Window->getSize().x / 2.f, m_Window->getSize().y / 2.0f);
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            m_Drawing = false;
            m_Removing = false;
            m_LastMousePos = {-1, -1};
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            m_Drawing = true;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            m_Removing = true;
    }
}

void Application::Update(const sf::Time &time)
{
    auto [mouseX, mouseY] = GetMousePosition(m_Window);
    int32_t x = mouseX / m_Scale.x;
    int32_t y = mouseY / m_Scale.y;

    if (m_Drawing || m_Removing)
    {
        sf::FloatRect r1;
        r1.left = mouseX;
        r1.top = mouseY;
        r1.height = 2.0f;
        r1.width = 2.0f;
        int32_t maxIndex_X = std::min(m_NumberOfCells, int32_t(m_View.getSize().x / 32.0f));
        int32_t maxIndex_Y = std::min(m_NumberOfCells, int32_t(m_View.getSize().y / 32.0f));
        if (!IntersectsWithImGuiWindows(r1))
        {
            if (x >= 0 && y >= 0 && x <= maxIndex_X && y <= maxIndex_Y)
            {
                std::list<sf::Vector2i> path;
                if (m_LastMousePos.x != -1)
                    path = Traverse(sf::Vector2i(x, y), m_LastMousePos);
                else
                    path.push_back(sf::Vector2i(x, y));

                for (auto &v : path)
                {
                    int32_t index = v.x * m_NumberOfCells + v.y;
                    m_Cells[index].selected = (m_Drawing && !m_Removing);
                }
            }
        }

        m_LastMousePos = sf::Vector2i(x, y);
    }
}

void Application::ImGuiLayer()
{
    auto [x, y] = GetMousePositionInt(m_Window, m_Scale);
    if (CreateImGuiWindow("TestWindow"))
    {
        ImGui::TextUnformatted("Till which index do you want to save?");
        ImGui::TextUnformatted("x:");
        ImGui::SameLine();
        ImGui::SliderInt("###1", &m_SaveUntil.x, 0, m_NumberOfCells - 1);
        ImGui::TextUnformatted("y:");
        ImGui::SameLine();
        ImGui::SliderInt("###2", &m_SaveUntil.y, 0, m_NumberOfCells - 1);
        ImGui::Text("x: %d, y: %d", x, y);
        ImGui::TextUnformatted("Click on Save to save the data");
        ImGui::SameLine();
        if (ImGui::Button("Save Data"))
        {
            SaveData(m_SaveUntil, m_NumberOfCells, m_Cells);
        }
        EndImGuiWindow();
    }
}

void Application::Render()
{
    int32_t maxIndex_X = std::min(m_NumberOfCells, int32_t(m_View.getSize().x / 32.0f));
    int32_t maxIndex_Y = std::min(m_NumberOfCells, int32_t(m_View.getSize().y / 32.0f));
    m_Window->clear(sf::Color(136, 135, 85, 255));
    m_Window->setView(m_View);
    sf::RectangleShape shape(m_Scale + sf::Vector2f(-1.0f, -1.0f));
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color(245, 245, 245));

    sf::Text *txtPtr = (m_ShowText) ? &m_Text : nullptr;

    for (int32_t i = 0; i <= maxIndex_X; i++)
    {
        for (int32_t j = 0; j <= maxIndex_Y; j++)
        {
            int32_t index = i * m_NumberOfCells + j;

            sf::Color clr = sf::Color(68, 68, 68);
            if (m_Cells[index].selected)
                clr = GetClrFromMap(m_Cells[index].category);
            m_Cells[index].Draw(shape, m_Scale, *m_Window, txtPtr, clr);
        }
    }

    ImGui::SFML::Render(*m_Window);
    m_Window->display();
}

bool Application::IntersectsWithImGuiWindows(const sf::FloatRect &rect)
{
    for (auto &r : m_ImGuiWindowRects)
    {
        if (rect.intersects(r))
            return true;
    }
    return false;
}

sf::Color Application::GetClrFromMap(const std::string &category)
{
    auto it = m_ClrMap.begin();
    if ((it = m_ClrMap.find(category)) == m_ClrMap.end())
        return sf::Color::Blue;

    return it->second;
}

void Application::DrawTable()
{
    ImGui::BeginChild("Category Table");
    HelpMarker("The category with the value 0 is a one that will be atteded incrementally in the output file.\n"
               "i.e. it will printed in the output file 1 2 3 4 5 and so on\n");
    ImGui::Columns(2, "Available Categories");
    for (uint32_t i = 0; i < m_Cateogries.size(); i++)
    {
        ImGui::Text("%s", m_Cateogries[i].first.c_str());
        ImGui::NextColumn();
        ImGui::Text("%ld", m_Cateogries[i].second);
        ImGui::NextColumn();
        ImGui::Separator();
    }
    ImGui::EndChild();
}

bool Application::CreateImGuiWindow(std::string_view title, bool *p_open, ImGuiWindowFlags windowFlags)
{
    return ImGui::Begin(title.data(), p_open, windowFlags);
}

void Application::EndImGuiWindow()
{
    ImVec2 size = ImGui::GetWindowSize();
    ImVec2 position = ImGui::GetWindowPos();

    m_ImGuiWindowRects.emplace_back(position.x, position.y, size.x, size.y);
    ImGui::End();
}

void Application::CreatePopUp(bool &opened, std::string_view title, std::string_view message)
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    sf::Vector2u size = {sf::VideoMode::getFullscreenModes()[0].width, sf::VideoMode::getFullscreenModes()[0].height};

    ImVec2 windowSize = ImVec2(size.x / 3, size.y / 6);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(size.x / 2.0f - windowSize.x, size.y / 2.0f));

    if (CreateImGuiWindow(title.data(), &opened, flags))
    {
        ImGui::PushTextWrapPos(ImGui::GetWindowWidth());
        ImGui::TextUnformatted(message.data());
        ImGui::PopTextWrapPos();
        ImGui::NewLine();
        ImGui::SetCursorPosX(windowSize.x / 2.0f - 20.0f);
        if (ImGui::Button("Ok"))
        {
            opened = false;
        }
        EndImGuiWindow();
    }
}

void Application::StartImGuiFrame()
{
    m_ImGuiWindowRects.clear();
}
