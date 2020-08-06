#include "board.hpp"

namespace gc_game
{
   Board::Board(size_t size) : size(size)
   {
      if (this->size < 4)
      {
         throw std::invalid_argument("Board size is too small");
      }

   }
} // namespace gc_game