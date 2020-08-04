#ifndef BLACK_GEM_HPP
#define BLACK_GEM_HPP

#include "gem.hpp"

namespace gc_game
{
   class BlackGem : public Gem
   {
   private:
   public:
      explicit BlackGem();
      virtual bool runAnimation() override;
   };
} // namespace gc_game

#endif // BLACK_GEM_HPP