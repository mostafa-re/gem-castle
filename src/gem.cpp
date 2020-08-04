#include "gem.hpp"

namespace gc_game
{
   Gem::Gem(const std::string &path, size_t point) : point(point)
   {
      this->sourceTex.loadFromFile(path);
      this->setTexture(this->sourceTex);
   }
} // namespace gc_game