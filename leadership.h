#ifndef LEADERSHIP_H
#define LEADERSHIP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Leadership {
public:
    Leadership();

    void showLeadership(sf::RenderWindow& window);
    void updateLeadership(const std::string& playerName, const std::string& playerTime);

private:
    struct Player {
        std::string time;
        std::string name;
    };

    std::vector<Player> m_players;
    sf::Font m_font;

    void loadLeadership();
    void saveLeadership();
};

#endif // LEADERSHIP_H

