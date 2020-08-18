#include "gem.hpp"

namespace gc_game
{
   Gem::Gem(const std::string &path, unsigned short id, size_t point)
       : id(id), currentGemStatus(GemStatus::NONE), position(0.f, 0.f), point(point)
   {
      if (!this->sourceTex.loadFromFile(path))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      this->sourceTex.setSmooth(true);
      this->sourceTex.setRepeated(false);
      this->spr.setTexture(this->sourceTex);
   }

   sf::Transformable &Gem::getTransformable()
   {
      return this->spr;
   }

   unsigned short Gem::getID() const
   {
      return this->id;
   }

   size_t Gem::getPoint() const
   {
      return this->point;
   }

   float Gem::getAbsoluteHeight() const
   {
      return this->spr.getLocalBounds().height * this->spr.getScale().y;
   }

   float Gem::getAbsoluteWidth() const
   {
      return this->spr.getLocalBounds().width * this->spr.getScale().x;
   }

   const sf::Vector2f &Gem::getPosition() const
   {
      return this->position;
   }

   const GemStatus &Gem::getStatus() const
   {
      return this->currentGemStatus;
   }

   void Gem::setPosition(const sf::Vector2f &pos)
   {
      this->position = pos;
   }

   void Gem::setStatus(const GemStatus &status)
   {
      this->currentGemStatus = status;
   }
} // namespace gc_game