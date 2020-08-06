#ifndef ANIMATION_BASE_HPP
#define ANIMATION_BASE_HPP

#include <SFML/Graphics.hpp>
#include <chrono>

namespace gc_game
{
   class AnimationBase
   {
   protected:
      bool done;
      sf::Transformable obj;
      std::chrono::milliseconds duration;
      std::chrono::high_resolution_clock::time_point start;

   public:
      explicit AnimationBase(sf::Transformable &, const std::chrono::milliseconds &);
      virtual bool isDone() = 0;
      virtual ~AnimationBase();
   };
} // namespace gc_game

#endif // ANIMATION_BASE_HPP
