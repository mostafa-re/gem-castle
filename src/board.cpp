#include "board.hpp"

namespace gc_game
{
   Board::Board(unsigned size) : isSelectable(false), size(size)
   {
      if (this->size < 4)
      {
         throw std::invalid_argument("Board size is too small");
      }
      if (!this->gemBoxTex.loadFromFile("../assets/gembox.png") ||
          !this->clickTex.loadFromFile("../assets/click.png") ||
          !this->swapTex.loadFromFile("../assets/swap.png"))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      if (!this->boardTex.create((this->size * 55) + 2, (this->size * 55) + 3))
      {
         throw std::runtime_error("unable to create textureRender of board!");
      }

      this->gemBoxTex.setSmooth(true);
      this->gemBoxTex.setRepeated(false);
      this->gemBoxSpr.setTexture(this->gemBoxTex);
      this->clickTex.setSmooth(true);
      this->clickTex.setRepeated(false);
      this->clickSpr.setTexture(this->clickTex);
      this->swapTex.setSmooth(true);
      this->swapTex.setRepeated(false);
      this->swapSpr.setTexture(this->swapTex);

      for (size_t i = 0; i < this->size; i++)
      {
         this->gemIdGrid.emplace_back(this->size);
      }
      this->gemIdGrid.shrink_to_fit();
      this->reset();
   }

   void Board::clearBoard() const
   {
      this->boardTex.clear(sf::Color(0, 0, 0, 0));
      sf::VertexArray border(sf::Lines, 2);
      border[0].color = border[1].color = sf::Color(149, 149, 149, 255);
      border[0].position = (sf::Vector2f(2.f, 1.f));
      border[1].position = (sf::Vector2f(static_cast<float>(this->size * 55), 1.f));
      this->boardTex.draw(border);
      border[0].position = (sf::Vector2f(static_cast<float>(this->size * 55) + 2, 2.f));
      border[1].position = (sf::Vector2f(static_cast<float>(this->size * 55) + 2, static_cast<float>(this->size * 55) - 1));
      this->boardTex.draw(border);
      border[0].position = (sf::Vector2f(2.f, static_cast<float>(this->size * 55) + 2));
      border[1].position = (sf::Vector2f(static_cast<float>(this->size * 55) - 1, static_cast<float>(this->size * 55) + 2));
      this->boardTex.draw(border);
      border[0].position = (sf::Vector2f(1.f, 2.f));
      border[1].position = (sf::Vector2f(1.f, static_cast<float>(this->size * 55) - 1));
      this->boardTex.draw(border);
      for (size_t i = 0; i < this->size; i++)
      {
         for (size_t j = 0; j < this->size; j++)
         {
            this->gemBoxSpr.setPosition((i * 55) + 1, (j * 55) + 1);
            this->boardTex.draw(this->gemBoxSpr);
         }
      }
   }

   void Board::reset()
   {
      std::default_random_engine randGenerator(time(nullptr));
      std::uniform_int_distribution<unsigned> randDistrib6(0, 5);
      std::uniform_int_distribution<unsigned> randDistrib5(0, 4);
      std::uniform_int_distribution<unsigned> randDistrib4(0, 3);
      std::vector<unsigned short> set(6);

      bool topDuplicate, leftDuplicate;
      for (size_t i = 0; i < this->size; i++)
      {
         for (size_t j = 0; j < this->size; j++)
         {
            set = {1, 2, 3, 4, 5, 6};
            topDuplicate = leftDuplicate = false;
            if (i > 2 && this->gemIdGrid[i - 1][j] == this->gemIdGrid[i - 2][j])
            {
               topDuplicate = true;
               std::swap(set[this->gemIdGrid[i - 1][j] - 1], set[set.size() - 1]);
            }
            if (j > 2 && this->gemIdGrid[i][j - 1] == this->gemIdGrid[i][j - 2])
            {
               leftDuplicate = true;
               if (topDuplicate)
               {
                  std::swap(set[this->gemIdGrid[i][j - 1] - 1], set[set.size() - 2]);
               }
               else
               {
                  std::swap(set[this->gemIdGrid[i][j - 1] - 1], set[set.size() - 1]);
               }
            }
            if (topDuplicate && leftDuplicate)
            {
               this->gemIdGrid[i][j] = set[randDistrib4(randGenerator)];
            }
            else if (topDuplicate || leftDuplicate)
            {
               this->gemIdGrid[i][j] = set[randDistrib5(randGenerator)];
            }
            else
            {
               this->gemIdGrid[i][j] = set[randDistrib6(randGenerator)];
            }
         }
      }
   }

   sf::Transformable &Board::getTransformable()
   {
      return this->boardSpr;
   }

   void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      this->clearBoard();

      this->boardTex.display();
      this->boardSpr.setTexture(this->boardTex.getTexture());
      target.draw(this->boardSpr, states);
   }
} // namespace gc_game