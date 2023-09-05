#include "leadership.h"
#include <fstream>
#include <algorithm>

Leadership::Leadership()
{
    if (!m_font.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/font.ttf")) {
        // Handle font loading error
    }
    loadLeadership();
}

void Leadership::loadLeadership()
{
    std::ifstream file("/Users/aparnanimmagadda/CLionProjects/Project3/leaderboard.txt");
    if (!file.is_open()) {
        // Handle file loading error
    }

    m_players.clear();
    Player player;
    while (file >> player.time >> player.name) {
        m_players.push_back(player);
    }

    file.close();
}

void Leadership::saveLeadership()
{
    std::ofstream file("/Users/aparnanimmagadda/CLionProjects/Project3/leaderboard.txt");
    if (!file.is_open()) {
        // Handle file saving error
    }

    for (const Player& player : m_players) {
        file << player.time << " " << player.name << "\n";
    }

    file.close();
}



void Leadership::showLeadership(sf::RenderWindow& window)
{
    sf::Text titleText("LEADERBOARD", m_font, 20);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setOrigin(titleText.getLocalBounds().width / 2.0f, titleText.getLocalBounds().height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 120);


    std::string leadershipText;
    for (size_t i = 0; i < m_players.size(); i++)
    {
        leadershipText += std::to_string(i + 1) + ".\t" + m_players[i].time + "\t" + m_players[i].name + "\n\n";
    }


    sf::Text contentText(leadershipText, m_font, 18);
    contentText.setFillColor(sf::Color::White);
    contentText.setStyle(sf::Text::Bold);
    contentText.setOrigin(contentText.getLocalBounds().width / 2.0f, contentText.getLocalBounds().height / 2.0f);
    contentText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20);

    sf::RectangleShape background(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
    background.setFillColor(sf::Color::Blue);
    background.setOrigin(background.getSize() / 2.0f);
    background.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    window.clear();
    window.draw(background);
    window.draw(titleText);
    window.draw(contentText);
    window.display();

    // You can add event handling here to close the leadership window
}

void Leadership::updateLeadership(const std::string& playerName, const std::string& playerTime)
{
    m_players.push_back({playerTime, playerName});
    std::sort(m_players.begin(), m_players.end(), [](const Player& a, const Player& b) {
        return a.time < b.time;
    });

    if (m_players.size() > 5) {
        m_players.pop_back();
    }

    saveLeadership();
}

