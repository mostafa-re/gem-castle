#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace gc_game
{
   class Board : public sf::Drawable
   {
   private:
      unsigned size;
      sf::Color bgColor;
      sf::RenderTexture boardTex;
      sf::Sprite spr;

   public:
      Board(unsigned);
      void reset();
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif //BOARD_HPP