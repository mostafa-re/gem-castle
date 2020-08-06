#ifndef GEM_HPP
#define GEM_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace gc_game
{

   enum class GemStatus : unsigned short
   {
      NONE,
      MOVE_DOWN,
      SWAP_TOP,
      SWAP_BOTTOM,
      FADEOUT
   };

   class Gem : public sf::Drawable
   {
   protected:
      sf::Sprite spr;
      sf::Texture sourceTex;
      unsigned short id;
      size_t point;
      GemStatus currentGemStatus;
      sf::Vector2f position;

   public:
      explicit Gem(const std::string &, unsigned short, size_t);
      sf::Transformable &getTransformable();
      unsigned short getID() const;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif // GEM_HPP