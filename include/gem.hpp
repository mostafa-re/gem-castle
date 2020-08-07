#ifndef GEM_HPP
#define GEM_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace gc_game
{

   enum class GemStatus : unsigned short
   {
      NONE,
      MOVE,
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
      size_t getPoint() const;
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;

      const sf::Vector2f &getPosition() const;
      void setPosition(const sf::Vector2f &);
      const GemStatus &getStatus() const;
      void setStatus(const GemStatus &);
   };
} // namespace gc_game

#endif // GEM_HPP