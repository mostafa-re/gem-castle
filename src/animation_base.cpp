#include "animation_base.hpp"

namespace gc_game
{
    AnimationBase::AnimationBase(sf::Transformable &obj, const std::chrono::milliseconds &duration)
        : done(false), obj(obj), duration(duration), start(std::chrono::high_resolution_clock::now())
    {
    }

    AnimationBase::~AnimationBase() {}
} // namespace gc_game