#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>
#include <random>
#include <memory>
#include <thread>
#include <mutex>
#include <algorithm>
#include "gem_animation.hpp"
#include "black_gem.hpp"
#include "blue_gem.hpp"
#include "green_gem.hpp"
#include "purple_gem.hpp"
#include "red_gem.hpp"
#include "yellow_gem.hpp"

namespace gc_game
{
   class Board : public sf::Drawable
   {
   private:
      bool isSelectable;
      unsigned size;
      sf::Texture gemBoxTex;
      sf::Texture clickTex;
      sf::Texture swapTex;
      sf::Sprite gemBoxSpr;
      sf::Sprite clickSpr;
      sf::Sprite swapSpr;
      mutable sf::RenderTexture boardTex;
      mutable sf::Sprite boardSpr;
      std::vector<std::vector<std::unique_ptr<Gem>>> gemGrid;

      std::thread renderThread;
      mutable std::mutex renderMutex;
      bool renderDone;
      void render();

      void clearBoard();

   public:
      Board(unsigned);
      void reset();
      sf::Transformable &getTransformable();
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
      virtual ~Board() override;
   };
} // namespace gc_game

#endif //BOARD_HPP