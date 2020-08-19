#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <cmath>
#include "board.hpp"

namespace gc_game
{
   class Window
   {
   private:
      Board mainBoard;
      u_int16_t score;
      std::string playerName;
      sf::RenderWindow renderWin;
      sf::Text playerNameTxt;
      sf::Text scoreTxt;
      sf::Text timerTxt;
      sf::Text comboTxt;
      sf::Texture bgTex;
      sf::Sprite bgSpr;
      sf::Font font;

      explicit Window();
      Window(Window &&) = delete;
      Window(const Window &) = delete;
      Window &operator=(Window &&) = delete;
      Window &operator=(const Window &) = delete;

   public:
      void setPlayerName(const std::string &);
      static Window &getHandler();
      void run();
   };
} // namespace gc_game

#endif // WINDOW_HPP