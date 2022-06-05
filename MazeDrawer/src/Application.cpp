#include "Application.h"
#include "Functions.h"
#include "Cell.h"
#include <cmath>
int count = 0;

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
    m_Scale = {32.0f, 32.0f};

    m_NumberOfCells = std::ceil(m_View.getSize().x / m_Scale.x);
    m_Cells.resize(m_NumberOfCells * m_NumberOfCells);

    m_ShowText = true;
    m_Categories["DEFAULT"] = {sf::Color(25, 165, 25, 255), -1};

    m_SaveUntil = {42, 22};
    int v = 0;
    for (int i = 0; i < m_NumberOfCells; i++)
        for (int j = 0; j < m_NumberOfCells; j++)
        {
            int32_t index = j * m_NumberOfCells + i;
            m_Cells[index].value = v;
            v++;
        }
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

                int count = 0;
                for (auto &v : path)
                {
                    count++;
                    int32_t index = v.x * m_NumberOfCells + v.y;
                    m_Cells[index].selected = (m_Drawing && !m_Removing);
                    if (m_Cells[index].selected)
                    {
                        m_Cells[index].category = m_CurrentCat;
                        m_Cells[index].value = m_Categories[m_Cells[index].category].second; // the value of the the cat
                    }
                }
                // if (count >= 1)
                RewriteCells(m_SaveUntil.x, m_SaveUntil.y);
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
        static int oldValueX, oldValueY;
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
        if (oldValueX != m_SaveUntil.x || oldValueY != m_SaveUntil.y)
        {
            RewriteCells(m_SaveUntil.x, m_SaveUntil.y);
            oldValueX = m_SaveUntil.x;
            oldValueY = m_SaveUntil.y;
        }
        ImGui::Checkbox("Show Numbers", &m_ShowText);
        if (ImGui::Button("Clear All"))
        {
            for (int i = 0; i <= m_SaveUntil.y; i++)
            {
                for (int j = 0; j <= m_SaveUntil.x; j++)
                {
                    int index = j * m_NumberOfCells + i;
                    m_Cells[index].selected = false;
                }
            }
            RewriteCells(m_SaveUntil.x, m_SaveUntil.y);
        }

        DrawTable();
        EndImGuiWindow();
    }
}

void Application::DrawTable()
{
    ImGui::Separator();
    static char inputName[1024] = "";
    static int value = 1;
    static float clr[3] = {0.0, 0.0, 0.0};
    ImGui::InputText("Title", inputName, sizeof(inputName));
    ImGui::SliderInt("Value", &value, -100, 100);
    ImGui::ColorEdit3("Color", clr);
    if (ImGui::Button("Add Category"))
    {
        bool flag = false;
        for (auto &cat : m_Categories)
        {
            if (cat.first == inputName || cat.second.second == value)
            {
                flag = true;
                break;
            }
        }
        if (flag == true)
        {
            // Pop up (TO BE CREATED LATER)
        }
        else
        {
            m_Categories[inputName] = {sf::Color(clr[0] * 255, clr[1] * 255, clr[2] * 255, 255), value};
            m_CurrentCat = inputName;
        }
        value++;
        clr[0] = 0;
        clr[1] = 0;
        clr[2] = 0;
    }

    ImGui::BeginChild("Category Table");
    HelpMarker("The category with the value 0 is a one that will be atteded incrementally in the output file.\n"
               "i.e. it will printed in the output file 1 2 3 4 5 and so on\n");
    ImGui::Columns(3, "Available Categories");

    static int selection = 0;
    int ind = 0;
    for (auto &cat : m_Categories)
    {
        ImGui::Text("%s", cat.first.c_str());
        ImGui::NextColumn();
        ImGui::Text("%d", cat.second.second);
        ImGui::NextColumn();
        if (ImGui::RadioButton(("###" + std::to_string(ind)).c_str(), &selection, ind++))
        {
            m_CurrentCat = cat.first;
        }
        ImGui::NextColumn();
        ImGui::Separator();
    }
    ImGui::EndChild();
}

void Application::Render()
{
    int32_t maxIndex_X = std::min(m_SaveUntil.x, int32_t(m_View.getSize().x / 32.0f));
    int32_t maxIndex_Y = std::min(m_SaveUntil.y, int32_t(m_View.getSize().y / 32.0f));
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
            shape.setPosition(i * m_Scale.x, j * m_Scale.y);
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
    auto it = m_Categories.begin();
    if ((it = m_Categories.find(category)) == m_Categories.end())
        return sf::Color::Blue;

    return it->second.first;
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
void Application::RewriteCells(int x, int y)
{
    int v = 0;
    for (int i = 0; i <= y; i++)
        for (int j = 0; j <= x; j++)
        {
            int32_t index = j * m_NumberOfCells + i;
            if (m_Cells[index].selected == false)
            {
                m_Cells[index].value = v;
                v++;
            }
        }
}