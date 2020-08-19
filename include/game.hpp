#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <thread>
#include <string>
#include <ctime>
#include <cmath>
#include "board.hpp"

#define MAX_PLAYER_NAME 20

namespace gc_game
{
   class Game
   {
   private:
      Board mainBoard;
      u_int64_t score;
      char playerName[MAX_PLAYER_NAME];
      time_t timerStart;
      bool isPause;

      sf::RenderWindow renderWin;
      sf::Text playerNameTxt;
      sf::Text scoreTxt;
      sf::Text timerTxt;
      sf::Text comboTxt;
      sf::Font font;
      sf::Texture redResumeBottunTex;
      sf::Texture resumeBottunTex;
      sf::Texture pauseBottunTex;
      sf::Texture resetButtonTex;
      sf::Texture bgTex;
      sf::Sprite resumeOrPauseButtonSpr;
      sf::Sprite resetBottunSpr;
      sf::Sprite bgSpr;

      explicit Game();
      Game(Game &&) = delete;
      Game(const Game &) = delete;
      Game &operator=(Game &&) = delete;
      Game &operator=(const Game &) = delete;

      //assist func
      void updateScore();
      void updateTimer();
      void savePlayerResult();

   public:
      void setPlayerName(const std::string &);
      static Game &getHandler();
      void run();
   };
} // namespace gc_game

#endif // GAME_HPP