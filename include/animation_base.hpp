#ifndef ANIMATION_BASE_HPP
#define ANIMATION_BASE_HPP

#include <SFML/Graphics.hpp>
#include <chrono>

namespace gc_game
{
   class AnimationBase
   {
   protected:
      std::chrono::milliseconds stopDuration;
      std::chrono::high_resolution_clock::time_point starTimePoint;

   public:
      explicit AnimationBase(const std::chrono::milliseconds &);
      void reset();
      virtual bool run(sf::Transformable&) = 0;
      virtual ~AnimationBase();
   };
} // namespace gc_game

#endif // ANIMATION_BASE_HPP