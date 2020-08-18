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
      sf::RenderWindow renderWin;
      Board mainBoard;
      sf::Texture bgTex;
      sf::Sprite bgSpr;
      std::string playerName;

      explicit Window();
      Window(Window &&) = delete;
      Window(const Window &) = delete;
      Window &operator=(Window &&) = delete;
      Window &operator=(const Window &) = delete;

      void setPlayerName(const std::string &);

   public:
      static Window &getHandler(const std::string &);
      void run();
   };
} // namespace gc_game

#endif // WINDOW_HPP