#include "yellow_gem.hpp"

namespace gc_game
{
   YellowGem::YellowGem() : Gem("../assets/yellow_gem.png", 6, 10) {}

   void YellowGem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->sourceSpr, states);
   }
} // namespace gc_game