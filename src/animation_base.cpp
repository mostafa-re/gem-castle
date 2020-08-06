#include "animation_base.hpp"

namespace gc_game
{
    AnimationBase::AnimationBase(const std::chrono::milliseconds &stopDuration)
        : stopDuration(stopDuration), starTimePoint(std::chrono::high_resolution_clock::now())
    {
    }

    void AnimationBase::reset()
    {
        this->starTimePoint = std::chrono::high_resolution_clock::now();
    }

    AnimationBase::~AnimationBase()
    {
    }
} // namespace gc_game