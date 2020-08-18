#ifndef RED_GEM_HPP
#define RED_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class RedGem : public Gem
   {
   public:
      explicit RedGem();
      RedGem(RedGem &&) = delete;
      RedGem(const RedGem &) = delete;
      RedGem &operator=(RedGem &&) = delete;
      RedGem &operator=(const RedGem &) = delete;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // RED_GEM_HPP