#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace gc_game
{
   class Board : sf::Sprite
   {
   private:
      size_t size;
      sf::RenderTexture boardTex;

   public:
      Board(size_t);
   };
} // namespace gc_game

#endif //BOARD_HPP