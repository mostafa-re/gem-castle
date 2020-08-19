#include "board.hpp"

namespace gc_game
{
   Board::Board(unsigned size) : size(size), renderDone(false), renderSleep(true)
   {
      if (this->size < 5)
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

   sf::Transformable &Board::getTransformable()
   {
      return this->boardSpr;
   }

   void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
   {
      std::unique_lock<std::timed_mutex> lock(this->renderMutex, std::chrono::microseconds(5));
      if (lock)
      {
         this->boardTex.display();
         this->boardSpr.setTexture(this->boardTex.getTexture());
      }
      target.draw(this->boardSpr, states);
   }

   Board::~Board()
   {
      {
         std::lock_guard<std::timed_mutex> lock(this->renderMutex);
         this->renderDone = true;
      }
      this->renderThread.join();
   }

   void Board::reset()
   {
      std::lock_guard<std::timed_mutex> lock(this->renderMutex);
      this->point = 0;
      this->refreshBoard();
      this->clearClickedGem();
      this->renderSleep = false;
   }

   bool Board::getPoint(size_t &p)
   {
      std::unique_lock<std::timed_mutex> lock(this->renderMutex, std::chrono::microseconds(1));
      if (lock)
      {
         p = this->point;
         return true;
      }
      return false;
   }

   bool Board::getComboIndicator(size_t &ci)
   {
      std::unique_lock<std::timed_mutex> lock(this->renderMutex, std::chrono::microseconds(1));
      if (lock)
      {
         ci = this->comboIndicator;
         return true;
      }
      return false;
   }

   void Board::mouseClick(const sf::Event::MouseButtonEvent &mouseButton)
   {
      std::lock_guard<std::timed_mutex> lock(this->renderMutex);

      if (this->renderSleep &&
          mouseButton.x > 0 && mouseButton.y > 0 &&
          mouseButton.x > this->boardSpr.getPosition().x &&
          mouseButton.y > this->boardSpr.getPosition().y &&
          mouseButton.x - this->boardSpr.getPosition().x < this->boardSpr.getLocalBounds().width &&
          mouseButton.y - this->boardSpr.getPosition().y < this->boardSpr.getLocalBounds().height)
      {
         auto justClickedGem = this->gemGrid[(mouseButton.x - this->boardSpr.getPosition().x) / 55][(mouseButton.y - this->boardSpr.getPosition().y) / 55];
         if (this->clickedGem)
         {
            if (this->swapGems(this->clickedGem, justClickedGem))
            {
               this->swapSpr.setPosition(this->clickedGem->getTransformable().getPosition());
               this->boardTex.draw(this->swapSpr);
               this->swapSpr.setPosition(justClickedGem->getTransformable().getPosition());
               this->boardTex.draw(this->swapSpr);
            }
            else
            {
               this->clearClickedGem();
            }
         }
         else
         {
            this->clickedGem = justClickedGem;
            this->clickSpr.setPosition(this->clickedGem->getTransformable().getPosition());
            this->boardTex.draw(this->clickSpr);
         }
      }
      else
      {
         this->clearClickedGem();
      }
      this->renderSleep = false;
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
      for (size_t j = 0; j < this->size; j++)
      {
         for (size_t i = 0; i < this->size; i++)
         {
            this->gemBoxSpr.setPosition((i * 55) + 1, (j * 55) + 1);
            this->boardTex.draw(this->gemBoxSpr);
         }
      }
   }

   void Board::reloadBoard()
   {
      bool endCol;
      bool foundMatch;
      for (ssize_t i = this->size - 1; i >= 0; i--)
      {
         endCol = false;
         for (ssize_t j = this->size - 1; j >= 0 && !endCol; j--)
         {
            if (this->gemGrid[i][j]->getStatus() == GemStatus::HIDE)
            {
               foundMatch = false;
               for (ssize_t k = j - 1; k >= 0 && !foundMatch; k--)
               {
                  if (this->gemGrid[i][k]->getStatus() != GemStatus::HIDE)
                  {
                     auto tempPos = this->gemGrid[i][j]->getPosition();
                     this->gemGrid[i][j]->setPosition(this->gemGrid[i][k]->getPosition());
                     this->gemGrid[i][k]->setPosition(tempPos);
                     this->gemGrid[i][k]->setStatus(GemStatus::MOVE);
                     std::swap(this->gemGrid[i][k], this->gemGrid[i][j]);
                     foundMatch = true;
                  }
               }
               if (!foundMatch)
               {
                  endCol = true;
               }
            }
         }
      }

      std::default_random_engine randGenerator(time(nullptr));
      std::uniform_int_distribution<unsigned> randDistrib6(0, 5);
      std::uniform_int_distribution<unsigned> randDistrib5(0, 4);
      std::uniform_int_distribution<unsigned> randDistrib4(0, 3);
      std::vector<unsigned short> set(6);
      unsigned short selected;

      bool topDuplicate, leftDuplicate;
      for (size_t j = 0; j < this->size; j++)
      {
         for (size_t i = 0; i < this->size; i++)
         {
            if (this->gemGrid[i][j]->getStatus() == GemStatus::HIDE)
            {
               set = {1, 2, 3, 4, 5, 6};
               topDuplicate = leftDuplicate = false;
               if (i > 1 &&
                   this->gemGrid[i - 2][j]->getStatus() == GemStatus::HIDE &&
                   this->gemGrid[i - 1][j]->getStatus() == GemStatus::HIDE &&
                   this->gemGrid[i - 1][j]->getID() == this->gemGrid[i - 2][j]->getID())
               {
                  leftDuplicate = true;
                  std::swap(set[this->gemGrid[i - 1][j]->getID() - 1], set[set.size() - 1]);
               }
               if (j > 1 &&
                   this->gemGrid[i][j - 2]->getStatus() == GemStatus::HIDE &&
                   this->gemGrid[i][j - 1]->getStatus() == GemStatus::HIDE &&
                   this->gemGrid[i][j - 1]->getID() == this->gemGrid[i][j - 2]->getID())
               {
                  topDuplicate = true;
                  if (leftDuplicate)
                  {
                     if (this->gemGrid[i - 1][j]->getID() == this->gemGrid[i][j - 1]->getID())
                     {
                        topDuplicate = false;
                     }
                     else
                     {
                        if (this->gemGrid[i][j - 1]->getID() == set.size())
                        {
                           std::swap(set[set[set.size() - 1] - 1], set[set.size() - 2]);
                        }
                        else
                        {
                           std::swap(set[this->gemGrid[i][j - 1]->getID() - 1], set[set.size() - 2]);
                        }
                     }
                  }
                  else
                  {
                     std::swap(set[this->gemGrid[i][j - 1]->getID() - 1], set[set.size() - 1]);
                  }
               }
               if (leftDuplicate && topDuplicate)
               {
                  selected = set[randDistrib4(randGenerator)];
               }
               else if (leftDuplicate || topDuplicate)
               {
                  selected = set[randDistrib5(randGenerator)];
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
               this->gemGrid[i][j]->setStatus(GemStatus::HIDE);
               this->gemGrid[i][j]->getTransformable().setPosition((i * 55) + 1, 55 * (static_cast<ssize_t>(j) - this->size) - 1);
               this->gemGrid[i][j]->setPosition(sf::Vector2f((i * 55) + 1, (j * 55) + 1));
            }
         }
      }

      for (size_t j = 0; j < this->size; j++)
      {
         for (size_t i = 0; i < this->size; i++)
         {
            if (this->gemGrid[i][j]->getStatus() == GemStatus::HIDE)
            {
               this->gemGrid[i][j]->setStatus(GemStatus::MOVE);
            }
         }
      }
   }

   void Board::refreshBoard()
   {
      std::default_random_engine randGenerator(time(nullptr));
      std::uniform_int_distribution<unsigned> randDistrib6(0, 5);
      std::uniform_int_distribution<unsigned> randDistrib5(0, 4);
      std::uniform_int_distribution<unsigned> randDistrib4(0, 3);
      std::vector<unsigned short> set(6);
      unsigned short selected;

      bool topDuplicate, leftDuplicate;
      for (size_t j = 0; j < this->size; j++)
      {
         for (size_t i = 0; i < this->size; i++)
         {
            set = {1, 2, 3, 4, 5, 6};
            topDuplicate = leftDuplicate = false;
            if (i > 1 && this->gemGrid[i - 1][j]->getID() == this->gemGrid[i - 2][j]->getID())
            {
               leftDuplicate = true;
               std::swap(set[this->gemGrid[i - 1][j]->getID() - 1], set[set.size() - 1]);
            }
            if (j > 1 && this->gemGrid[i][j - 1]->getID() == this->gemGrid[i][j - 2]->getID())
            {
               topDuplicate = true;
               if (leftDuplicate)
               {
                  if (this->gemGrid[i - 1][j]->getID() == this->gemGrid[i][j - 1]->getID())
                  {
                     topDuplicate = false;
                  }
                  else
                  {
                     if (this->gemGrid[i][j - 1]->getID() == set.size())
                     {
                        std::swap(set[set[set.size() - 1] - 1], set[set.size() - 2]);
                     }
                     else
                     {
                        std::swap(set[this->gemGrid[i][j - 1]->getID() - 1], set[set.size() - 2]);
                     }
                  }
               }
               else
               {
                  std::swap(set[this->gemGrid[i][j - 1]->getID() - 1], set[set.size() - 1]);
               }
            }
            if (leftDuplicate && topDuplicate)
            {
               selected = set[randDistrib4(randGenerator)];
            }
            else if (leftDuplicate || topDuplicate)
            {
               selected = set[randDistrib5(randGenerator)];
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
            this->gemGrid[i][j]->setStatus(GemStatus::MOVE);
            this->gemGrid[i][j]->getTransformable().setPosition((i * 55) + 1, 55 * (static_cast<ssize_t>(j) - this->size) - 1);
            this->gemGrid[i][j]->setPosition(sf::Vector2f((i * 55) + 1, (j * 55) + 1));
         }
      }
   }

   void Board::clearClickedGem()
   {
      if (this->clickedGem)
      {
         this->clickSpr.setPosition(0, -55);
         this->clickedGem = nullptr;
      }
   }

   bool Board::boardEvaluation()
   {
      bool haveNewResult = {false};
      size_t totalGems;
      size_t tempPoint;
      size_t exteraPoint3Gem = {0};
      size_t exteraPoint4Gem = {100};
      size_t exteraPoint5Gem = {200};
      size_t exteraPointMore5Gem = {400};

      for (size_t j = 0; j < this->size; j++)
      {
         for (size_t i = 0; i < this->size; i++)
         {
            if (i > 1 && this->gemGrid[i - 1][j]->getID() == this->gemGrid[i - 2][j]->getID() && this->gemGrid[i - 1][j]->getID() == this->gemGrid[i][j]->getID())
            {
               haveNewResult = true;
               if (i == this->size - 1 || this->gemGrid[i + 1][j]->getID() != this->gemGrid[i][j]->getID())
               {
                  totalGems = 3;
                  for (ssize_t k = i - 3; k >= 0; k--)
                  {
                     if (this->gemGrid[i][j]->getID() == this->gemGrid[k][j]->getID())
                     {
                        totalGems++;
                     }
                     else
                     {
                        break;
                     }
                  }
                  tempPoint = 0;
                  switch (totalGems)
                  {
                  case 3:
                     tempPoint += (this->gemGrid[i][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i - 1][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i - 1][j]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i - 2][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i - 2][j]->getPoint() : 0;
                     this->gemGrid[i][j]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i - 1][j]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i - 2][j]->setStatus(GemStatus::FADEOUT);
                     tempPoint += exteraPoint3Gem;
                     break;
                  case 4:
                     tempPoint += (this->gemGrid[i][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i - 2][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i - 2][j]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i - 3][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i - 3][j]->getPoint() : 0;
                     this->gemGrid[i][j]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i - 2][j]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i - 3][j]->setStatus(GemStatus::FADEOUT);
                     for (size_t k = 0; k < this->size; k++)
                     {
                        tempPoint += (this->gemGrid[i - 1][k]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i - 1][k]->getPoint() : 0;
                        this->gemGrid[i - 1][k]->setStatus(GemStatus::FADEOUT);
                     }
                     tempPoint += exteraPoint4Gem;
                     break;
                  case 5:
                     size_t k1;
                     size_t w1;
                     size_t k2;
                     size_t w2;
                     k1 = (i > 4) ? i - 4 : 0;
                     w1 = (j > 2) ? j - 2 : 0;
                     k2 = (i < this->size - 1) ? i + 1 : this->size;
                     w2 = (j < this->size - 3) ? j + 3 : this->size;
                     for (size_t k3 = k1; k3 < k2; k3++)
                     {
                        for (size_t w3 = w1; w3 < w2; w3++)
                        {
                           tempPoint += (this->gemGrid[k3][w3]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[k3][w3]->getPoint() : 0;
                           this->gemGrid[k3][w3]->setStatus(GemStatus::FADEOUT);
                        }
                     }
                     tempPoint += exteraPoint5Gem;
                     break;

                  default:
                     for (size_t k = 0; k < this->size; k++)
                     {
                        for (size_t w = 0; w < this->size; w++)
                        {
                           tempPoint += (this->gemGrid[k][w]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[k][w]->getPoint() : 0;
                           this->gemGrid[k][w]->setStatus(GemStatus::FADEOUT);
                        }
                     }
                     tempPoint += exteraPointMore5Gem;
                     break;
                  }
                  this->point += powl(2, this->comboIndicator) * tempPoint;
                  this->comboIndicator++;
               }
            }
            if (j > 1 && this->gemGrid[i][j - 1]->getID() == this->gemGrid[i][j - 2]->getID() && this->gemGrid[i][j - 1]->getID() == this->gemGrid[i][j]->getID())
            {
               haveNewResult = true;
               if (j == this->size - 1 || this->gemGrid[i][j + 1]->getID() != this->gemGrid[i][j]->getID())
               {
                  totalGems = 3;
                  for (ssize_t k = j - 3; k >= 0; k--)
                  {
                     if (this->gemGrid[i][j]->getID() == this->gemGrid[i][k]->getID())
                     {
                        totalGems++;
                     }
                     else
                     {
                        break;
                     }
                  }
                  tempPoint = 0;
                  switch (totalGems)
                  {
                  case 3:
                     tempPoint += (this->gemGrid[i][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i][j - 1]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j - 1]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i][j - 2]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j - 2]->getPoint() : 0;
                     this->gemGrid[i][j]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i][j - 1]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i][j - 2]->setStatus(GemStatus::FADEOUT);
                     tempPoint += exteraPoint3Gem;
                     break;
                  case 4:
                     tempPoint += (this->gemGrid[i][j]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i][j - 2]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j - 2]->getPoint() : 0;
                     tempPoint += (this->gemGrid[i][j - 3]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[i][j - 3]->getPoint() : 0;
                     this->gemGrid[i][j]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i][j - 2]->setStatus(GemStatus::FADEOUT);
                     this->gemGrid[i][j - 3]->setStatus(GemStatus::FADEOUT);
                     for (size_t k = 0; k < this->size; k++)
                     {
                        tempPoint += (this->gemGrid[k][j - 1]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[k][j - 1]->getPoint() : 0;
                        this->gemGrid[k][j - 1]->setStatus(GemStatus::FADEOUT);
                     }
                     tempPoint += exteraPoint4Gem;
                     break;
                  case 5:
                     size_t k1;
                     size_t w1;
                     size_t k2;
                     size_t w2;
                     k1 = (i > 2) ? i - 2 : 0;
                     w1 = (j > 4) ? j - 4 : 0;
                     k2 = (i < this->size - 3) ? i + 3 : this->size;
                     w2 = (j < this->size - 1) ? j + 1 : this->size;
                     for (size_t k3 = k1; k3 < k2; k3++)
                     {
                        for (size_t w3 = w1; w3 < w2; w3++)
                        {
                           tempPoint += (this->gemGrid[k3][w3]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[k3][w3]->getPoint() : 0;
                           this->gemGrid[k3][w3]->setStatus(GemStatus::FADEOUT);
                        }
                     }
                     tempPoint += exteraPoint5Gem;
                     break;

                  default:
                     for (size_t k = 0; k < this->size; k++)
                     {
                        for (size_t w = 0; w < this->size; w++)
                        {
                           tempPoint += (this->gemGrid[k][w]->getStatus() != GemStatus::FADEOUT) ? this->gemGrid[k][w]->getPoint() : 0;
                           this->gemGrid[k][w]->setStatus(GemStatus::FADEOUT);
                        }
                     }
                     tempPoint += exteraPointMore5Gem;
                     break;
                  }
                  this->point += powl(2, this->comboIndicator) * tempPoint;
                  this->comboIndicator++;
               }
            }
         }
      }
      return haveNewResult;
   }

   bool Board::boardCheckBlock()
   {
      bool isBoardBlocked = {true};

      for (size_t j = 0; j < this->size && isBoardBlocked; j++)
      {
         for (size_t i = 0; i < this->size && isBoardBlocked; i++)
         {
            if (i > 1 && this->gemGrid[i - 1][j]->getID() == this->gemGrid[i][j]->getID())
            {
               if (j > 0 && this->gemGrid[i - 2][j - 1]->getID() == this->gemGrid[i][j]->getID())
               {
                  isBoardBlocked = false;
               }
               else if (j < this->size - 1 && this->gemGrid[i - 2][j + 1]->getID() == this->gemGrid[i][j]->getID())
               {
                  isBoardBlocked = false;
               }
               else if (i > 2 && this->gemGrid[i - 3][j]->getID() == this->gemGrid[i][j]->getID())
               {
                  isBoardBlocked = false;
               }
            }
            if (isBoardBlocked)
            {
               if (j > 1 && this->gemGrid[i][j - 1]->getID() == this->gemGrid[i][j]->getID())
               {
                  if (i > 0 && this->gemGrid[i - 1][j - 2]->getID() == this->gemGrid[i][j]->getID())
                  {
                     isBoardBlocked = false;
                  }
                  else if (i < this->size - 1 && this->gemGrid[i + 1][j - 2]->getID() == this->gemGrid[i][j]->getID())
                  {
                     isBoardBlocked = false;
                  }
                  else if (j > 2 && this->gemGrid[i][j - 3]->getID() == this->gemGrid[i][j]->getID())
                  {
                     isBoardBlocked = false;
                  }
               }
               if (isBoardBlocked)
               {
                  if (i < this->size - 2 && this->gemGrid[i + 1][j]->getID() == this->gemGrid[i][j]->getID())
                  {
                     if (j > 0 && this->gemGrid[i + 2][j - 1]->getID() == this->gemGrid[i][j]->getID())
                     {
                        isBoardBlocked = false;
                     }
                     else if (j < this->size - 1 && this->gemGrid[i + 2][j + 1]->getID() == this->gemGrid[i][j]->getID())
                     {
                        isBoardBlocked = false;
                     }
                     else if (i < this->size - 3 && this->gemGrid[i + 3][j]->getID() == this->gemGrid[i][j]->getID())
                     {
                        isBoardBlocked = false;
                     }
                     if (isBoardBlocked)
                     {
                        if (j < this->size - 2 && this->gemGrid[i][j + 1]->getID() == this->gemGrid[i][j]->getID())
                        {
                           if (i > 0 && this->gemGrid[i - 1][j + 2]->getID() == this->gemGrid[i][j]->getID())
                           {
                              isBoardBlocked = false;
                           }
                           else if (i < this->size - 1 && this->gemGrid[i + 1][j + 2]->getID() == this->gemGrid[i][j]->getID())
                           {
                              isBoardBlocked = false;
                           }
                           else if (j < this->size - 3 && this->gemGrid[i][j + 3]->getID() == this->gemGrid[i][j]->getID())
                           {
                              isBoardBlocked = false;
                           }
                           if (isBoardBlocked)
                           {
                              if (i > 1 && this->gemGrid[i - 2][j]->getID() == this->gemGrid[i][j]->getID())
                              {
                                 if (j > 0 && this->gemGrid[i - 1][j - 1]->getID() == this->gemGrid[i][j]->getID())
                                 {
                                    isBoardBlocked = false;
                                 }
                                 else if (j < this->size - 1 && this->gemGrid[i - 1][j + 1]->getID() == this->gemGrid[i][j]->getID())
                                 {
                                    isBoardBlocked = false;
                                 }
                              }
                              if (isBoardBlocked)
                              {
                                 if (j > 1 && this->gemGrid[i][j - 2]->getID() == this->gemGrid[i][j]->getID())
                                 {
                                    if (i > 0 && this->gemGrid[i - 1][j - 1]->getID() == this->gemGrid[i][j]->getID())
                                    {
                                       isBoardBlocked = false;
                                    }
                                    else if (i < this->size - 1 && this->gemGrid[i + 1][j - 1]->getID() == this->gemGrid[i][j]->getID())
                                    {
                                       isBoardBlocked = false;
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
      if (isBoardBlocked)
      {
         this->refreshBoard();
      }
      return isBoardBlocked;
   }

   void Board::moveGems(Gem &gem, const float &moveFactor)
   {
      if (fabsf(gem.getPosition().x - gem.getTransformable().getPosition().x) < 1.f && fabsf(gem.getPosition().y - gem.getTransformable().getPosition().y) < 1.f)
      {
         gem.getTransformable().setPosition(gem.getPosition());
         gem.setStatus(GemStatus::NONE);
      }
      else
      {
         gem.getTransformable().move((gem.getPosition() - gem.getTransformable().getPosition()) * moveFactor);
      }
   }

   void Board::fadeoutGems(Gem &gem, const float &scaleFactor)
   {
      if (gem.getAbsoluteWidth() < 5.f && gem.getAbsoluteHeight() < 5.f)
      {
         gem.getTransformable().setScale(0, 0);
         gem.setStatus(GemStatus::HIDE);
      }
      else
      {
         gem.getTransformable().setScale(gem.getTransformable().getScale() * scaleFactor);
         gem.getTransformable().move(gem.getAbsoluteWidth() * (1.f / scaleFactor - 1.f) / 2, gem.getAbsoluteHeight() * (1.f / scaleFactor - 1.f) / 2);
      }
   }

   bool Board::swapGems(std::shared_ptr<Gem> gem1, std::shared_ptr<Gem> gem2)
   {
      if (gem1 == gem2)
      {
         return false;
      }
      if (fabsf(gem1->getPosition().x - gem2->getPosition().x) <= 1.f)
      {
         if (fabsf(gem1->getPosition().y - gem2->getPosition().y) >= 56.f)
         {
            return false;
         }
      }
      else if (fabsf(gem1->getPosition().y - gem2->getPosition().y) <= 1.f)
      {
         if (fabsf(gem1->getPosition().x - gem2->getPosition().x) >= 56.f)
         {
            return false;
         }
      }
      else
      {
         return false;
      }

      bool match = {false};

      size_t newXPosGem1 = gem2->getPosition().x / 55;
      size_t newYPosGem1 = gem2->getPosition().y / 55;
      size_t newXPosGem2 = gem1->getPosition().x / 55;
      size_t newYPosGem2 = gem1->getPosition().y / 55;

      std::swap(this->gemGrid[newXPosGem1][newYPosGem1], this->gemGrid[newXPosGem2][newYPosGem2]);

      size_t minXToSweep;
      size_t maxXToSweep;
      size_t minYToSweep;
      size_t maxYToSweep;

      if (newXPosGem1 > 2)
      {
         minXToSweep = newXPosGem1 - 2;
      }
      else
      {
         minXToSweep = 0;
      }
      if (newXPosGem1 < this->size - 3)
      {
         maxXToSweep = newXPosGem1 + 3;
      }
      else
      {
         maxXToSweep = this->size;
      }
      for (size_t i = minXToSweep + 2; i < maxXToSweep; i++)
      {
         if (this->gemGrid[i - 2][newYPosGem1]->getID() == this->gemGrid[i - 1][newYPosGem1]->getID() && this->gemGrid[i - 1][newYPosGem1]->getID() == this->gemGrid[i][newYPosGem1]->getID())
         {
            match = true;
         }
      }
      if (!match)
      {
         if (newYPosGem1 > 2)
         {
            minYToSweep = newYPosGem1 - 2;
         }
         else
         {
            minYToSweep = 0;
         }
         if (newYPosGem1 < this->size - 3)
         {
            maxYToSweep = newYPosGem1 + 3;
         }
         else
         {
            maxYToSweep = this->size;
         }
         for (size_t j = minYToSweep + 2; j < maxYToSweep; j++)
         {
            if (this->gemGrid[newXPosGem1][j - 2]->getID() == this->gemGrid[newXPosGem1][j - 1]->getID() && this->gemGrid[newXPosGem1][j - 1]->getID() == this->gemGrid[newXPosGem1][j]->getID())
            {
               match = true;
            }
         }
         if (!match)
         {
            if (newXPosGem2 > 2)
            {
               minXToSweep = newXPosGem2 - 2;
            }
            else
            {
               minXToSweep = 0;
            }
            if (newXPosGem2 < this->size - 3)
            {
               maxXToSweep = newXPosGem2 + 3;
            }
            else
            {
               maxXToSweep = this->size;
            }
            for (size_t i = minXToSweep + 2; i < maxXToSweep; i++)
            {
               if (this->gemGrid[i - 2][newYPosGem2]->getID() == this->gemGrid[i - 1][newYPosGem2]->getID() && this->gemGrid[i - 1][newYPosGem2]->getID() == this->gemGrid[i][newYPosGem2]->getID())
               {
                  match = true;
               }
            }
            if (!match)
            {
               if (newYPosGem2 > 2)
               {
                  minYToSweep = newYPosGem2 - 2;
               }
               else
               {
                  minYToSweep = 0;
               }
               if (newYPosGem2 < this->size - 3)
               {
                  maxYToSweep = newYPosGem2 + 3;
               }
               else
               {
                  maxYToSweep = this->size;
               }
               for (size_t j = minYToSweep + 2; j < maxYToSweep; j++)
               {
                  if (this->gemGrid[newXPosGem2][j - 2]->getID() == this->gemGrid[newXPosGem2][j - 1]->getID() && this->gemGrid[newXPosGem2][j - 1]->getID() == this->gemGrid[newXPosGem2][j]->getID())
                  {
                     match = true;
                  }
               }
            }
         }
      }

      if (match)
      {
         auto tempPos = gem1->getPosition();
         gem1->setPosition(gem2->getPosition());
         gem2->setPosition(tempPos);
         gem1->setStatus(GemStatus::SWAP);
         gem2->setStatus(GemStatus::SWAP);
         return true;
      }
      else
      {
         std::swap(this->gemGrid[newXPosGem1][newYPosGem1], this->gemGrid[newXPosGem2][newYPosGem2]);
         return false;
      }
   }

   void Board::render()
   {
      float moveFactorOfGems = {0.3f};
      float fadeoutScaleFactorOfGems = {0.7f};
      bool haveAnimation;
      bool haveToReload;

      std::unique_lock<std::timed_mutex> lock(this->renderMutex);
      while (!this->renderDone)
      {
         lock.unlock();
         std::this_thread::sleep_for(std::chrono::milliseconds(20));
         lock.lock();

         if (!this->renderSleep)
         {
            this->clearBoard();
            haveAnimation = false;
            haveToReload = false;
            for (auto &rows : this->gemGrid)
            {
               for (auto &item : rows)
               {
                  switch (item->getStatus())
                  {
                  case GemStatus::NONE:
                     this->boardTex.draw(*item);
                     break;

                  case GemStatus::MOVE:
                     this->moveGems(*item, moveFactorOfGems);
                     this->boardTex.draw(*item);
                     haveAnimation = true;
                     break;

                  case GemStatus::SWAP:
                     this->swapSpr.setPosition(item->getTransformable().getPosition());
                     this->moveGems(*item, moveFactorOfGems);
                     this->boardTex.draw(this->swapSpr);
                     this->boardTex.draw(*item);
                     haveAnimation = true;
                     break;

                  case GemStatus::FADEOUT:
                     this->fadeoutGems(*item, fadeoutScaleFactorOfGems);
                     this->boardTex.draw(*item);
                     haveAnimation = true;
                     break;

                  case GemStatus::HIDE:
                     haveToReload = true;
                     break;
                  default:
                     // std::out_of_range("Undifiend animation type detected!");
                     break;
                  }
               }
            }
            if (!haveAnimation)
            {
               if (haveToReload)
               {
                  this->reloadBoard();
               }
               else if (this->boardEvaluation())
               {
                  this->clearClickedGem();
               }
               else
               {
                  this->comboIndicator = 0;
                  if (!this->boardCheckBlock())
                  {
                     this->renderSleep = true;
                  }
               }
            }
            if (this->clickedGem)
            {
               if (clickedGem->getStatus() == GemStatus::SWAP)
               {
                  this->boardTex.draw(*this->clickedGem);
               }
               else
               {
                  this->boardTex.draw(this->clickSpr);
               }
            }
         }
      }
   }
} // namespace gc_game