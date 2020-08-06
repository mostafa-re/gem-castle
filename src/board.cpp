#include "board.hpp"

namespace gc_game
{
   Board::Board(unsigned size) : size(size), bgColor(128, 128, 128, 128)
   {
      if (this->size < 4)
      {
         throw std::invalid_argument("Board size is too small");
      }
      if (!this->boardTex.create((this->size * 55) + 2, (this->size * 55) + 2))
      {
         throw std::runtime_error("unable to create textureRender of board!");
      }
      this->reset();
   }

   void Board::reset()
   {
      this->boardTex.clear(this->bgColor);
   }

   void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      this->boardTex.display();
      this->spr.setTexture(this->boardTex.getTexture());
      target.draw(this->spr, states);
   }
} // namespace gc_game