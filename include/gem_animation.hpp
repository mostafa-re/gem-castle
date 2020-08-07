#ifndef GEM_ANIMATION_HPP
#define GEM_ANIMATION_HPP

#include <stdexcept>
#include "gem.hpp"

namespace gc_game
{
   class GemAnimation
   {
   protected:
      float moveStep;

   public:
      explicit GemAnimation(float);
      void operator()(Gem &);
   };
} // namespace gc_game
#endif //GEM_ANIMATION_HPP