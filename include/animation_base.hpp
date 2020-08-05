#ifndef ANIMATION_BASE_HPP
#define ANIMATION_BASE_HPP

#include <SFML/Graphics.hpp>
#include <chrono>

namespace gc_game
{
   class AnimationBase
   {
   protected:
      sf::Transformable obj;
      std::chrono::high_resolution_clock::time_point start;
      std::chrono::milliseconds duration;

   public:
      AnimationBase(sf::Transformable &, const std::chrono::milliseconds&);
      virtual bool done() = 0;
      virtual ~AnimationBase();
   };
} // namespace gc_game

#endif // ANIMATION_BASE_HPP
