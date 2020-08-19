#include "game.hpp"

namespace gc_game
{
   Game::Game()
       : mainBoard(9), score(0), timerStart(std::chrono::system_clock::now()), renderWin(sf::VideoMode(581, 681), "| Gem Castle |", sf::Style::Close)
   {
      if (!this->font.loadFromFile("../assets/default_font.ttf") ||
          !this->redResumeBottunTex.loadFromFile("../assets/red_resume_button.png") ||
          !this->resumeBottunTex.loadFromFile("../assets/resume_button.png") ||
          !this->pauseBottunTex.loadFromFile("../assets/pause_button.png") ||
          !this->resetButtonTex.loadFromFile("../assets/reset_button.png") ||
          !this->bgTex.loadFromFile("../assets/bg_image.png"))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      this->renderWin.setFramerateLimit(60);
      this->mainBoard.getTransformable().setPosition(42.f, 137.f);

      this->redResumeBottunTex.setSmooth(true);
      this->redResumeBottunTex.setRepeated(false);
      this->resumeBottunTex.setSmooth(true);
      this->resumeBottunTex.setRepeated(false);
      this->pauseBottunTex.setSmooth(true);
      this->pauseBottunTex.setRepeated(false);
      this->resetButtonTex.setSmooth(true);
      this->resetButtonTex.setRepeated(false);
      this->bgTex.setSmooth(true);
      this->bgTex.setRepeated(false);

      this->resumeOrPauseButtonSpr.setTexture(this->pauseBottunTex);
      this->resetBottunSpr.setTexture(this->resetButtonTex);
      this->bgSpr.setTexture(this->bgTex);

      this->resumeOrPauseButtonSpr.setPosition(526, 14);
      this->resetBottunSpr.setPosition(476, 15);

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
      size_t point;
      size_t comboIndicator;
      if (this->mainBoard.getPoint(point))
      {
         this->score = point;
         this->scoreTxt.setString("score: " + std::to_string(this->score));
      }
      if (this->mainBoard.getComboIndicator(comboIndicator))
      {
         if (comboIndicator)
         {
            this->comboTxt.setString(std::to_string(static_cast<unsigned long>(powl(2, comboIndicator))) + "X Comb" + std::string(comboIndicator, 'o'));
            this->comboTxt.setPosition(473 + this->timerTxt.getLocalBounds().width - this->comboTxt.getLocalBounds().width, 95);
         }
         else
         {
            this->comboTxt.setString("");
         }
      }
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
      this->playerNameTxt.setString("player: " + std::string(this->playerName));
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
         this->updateTimer();
         this->updateScore();
         this->renderWin.clear(sf::Color::White);
         this->renderWin.draw(this->bgSpr);
         this->renderWin.draw(this->timerTxt);
         this->renderWin.draw(this->scoreTxt);
         this->renderWin.draw(this->comboTxt);
         this->renderWin.draw(this->mainBoard);
         this->renderWin.draw(this->playerNameTxt);
         this->renderWin.draw(this->resetBottunSpr);
         this->renderWin.draw(this->resumeOrPauseButtonSpr);
         this->renderWin.display();
      }
   }
} // namespace gc_game