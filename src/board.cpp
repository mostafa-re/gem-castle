#include "board.hpp"

namespace gc_game
{
   Board::Board(unsigned size) : isSelectable(false), size(size), renderDone(false), renderSleep(true)
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
         this->gemGrid.emplace_back(this->size);
      }
      this->gemGrid.shrink_to_fit();
      this->renderThread = std::thread(&Board::render, this);
      this->reset();
   }

   void Board::clearBoard()
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
      std::lock_guard<std::mutex> lock(this->renderMutex);

      std::default_random_engine randGenerator(time(nullptr));
      std::uniform_int_distribution<unsigned> randDistrib6(0, 5);
      std::uniform_int_distribution<unsigned> randDistrib5(0, 4);
      std::uniform_int_distribution<unsigned> randDistrib4(0, 3);
      std::vector<unsigned short> set(6);
      set = {1, 2, 3, 4, 5, 6};
      unsigned short selected;

      bool topDuplicate, leftDuplicate;
      for (size_t i = 0; i < this->size; i++)
      {
         for (size_t j = 0; j < this->size; j++)
         {
            topDuplicate = leftDuplicate = false;
            if (i > 1 && this->gemGrid[i - 1][j]->getID() == this->gemGrid[i - 2][j]->getID())
            {
               topDuplicate = true;
               std::swap(set[this->gemGrid[i - 1][j]->getID() - 1], set[set.size() - 1]);
            }
            if (j > 1 && this->gemGrid[i][j - 1]->getID() == this->gemGrid[i][j - 2]->getID())
            {
               leftDuplicate = true;
               if (topDuplicate)
               {
                  std::swap(set[this->gemGrid[i][j - 1]->getID() - 1], set[set.size() - 2]);
               }
               else
               {
                  std::swap(set[this->gemGrid[i][j - 1]->getID() - 1], set[set.size() - 1]);
               }
            }
            if (topDuplicate && leftDuplicate)
            {
               selected = set[randDistrib4(randGenerator)];
               std::swap(set[set[set.size() - 1] - 1], set[set.size() - 1]);
               std::swap(set[set[set.size() - 2] - 1], set[set.size() - 2]);
            }
            else if (topDuplicate || leftDuplicate)
            {
               selected = set[randDistrib5(randGenerator)];
               std::swap(set[set[set.size() - 1] - 1], set[set.size() - 1]);
            }
            else
            {
               selected = set[randDistrib6(randGenerator)];
            }
            switch (selected)
            {
            case 1:
               this->gemGrid[i][j].reset(new BlackGem());
               break;
            case 2:
               this->gemGrid[i][j].reset(new BlueGem());
               break;
            case 3:
               this->gemGrid[i][j].reset(new GreenGem());
               break;
            case 4:
               this->gemGrid[i][j].reset(new PurpleGem());
               break;
            case 5:
               this->gemGrid[i][j].reset(new RedGem());
               break;
            case 6:
               this->gemGrid[i][j].reset(new YellowGem());
               break;

            default:
               throw std::out_of_range("Unable to cerate undefined gem!");
               break;
            }

            this->gemGrid[i][j]->getTransformable().setPosition((i * 55) + 1, -255);
            this->gemGrid[i][j]->setStatus(GemStatus::MOVE);
            this->gemGrid[i][j]->setPosition(sf::Vector2f((i * 55) + 1, (j * 55) + 1));
         }
      }

      this->renderSleep = false;
   }

   sf::Transformable &Board::getTransformable()
   {
      return this->boardSpr;
   }

   void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      {
         std::lock_guard<std::mutex> lock(this->renderMutex);
         this->boardTex.display();
         this->boardSpr.setTexture(this->boardTex.getTexture());
      }
      target.draw(this->boardSpr, states);
   }

   Board::~Board()
   {
      {
         std::lock_guard<std::mutex> lock(this->renderMutex);
         this->renderDone = true;
      }
      this->renderThread.join();
   }

   void Board::render()
   {
      GemAnimation anim(0.0608f);
      size_t withoutRenderGems;

      std::unique_lock<std::mutex> lock(this->renderMutex);
      while (!this->renderDone)
      {
         lock.unlock();
         std::this_thread::sleep_for(std::chrono::milliseconds(25));
         lock.lock();

         if (!this->renderSleep)
         {
            this->clearBoard();
            withoutRenderGems = 0;
            for (auto &rows : this->gemGrid)
            {
               for (auto &item : rows)
               {
                  anim(*item);
                  this->boardTex.draw(*item);
                  if (item->getStatus() == GemStatus::NONE)
                  {
                     withoutRenderGems++;
                  }
               }
            }

            if (withoutRenderGems == this->size * this->size)
            {
               this->renderSleep = true;
            }
         }
      }
   }
} // namespace gc_game