#include "gem.hpp"

namespace gc_game
{
   Gem::Gem(const std::string &path, unsigned short id, size_t point) : id(id), point(point)
   {
      if (!this->sourceTex.loadFromFile(path))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      this->sourceTex.setSmooth(true);
      this->sourceTex.setRepeated(false);
      this->spr.setTexture(this->sourceTex);
   }

   sf::Sprite &Gem::getSprite()
   {
      return this->spr;
   }

   unsigned short Gem::getID() const
   {
      return this->id;
   }

   void Gem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->spr, states);
   }
} // namespace gc_game