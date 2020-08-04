#include "gem.hpp"

namespace gc_game
{
   Gem::Gem(const std::string &path, unsigned short id, size_t point) : id(id), point(point)
   {
      this->sourceTex.loadFromFile(path);
      this->setTexture(this->sourceTex);
   }

   unsigned short Gem::getID() const
   {
      return this->id;
   }
} // namespace gc_game