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
      for (size_t i = 0; i < this->size; i++)
      {
         this->gemIdGrid.emplace_back(this->size);
      }
      this->gemIdGrid.shrink_to_fit();

      this->reset();
   }

   void Board::reset()
   {
      this->boardTex.clear(this->bgColor);
      std::default_random_engine randGenerator(time(nullptr));
      std::uniform_int_distribution<unsigned> randDistrib(1, 6);
      for (auto &row : this->gemIdGrid)
      {
         for (auto &cell : row)
         {
            cell = randDistrib(randGenerator);
         }
      }
      
   }

   void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      this->boardTex.display();
      this->spr.setTexture(this->boardTex.getTexture());
      target.draw(this->spr, states);
   }
} // namespace gc_game