#ifndef GEM_HPP
#define GEM_HPP

#include <SFML/Graphics.hpp>

namespace gc_game
{
   class Gem : public sf::Sprite
   {
   protected:
      sf::Texture sourceTex;
      unsigned short id;
      size_t point;

   public:
      explicit Gem(const std::string &, unsigned short, size_t);
      unsigned short getID() const;
      // virtual bool runAnimation() = 0;
   };
} // namespace gc_game

#endif // GEM_HPP