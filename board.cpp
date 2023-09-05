#include "board.h"
#include "leadership.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <SFML/System/Clock.hpp>


void Board::LoadTextures()
{
    if (!m_tileHiddenTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/tile_hidden.png") ||
        !m_tileRevealedTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/tile_revealed.png") ||
        !m_mineTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/mine.png") ||
        !m_flagTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/flag.png") ||
        !m_happyFaceTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/face_happy.png") ||
        !m_debugButtonTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/debug.png") ||
        !m_pauseButtonTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/pause.png") ||
        !m_playButtonTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/play.png")||
        !m_winfaceTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/face_win.png")||
        !m_losefaceTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/face_lose.png")||
        !m_leaderboardButtonTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/leaderboard.png")||
        !m_digitsTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/digits.png"))

    {
        std::cerr << "Error loading textures." << std::endl;
    }

    // Set textures for UI components
    m_happyFaceButton.setTexture(m_happyFaceTexture);
    m_debugButton.setTexture(m_debugButtonTexture);
    m_pausePlayButton.setTexture(m_pauseButtonTexture);
    m_playButton.setTexture(m_playButtonTexture);
    m_leaderboardButton.setTexture(m_leaderboardButtonTexture);
    m_mine.setTexture(m_mineTexture);
    m_winfaceButton.setTexture(m_winfaceTexture);
    m_losefaceButton.setTexture(m_losefaceTexture);
    m_flag.setTexture(m_flagTexture);
    m_digitsButton.setTexture(m_digitsTexture);

    for (int i = 1; i <= 8; ++i)
    {
        sf::Texture numberTexture;
        if (!numberTexture.loadFromFile("/Users/aparnanimmagadda/CLionProjects/Project3/images/number_" + std::to_string(i) + ".png"))
        {
            std::cerr << "Error loading number texture." << std::endl;
        }
        m_numberTextures.push_back(numberTexture);
    }
}

Board::Board()
        : m_window(sf::VideoMode(800, 612), "Minesweeper Game", sf::Style::Close),
          m_numColumns(0),
          m_numRows(0),
          m_numMines(0),
          m_debugMode(false),
          m_gamePaused(false),
          m_gameOver(false),
          m_gameWon(false)

          {
           LoadTextures();
          }

bool Board::readConfigFile(const std::string &filename)
{
    std::ifstream configFile("/Users/aparnanimmagadda/CLionProjects/Project3/board_config.cfg");
    if (!configFile.is_open()) {
        std::cerr << "Error opening config file." << std::endl;
        return false;
    }

    configFile >> m_numColumns >> m_numRows >> m_numMines;
    configFile.close();

    if (m_numColumns < 22 || m_numRows < 16 || m_numMines <= 0 || m_numMines > m_numColumns * m_numRows) {
        std::cerr << "Invalid config values." << std::endl;
        return false;
    }

    return true;
}

void Board::drawDigit(int posX, int posY, int digitValue) {
    if (digitValue >= 0 && digitValue <= 9) {
        int digitWidth = 21;
        int digitHeight = 32;
        sf::IntRect textureRect(digitValue * digitWidth, 0, digitWidth, digitHeight);

        sf::Sprite digitSprite;
        digitSprite.setTexture(m_digitsTexture);
        digitSprite.setTextureRect(textureRect);
        digitSprite.setPosition(posX, posY);
        m_window.draw(digitSprite);
    }
}

void Board::drawTimerDigits() {
    int posXMinutes = (m_numColumns * TILE_SIZE) - 97;
    int posXSeconds = (m_numColumns * TILE_SIZE) - 54;
    int posY = TILE_SIZE * (m_numRows + 0.5f) + 16;

    int tensMinutes = m_minutes / 10;
    int onesMinutes = m_minutes % 10;
    int tensSeconds = m_seconds / 10;
    int onesSeconds = m_seconds % 10;

    drawDigit(posXMinutes, posY, tensMinutes);
    drawDigit(posXMinutes + 21, posY, onesMinutes);
    drawDigit(posXSeconds, posY, tensSeconds);
    drawDigit(posXSeconds + 21, posY, onesSeconds);
}


void Board::revealTile(int row, int col)
{
    if (row < 0 || row >= m_numRows || col < 0 || col >= m_numColumns) {
        return;
    }
    if (m_gameGrid[row][col].isRevealed || m_gameGrid[row][col].isFlagged) {
        return;
    }

    m_gameGrid[row][col].isRevealed = true;

    if (m_gameGrid[row][col].isMine) {
        std::cout << "You revealed a mine! Game over." << std::endl;
        exit(0); // Exit the program
    } else if (m_gameGrid[row][col].adjacentMines == 0) {
        // If there are no adjacent mines, reveal neighboring tiles recursively
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                revealTile(row + i, col + j);
            }
        }
    }
}

bool Board::handleClickingLeader(const sf::Vector2i& mousePosition)
{
    sf::FloatRect buttonRect((m_numColumns * TILE_SIZE) - 176, TILE_SIZE * (m_numRows + 0.5f), m_leaderboardButton.getGlobalBounds().width, m_leaderboardButton.getGlobalBounds().height);
    return buttonRect.contains(sf::Vector2f(mousePosition));
}

bool Board::handleClickingPause(const sf::Vector2i& mousePosition)
{
    sf::FloatRect buttonRect((m_numColumns * TILE_SIZE) - 240, TILE_SIZE * (m_numRows + 0.5f), m_pausePlayButton.getGlobalBounds().width, m_pausePlayButton.getGlobalBounds().height);
    return buttonRect.contains(sf::Vector2f(mousePosition));
}

void Board::gameOver(bool isWin)
{
    m_gameOver = true;

    m_timerClock.restart();
    m_elapsedTime = 0;
    m_minutes = 0;
    m_seconds = 0;

    if (isWin)
    {
        m_happyFaceButton.setTexture(m_winfaceTexture);
        m_winfaceButton.setPosition(m_happyFaceButton.getPosition());
    } else //reveal the tiles as well
    {
        m_happyFaceButton.setTexture(m_losefaceTexture);
        m_losefaceButton.setPosition(m_happyFaceButton.getPosition());

        for (int row = 0; row < m_numRows; ++row) {
            for (int col = 0; col < m_numColumns; ++col) {
                if (m_gameGrid[row][col].isMine) {
                    m_gameGrid[row][col].isRevealed = true;
                }
            }
        }
    }

}

void Board::handleLeftTileClick(int row, int col) {
    bool isControlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (!m_gameGrid[row][col].isRevealed) {
        if (isControlPressed) {
            // Place or remove a flag
            m_gameGrid[row][col].isFlagged = !m_gameGrid[row][col].isFlagged;
        } else if (!m_gameGrid[row][col].isFlagged) {
            // Regular tile revealing
            m_gameGrid[row][col].isRevealed = true;

            if (m_gameGrid[row][col].isMine) {
                gameOver(false); // The player loses
            } else if (m_gameGrid[row][col].adjacentMines == 0) {
                // Handle revealing adjacent tiles with 0 adjacent mines
                // Recursively reveal neighboring tiles
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int newRow = row + i;
                        int newCol = col + j;

                        if (newRow >= 0 && newRow < m_numRows && newCol >= 0 && newCol < m_numColumns) {
                            handleLeftTileClick(newRow, newCol);
                        }
                    }
                }
            }
        }
    }
}


void Board::handleTileClick(const sf::Vector2i& mousePosition)
{
    int clickedRow = mousePosition.y / TILE_SIZE;
    int clickedCol = mousePosition.x / TILE_SIZE;

    bool isControlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (clickedRow >= 0 && clickedRow < m_numRows && clickedCol >= 0 && clickedCol < m_numColumns) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (isControlPressed)
                {
                    m_gameGrid[clickedRow][clickedCol].isFlagged = !m_gameGrid[clickedRow][clickedCol].isFlagged;
                    m_window.draw(m_flag);
                }
            } else {
                // Handle regular left-click (revealing)
                handleLeftTileClick(clickedRow, clickedCol);
            }
        }
    }

void Board::drawUIComponents()
{

    m_happyFaceButton.setPosition(((m_numColumns / 2.0f) * TILE_SIZE) - 32, TILE_SIZE * (m_numRows + 0.5f));
    m_debugButton.setPosition((m_numColumns * TILE_SIZE) - 304, TILE_SIZE * (m_numRows + 0.5f));
    m_pausePlayButton.setPosition((m_numColumns * TILE_SIZE) - 240, TILE_SIZE * (m_numRows + 0.5f));
    m_leaderboardButton.setPosition((m_numColumns * TILE_SIZE) - 176, TILE_SIZE * (m_numRows + 0.5f));


    m_window.draw(m_happyFaceButton);
    m_window.draw(m_debugButton);
    m_window.draw(m_pausePlayButton);
    m_window.draw(m_leaderboardButton);

    drawTimerDigits();

}


void Board::runLeadershipButton() {
    if (!m_gamePaused) {
        m_gamePaused = true;

        m_window.clear(sf::Color::Black);
        drawUIComponents();
        drawGameGrid();
        m_window.display();

        m_leadership.showLeadership(m_window);

        m_gamePaused = false;
    }


    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                if (handleClickingLeader(mousePosition)) {
                    runLeadershipButton();
                } else if (handleClickingPause(mousePosition)) {
                    runPausePlayButton();
                }
            }
        }
        // Update game logic
        // ...
        // Clear and redraw window
        m_window.clear(sf::Color::White);
        drawUIComponents();
        drawGameGrid();
        m_window.display();
    }
}

void Board::runPausePlayButton()
{
    if (!m_gameOver) {
        m_gamePaused = !m_gamePaused;
        if (m_gamePaused) {
            m_pausePlayButton.setTexture(m_playButtonTexture);
        } else {
            m_pausePlayButton.setTexture(m_pauseButtonTexture);
        }

        if (m_gamePaused)
        {
            for (int row = 0; row < m_numRows; ++row){
                for (int col = 0; col < m_numColumns; ++col){
                    m_gameGrid[row][col].isRevealed = false;
                }
            }
        }
        m_window.clear(sf::Color::Black);
        drawUIComponents();
        drawGameGrid();
        m_window.display();
    }
}

void Board::drawGameGrid() {
    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numColumns; ++col) {
            sf::Texture texture;
            if (m_gameGrid[row][col].isRevealed) {
                if (m_gameGrid[row][col].isMine) {
                    texture = m_mineTexture;
                } else {
                    int adjacentMines = m_gameGrid[row][col].adjacentMines;
                    texture = m_numberTextures[adjacentMines];
                }
            } else {
                if (m_gamePaused) {
                    texture = m_tileRevealedTexture; // Use the revealed texture when paused
                } else {
                    texture = m_tileHiddenTexture;
                }
            }

            sf::Sprite tileSprite;
            tileSprite.setTexture(texture);
            tileSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
            m_window.draw(tileSprite);

            if (m_gameGrid[row][col].isFlagged)
            {
                texture = m_flagTexture;
                // Override texture for flagged tiles
            }

            // Create and position the sprite
            tileSprite.setTexture(texture);
            tileSprite.setPosition(col * TILE_SIZE, row * TILE_SIZE);
            m_window.draw(tileSprite);
        }
    }
}


void Board::initializeGameGrid()
{
    m_gameGrid.resize(m_numRows, std::vector<Tile>(m_numColumns));
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for random number generator
    for (int i = 0; i < m_numMines; ++i) {
        int randomRow = std::rand() % m_numRows;
        int randomCol = std::rand() % m_numColumns;

        // Ensure that a mine is not placed at the same position twice
        while (m_gameGrid[randomRow][randomCol].isMine) {
            randomRow = std::rand() % m_numRows;
            randomCol = std::rand() % m_numColumns;
        }

        m_gameGrid[randomRow][randomCol].isMine = true;
    }
    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numColumns; ++col) {
            if (!m_gameGrid[row][col].isMine) {
                int adjacentMines = 0;

                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int newRow = row + i;
                        int newCol = col + j;

                        if (newRow >= 0 && newRow < m_numRows && newCol >= 0 && newCol < m_numColumns &&
                            m_gameGrid[newRow][newCol].isMine) {
                            ++adjacentMines;
                        }
                    }
                }

                m_gameGrid[row][col].adjacentMines = adjacentMines;
            }
        }
    }
}

void Board::reset()
{
    m_gameOver = false;
    m_gameWon = false;
    m_gamePaused = false;

    // Reset game grid and initialize new mines
    initializeGameGrid();

    // Clear revealed and flagged status for all tiles
    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numColumns; ++col) {
            m_gameGrid[row][col].isRevealed = false;
            m_gameGrid[row][col].isFlagged = false;
        }
    }

    // Reset UI components and their positions
    m_happyFaceButton.setTexture(m_happyFaceTexture);
    m_winfaceButton.setPosition(m_happyFaceButton.getPosition());
    m_losefaceButton.setPosition(m_happyFaceButton.getPosition());

    // Redraw the window with the fresh game grid
    m_window.clear(sf::Color::White);
    drawUIComponents();
    drawGameGrid();
    m_window.display();
}

bool Board::isInsideSmileyArea(const sf::Vector2i& mousePosition) const
{
    sf::FloatRect smileyAreaRect(
            ((m_numColumns / 2.0f) * TILE_SIZE) - 32,
            TILE_SIZE * (m_numRows + 0.5f),
            m_happyFaceButton.getGlobalBounds().width,
            m_happyFaceButton.getGlobalBounds().height
    );

    return smileyAreaRect.contains(sf::Vector2f(mousePosition));
}

bool Board::runGameWindow()
{
    if (!readConfigFile("/Users/aparnanimmagadda/CLionProjects/Project3/board_config.cfg"))
    {
        std::cerr << "Error reading config file." << std::endl;
        return false;
    }

    int requiredUIHeight = TILE_SIZE * (m_numRows + 1);
    int windowWidth = m_numColumns * TILE_SIZE;
    int windowHeight = requiredUIHeight + BUTTON_SPACE;
    m_window.create(sf::VideoMode(windowWidth, windowHeight), "Minesweeper Game", sf::Style::Close);
    initializeGameGrid();

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                if (handleClickingLeader(mousePosition)) {
                    m_leadership.showLeadership(m_window);
                    // Keep the window open until the user closes it
                    sf::Event leadershipEvent;
                    while (m_window.waitEvent(leadershipEvent)) {
                        if (leadershipEvent.type == sf::Event::Closed) {
                            break;
                        }
                    }
                } else if (handleClickingPause(mousePosition)) {
                    runPausePlayButton();
                }else if(isInsideSmileyArea(mousePosition))
                {
                    reset();

                } else {
                    int clickedRow = mousePosition.y / TILE_SIZE;
                    int clickedCol = mousePosition.x / TILE_SIZE;
                    handleLeftTileClick(clickedRow, clickedCol); //left click
                }
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                handleTileClick(mousePosition);
            }
        }

        if (!m_gamePaused && !m_gameOver) {
            int newElapsedTime = static_cast<int>(m_timerClock.getElapsedTime().asSeconds());
            if (newElapsedTime > m_elapsedTime) {
                m_elapsedTime = newElapsedTime;
                m_minutes = m_elapsedTime / 60;
                m_seconds = m_elapsedTime % 60;
            }
        }
        // Update game logic
        // Clear and redraw window
        m_window.clear(sf::Color::White);
        drawUIComponents();
        drawGameGrid();
        drawTimerDigits();
        m_window.display();
    }

    return true;
}

