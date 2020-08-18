#include "purple_gem.hpp"

namespace gc_game
{
   PurpleGem::PurpleGem() : Gem("../assets/purple_gem.png", 4, 10) {}

   void PurpleGem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->spr, states);
   }
} // namespace gc_game