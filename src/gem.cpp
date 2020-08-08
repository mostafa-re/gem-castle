#include "gem.hpp"

namespace gc_game
{
   Gem::Gem(const std::string &path, unsigned short id, size_t point)
       : id(id), point(point), currentGemStatus(GemStatus::NONE), position(0.f, 0.f)
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

   void Gem::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      target.draw(this->spr, states);
   }

   const sf::Vector2f &Gem::getPosition() const
   {
      return this->position;
   }

   void Gem::setPosition(const sf::Vector2f &pos)
   {
      this->position = pos;
   }

   const GemStatus &Gem::getStatus() const
   {
      return this->currentGemStatus;
   }

   void Gem::setStatus(const GemStatus &status)
   {
      this->currentGemStatus = status;
   }

   float Gem::getHeight() const
   {
      return this->spr.getLocalBounds().height * this->spr.getScale().y;
   }

   float Gem::getWidth() const
   {
      return this->spr.getLocalBounds().width * this->spr.getScale().x;
   }
} // namespace gc_game