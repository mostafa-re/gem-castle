#include "gem.hpp"

namespace gc_game
{
   Gem::Gem(const std::string &path, unsigned short id, size_t point) : id(id), point(point)
   {
      if (!this->sourceTex.loadFromFile(path))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      this->setTexture(this->sourceTex);
   }

   unsigned short Gem::getID() const
   {
      return this->id;
   }
} // namespace gc_game