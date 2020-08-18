#include "green_gem.hpp"

namespace gc_game
{
   GreenGem::GreenGem() : Gem("../assets/green_gem.png", 3, 10) {}

   void GreenGem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->sourceSpr, states);
   }
} // namespace gc_game