#ifndef GEM_HPP
#define GEM_HPP

#include <SFML/Graphics.hpp>

namespace gc_game
{
   class Gem : public sf::Sprite
   {
   protected:
      sf::Texture sourceTex;
      size_t point;

   public:
      explicit Gem(const std::string &, size_t);
      virtual bool runAnimation() = 0;
      virtual ~Gem();
   };
} // namespace gc_game

#endif // GEM_HPP