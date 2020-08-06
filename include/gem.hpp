#ifndef GEM_HPP
#define GEM_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace gc_game
{
   class Gem : public sf::Drawable
   {
   protected:
      sf::Sprite spr;
      sf::Texture sourceTex;
      unsigned short id;
      size_t point;

   public:
      explicit Gem(const std::string &, unsigned short, size_t);
      sf::Transformable &getTransformable();
      unsigned short getID() const;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // GEM_HPP