#include "black_gem.hpp"

namespace gc_game
{
   BlackGem::BlackGem() : Gem("../assets/black_gem.png", 1, 10) {}

   void BlackGem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->sourceSpr, states);
   }
} // namespace gc_game