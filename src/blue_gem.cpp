#include "blue_gem.hpp"

namespace gc_game
{
   BlueGem::BlueGem() : Gem("../assets/blue_gem.png", 2, 10) {}

   void BlueGem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->sourceSpr, states);
   }
} // namespace gc_game