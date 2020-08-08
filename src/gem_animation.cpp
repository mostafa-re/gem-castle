#include "gem_animation.hpp"

namespace gc_game
{
   GemAnimation::GemAnimation(float moveStep) : moveStep(moveStep) {}

   void GemAnimation::operator()(Gem &obj)
   {
      switch (obj.getStatus())
      {
      case GemStatus::MOVE:
      case GemStatus::SWAP_BOTTOM:
      case GemStatus::SWAP_TOP:
         if (obj.getPosition().x - obj.getTransformable().getPosition().x < this->moveStep && obj.getPosition().y - obj.getTransformable().getPosition().y < this->moveStep)
         {
            obj.getTransformable().setPosition(obj.getPosition());
            obj.setStatus(GemStatus::NONE);
         }
         else
         {
            obj.getTransformable().move((obj.getPosition() - obj.getTransformable().getPosition()) * 0.2f);
         }
         break;

      default:
         std::out_of_range("Undifiend animation type detected!");
         break;
      }
   }
} // namespace gc_game