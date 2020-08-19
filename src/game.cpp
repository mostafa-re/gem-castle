#include "game.hpp"

namespace gc_game
{
   Game::Game()
       : mainBoard(9), score(0), renderWin(sf::VideoMode(581, 681), "| Gem Castle |", sf::Style::Close), timerStart(std::chrono::system_clock::now())
   {
      if (!this->bgTex.loadFromFile("../assets/bg_image.png") ||
          !this->font.loadFromFile("../assets/default_font.ttf"))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      this->bgTex.setSmooth(true);
      this->bgTex.setRepeated(false);
      this->bgSpr.setTexture(this->bgTex);

      this->renderWin.setFramerateLimit(60);
      this->mainBoard.getTransformable().setPosition(42.f, 137.f);

      this->setPlayerName("Unknown");
      this->playerNameTxt.setPosition(65, 65);
      this->playerNameTxt.setFont(this->font);
      this->playerNameTxt.setCharacterSize(22);
      this->playerNameTxt.setFillColor(sf::Color{127, 127, 127});

      this->scoreTxt.setString("score: " + std::to_string(this->score));
      this->scoreTxt.setPosition(65, 95);
      this->scoreTxt.setFont(this->font);
      this->scoreTxt.setCharacterSize(22);
      this->scoreTxt.setFillColor(sf::Color{127, 127, 127});

      this->timerTxt.setString("00:00:00");
      this->timerTxt.setPosition(475, 65);
      this->timerTxt.setFont(this->font);
      this->timerTxt.setCharacterSize(22);
      this->timerTxt.setFillColor(sf::Color{0, 0, 0});

      this->comboTxt.setString("");
      this->comboTxt.setPosition(475, 95);
      this->comboTxt.setFont(this->font);
      this->comboTxt.setCharacterSize(22);
      this->comboTxt.setFillColor(sf::Color{0, 0, 0});
   }

   void Game::updateScore()
   {
   }

   void Game::updateTimer()
   {
      time_t diffTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - std::chrono::system_clock::to_time_t(this->timerStart);
      char timeStr[MAX_TIMER_STRING];
      strftime(timeStr, sizeof timeStr, "%H:%M:%S", std::gmtime(&diffTime));
      this->timerTxt.setString(timeStr);
   }

   void Game::setPlayerName(const std::string &pn)
   {
      size_t charSize = pn.size();
      if (charSize <= MAX_PLAYER_NAME)
      {
         pn.copy(this->playerName, charSize, 0);
         this->playerName[charSize] = '\0';
      }
      else
      {
         pn.copy(this->playerName, 17, 0);
         this->playerName[17] = this->playerName[18] = this->playerName[19] = '.';
         this->playerName[20] = '\0';
      }
      this->playerNameTxt.setString("player: " + std::to_string(this->playerName));
   }

   Game &Game::getHandler()
   {
      static Game win;
      return win;
   }

   void Game::run()
   {
      this->renderWin.requestFocus();
      sf::Event event;
      while (this->renderWin.isOpen())
      {
         while (this->renderWin.pollEvent(event))
         {
            if (event.type == sf::Event::Closed)
            {
               this->renderWin.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
               this->mainBoard.mouseClick(event.mouseButton);
            }
         }
         this->renderWin.clear(sf::Color::White);
         this->renderWin.draw(this->bgSpr);
         this->renderWin.draw(this->mainBoard);
         this->renderWin.draw(this->playerNameTxt);
         this->updateTimer();
         this->renderWin.draw(this->timerTxt);
         this->updateScore();
         this->renderWin.draw(this->scoreTxt);
         this->renderWin.draw(this->comboTxt);
         this->renderWin.display();
      }
   }
} // namespace gc_game