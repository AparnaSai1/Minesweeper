#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "leadership.h"

class Board {
public:
    Board();
    bool runGameWindow();

private:
    struct Tile {
        bool isMine;
        bool isRevealed;
        bool isFlagged;
        int adjacentMines;
    };
    Leadership m_leadership;
    bool readConfigFile(const std::string &filename);
    void LoadTextures();
    void initializeButtons();
    bool handleClickingLeader(const sf::Vector2i& mousePosition);
    bool handleClickingPause(const sf::Vector2i& mousePosition);
    void drawUIComponents();
    void drawGameGrid();
    void initializeGameGrid();
    void runLeadershipButton();
    void runPausePlayButton();
    void reset();
    bool isInsideSmileyArea(const sf::Vector2i& mousePosition) const;
    int getMinutesDigitValue(int position) const;
    int getSecondsDigitValue(int position) const;
    void handleLeftTileClick(int row, int col);
    void handleRightTileClick(int row, int col);
    void handleTileClick(const sf::Vector2i& mousePosition);
    void gameOver(bool isWin);
    void revealTile(int row, int col);
    void drawTimerDigits();
    void drawDigit(int posX, int posY, int digitValue);
    void checkAdjacentTiles(int row, int col);
    bool checkWinCondition();
    sf::RenderWindow m_window;
    int m_numColumns;
    int m_numRows;
    int m_numMines;

    std::vector<std::vector<Tile>> m_gameGrid;

    sf::Texture m_tileHiddenTexture;
    sf::Texture m_tileRevealedTexture;
    sf::Texture m_mineTexture;
    sf::Texture m_flagTexture;
    std::vector<sf::Texture> m_numberTextures;

    sf::Texture m_happyFaceTexture;
    sf::Texture m_debugButtonTexture;
    sf::Texture m_pauseButtonTexture;
    sf::Texture m_playButtonTexture;
    sf::Texture m_leaderboardButtonTexture;
    sf::Texture m_winfaceTexture;
    sf::Texture m_losefaceTexture;
    sf::Texture m_digitsTexture;

    sf::Sprite m_happyFaceButton;
    sf::Sprite m_debugButton;
    sf::Sprite m_pausePlayButton;
    sf::Sprite m_playButton;
    sf::Sprite m_leaderboardButton;
    sf::Sprite m_winfaceButton;
    sf::Sprite m_losefaceButton;
    sf::Sprite m_digitsButton;
    sf::Sprite m_mine;
    sf::Sprite m_flag;

    sf::Font m_font;
    sf::Text m_counterText;
    sf::Text m_timerText;

    sf::Clock m_timerClock;
    int m_elapsedTime = 0; // Elapsed time in seconds
    int m_minutes = 0;
    int m_seconds = 0;

    bool m_debugMode;
    bool m_gamePaused;
    bool m_gameOver;
    bool m_gameWon;

    static constexpr int TILE_SIZE = 32;
    static constexpr int BUTTON_SPACE = 50;

};

#endif // BOARD_H
