#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <chrono>
#include <cmath>
#include "board.hpp"

#define MAX_PLAYER_NAME 20
#define MAX_TIMER_STRING 10

namespace gc_game
{
   class Game
   {
   private:
      Board mainBoard;
      u_int64_t score;
      char playerName[MAX_PLAYER_NAME];
      std::chrono::system_clock::time_point timerStart;
      sf::RenderWindow renderWin;
      sf::Text playerNameTxt;
      sf::Text scoreTxt;
      sf::Text timerTxt;
      sf::Text comboTxt;
      sf::Texture bgTex;
      sf::Sprite bgSpr;
      sf::Font font;

      explicit Game();
      Game(Game &&) = delete;
      Game(const Game &) = delete;
      Game &operator=(Game &&) = delete;
      Game &operator=(const Game &) = delete;

      //assist func
      void updateScore();
      void updateTimer();

   public:
      void setPlayerName(const std::string &);
      static Game &getHandler();
      void run();
   };
} // namespace gc_game

#endif // GAME_HPP