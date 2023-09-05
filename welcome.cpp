#include "welcome.h"
#include "board.h"
#include <cctype> // For std::isalpha
#include <iostream>

WelcomeWindow::WelcomeWindow()
        : m_window(sf::VideoMode(800, 612), "Minesweeper Game", sf::Style::Close),
          m_gameStarted(false),
          m_gameShouldStart(false),
          m_cursor('|'){}

void WelcomeWindow::setText(sf::Text &text, float x, float y)
{
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

bool WelcomeWindow::runWelcomeWindow()
{
    m_window.clear(sf::Color::Blue);
    if (!m_font.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/font.ttf"))
    {
        std::cerr << "Error loading font file" << std::endl;
    }

    m_welcomeText.setString("WELCOME TO MINESWEEPER!");
    m_welcomeText.setFont(m_font);
    m_welcomeText.setCharacterSize(24);
    m_welcomeText.setFillColor(sf::Color::White);
    m_welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(m_welcomeText, m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f - 150);

    m_prompt.setString("Enter your name:");
    m_prompt.setFont(m_font);
    m_prompt.setCharacterSize(20);
    m_prompt.setFillColor(sf::Color::White);
    m_prompt.setStyle(sf::Text::Bold);
    setText(m_prompt, m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f - 75);

    m_text.setFont(m_font);
    m_text.setCharacterSize(18);
    m_text.setFillColor(sf::Color::Yellow);
    m_text.setStyle(sf::Text::Bold);

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close(); // Close the window and stop the program
                return false;
            } else if (event.type == sf::Event::TextEntered)
            {
                if (std::isalpha(static_cast<char>(event.text.unicode)))
                {
                    if (m_playerName.size() < 10)
                    {
                        m_playerName += static_cast<char>(event.text.unicode);
                    }
                } else if (event.text.unicode == 8) // Backspace
                {
                    if (!m_playerName.empty()) {
                        m_playerName.pop_back();
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                if (!m_playerName.empty() && !m_gameStarted)
                {
                    m_gameShouldStart = true;
                    m_window.close();
                }
            }
        }
        m_text.setString(m_playerName + m_cursor);
        setText(m_text, m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f - 45);
        m_window.clear(sf::Color::Blue);
        m_window.draw(m_welcomeText);
        m_window.draw(m_prompt);
        m_window.draw(m_text);
        m_window.display();
    }

    return m_gameShouldStart;
}
