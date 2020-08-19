#include "window.hpp"

namespace gc_game
{
   Window::Window()
       : mainBoard(9), score(0), playerName("Unknown"), renderWin(sf::VideoMode(581, 681), "| Gem Castle |", sf::Style::Close), timerStart(std::chrono::system_clock::now())
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

      this->playerNameTxt.setString("player: " + this->playerName);
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

   void Window::updateScore() {}

   void Window::updateTimer()
   {
      time_t diffTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - std::chrono::system_clock::to_time_t(this->timerStart);
      char timeStr[32];
      strftime(timeStr, 32, "%H:%M:%S", std::gmtime(&diffTime));
      this->timerTxt.setString(timeStr);
   }

   void Window::setPlayerName(const std::string &pn)
   {
      this->playerName = pn;
      if (this->playerName.size() <= 20)
      {
         this->playerNameTxt.setString("player: " + this->playerName);
      }
      else
      {
         this->playerNameTxt.setString("player: " + this->playerName.substr(0, 17) + "...");
      }
   }

   Window &Window::getHandler()
   {
      static Window win;
      return win;
   }

   void Window::run()
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