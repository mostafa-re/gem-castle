#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>
#include <random>
#include <algorithm>
#include "black_gem.hpp"
#include "blue_gem.hpp"
#include "green_gem.hpp"
#include "purple_gem.hpp"
#include "red_gem.hpp"
#include "yellow_gem.hpp"
#include "move_anim.hpp"

namespace gc_game
{
   class Board : public sf::Drawable
   {
   private:
      unsigned size;
      sf::Color bgColor;
      mutable sf::RenderTexture boardTex;
      mutable sf::Sprite spr;
      std::vector<std::vector<unsigned short>> gemIdGrid;

   public:
      Board(unsigned);
      void reset();
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif //BOARD_HPP