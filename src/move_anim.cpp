#include "move_anim.hpp"

namespace gc_game
{
   MoveAnim::MoveAnim(const std::chrono::milliseconds &stopDuration, float moveStep)
       : AnimationBase(stopDuration), moveStep(moveStep) {}

   bool MoveAnim::operator()(sf::Transformable &obj, const sf::Vector2f &endPos)
   {
      std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - this->starTimePoint;
      if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() > this->stopDuration.count())
      {
         if (endPos.x - obj.getPosition().x < 2.f && endPos.y - obj.getPosition().y < 2.f)
         {
            obj.setPosition(endPos);
            return true;
         }
         else
         {
            obj.move((endPos - obj.getPosition()) * this->moveStep);
            return false;
         }
      }
      else
      {
         return false;
      }
   }
} // namespace gc_game