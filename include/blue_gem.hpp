#ifndef BLUE_GEM_HPP
#define BLUE_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class BlueGem : public Gem
   {
   public:
      explicit BlueGem();
      BlueGem(BlueGem &&) = delete;
      BlueGem(const BlueGem &) = delete;
      BlueGem &operator=(BlueGem &&) = delete;
      BlueGem &operator=(const BlueGem &) = delete;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // BLUE_GEM_HPP