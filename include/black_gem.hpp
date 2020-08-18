#ifndef BLACK_GEM_HPP
#define BLACK_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class BlackGem : public Gem
   {
   public:
      explicit BlackGem();
      BlackGem(BlackGem &&) = delete;
      BlackGem(const BlackGem &) = delete;
      BlackGem &operator=(BlackGem &&) = delete;
      BlackGem &operator=(const BlackGem &) = delete;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // BLACK_GEM_HPP