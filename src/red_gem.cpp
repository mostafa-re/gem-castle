#include "red_gem.hpp"

namespace gc_game
{
   RedGem::RedGem() : Gem("../assets/red_gem.png", 5, 10) {}

   void RedGem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->sourceSpr, states);
   }
} // namespace gc_game