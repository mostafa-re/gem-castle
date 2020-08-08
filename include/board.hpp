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
#include <cmath>
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
      unsigned size;
      sf::Texture gemBoxTex;
      sf::Texture clickTex;
      sf::Texture swapTex;
      sf::Sprite gemBoxSpr;
      sf::Sprite clickSpr;
      sf::Sprite swapSpr;
      mutable sf::Sprite boardSpr;

      // thread based vars
      mutable sf::RenderTexture boardTex;
      std::vector<std::vector<std::shared_ptr<Gem>>> gemGrid;
      std::shared_ptr<Gem> clickedGem;
      size_t point;
      size_t comboIndicator;

      std::thread renderThread;
      mutable std::mutex renderMutex;
      bool renderDone;
      bool renderSleep;
      void render();

      // non lock -- assistant funcs
      void clearBoard();
      void clearClickedGem();
      void moveGems(Gem &, const float &);
      bool swapGems(std::shared_ptr<Gem>, std::shared_ptr<Gem>);
      bool checkBoard();

   public:
      Board(unsigned);
      void reset();
      sf::Transformable &getTransformable();
      virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;
      virtual ~Board() override;

      void mouseClick(const sf::Event::MouseButtonEvent &);
   };
} // namespace gc_game

#endif //BOARD_HPP