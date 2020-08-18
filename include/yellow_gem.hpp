#ifndef YELLOW_GEM_HPP
#define YELLOW_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class YellowGem : public Gem
   {
   public:
      explicit YellowGem();
      YellowGem(YellowGem &&) = delete;
      YellowGem(const YellowGem &) = delete;
      YellowGem &operator=(YellowGem &&) = delete;
      YellowGem &operator=(const YellowGem &) = delete;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // YELLOW_GEM_HPP