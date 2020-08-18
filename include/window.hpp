#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
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

      explicit Window();
      Window(Window &&) = delete;
      Window(const Window &) = delete;
      Window &operator=(Window &&) = delete;
      Window &operator=(const Window &) = delete;

   public:
      static Window &getHandler();
      void run();
   };
} // namespace gc_game

#endif // WINDOW_HPP