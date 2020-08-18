#ifndef GREEN_GEM_HPP
#define GREEN_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class GreenGem : public Gem
   {
   public:
      explicit GreenGem();
      GreenGem(GreenGem &&) = delete;
      GreenGem(const GreenGem &) = delete;
      GreenGem &operator=(GreenGem &&) = delete;
      GreenGem &operator=(const GreenGem &) = delete;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // GREEN_GEM_HPP