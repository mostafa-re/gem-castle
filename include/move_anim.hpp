#ifndef MOVE_ANIM_HPP
#define MOVE_ANIM_HPP

#include "animation_base.hpp"

namespace gc_game
{
   class MoveAnim : public AnimationBase
   {
   protected:
      float moveStep;

   public:
      explicit MoveAnim(const std::chrono::milliseconds &, float);
      bool operator()(sf::Transformable &, const sf::Vector2f &);
   };
} // namespace gc_game
#endif //MOVE_ANIM_HPP