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
      bool isSelectable;
      unsigned size;
      sf::Color bgColor;
      sf::Texture gemBoxTex;
      sf::Texture clickTex;
      sf::Texture swapTex;
      mutable sf::Sprite gemBoxSpr;
      mutable sf::Sprite clickSpr;
      mutable sf::Sprite swapSpr;
      mutable sf::RenderTexture boardTex;
      mutable sf::Sprite boardSpr;
      std::vector<std::vector<unsigned short>> gemIdGrid;

   public:
      Board(unsigned);
      void reset();
      sf::Transformable &getTransformable();
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
   };
} // namespace gc_game

#endif //BOARD_HPP