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
      SWAP,
      FADEOUT,
      HIDE
   };

   class Gem : public sf::Drawable
   {
   protected:
      unsigned short id;
      GemStatus currentGemStatus;
      sf::Vector2f position;
      sf::Texture sourceTex;
      sf::Sprite spr;
      size_t point;

   public:
      explicit Gem(const std::string &, unsigned short, size_t);
      sf::Transformable &getTransformable();
      unsigned short getID() const;
      size_t getPoint() const;

      float getAbsoluteHeight() const;
      float getAbsoluteWidth() const;
      const sf::Vector2f &getPosition() const;
      const GemStatus &getStatus() const;
      void setPosition(const sf::Vector2f &);
      void setStatus(const GemStatus &);
   };
} // namespace gc_game

#endif // GEM_HPP