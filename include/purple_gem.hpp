#ifndef PURPLE_GEM_HPP
#define PURPLE_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class PurpleGem : public Gem
   {
   public:
      explicit PurpleGem();
      PurpleGem(PurpleGem &&) = delete;
      PurpleGem(const PurpleGem &) = delete;
      PurpleGem &operator=(PurpleGem &&) = delete;
      PurpleGem &operator=(const PurpleGem &) = delete;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // PURPLE_GEM_HPP