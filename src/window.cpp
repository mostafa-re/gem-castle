#include "window.hpp"

namespace gc_game
{
   Window::Window()
       : renderWin(sf::VideoMode(581, 681), "| Gem Castle |", sf::Style::Close), mainBoard(9)
   {
      if (!this->bgTex.loadFromFile("../assets/bg_image.png"))
      {
         throw std::runtime_error("Unable to open asset files!");
      }
      this->bgTex.setSmooth(true);
      this->bgTex.setRepeated(false);
      this->bgSpr.setTexture(this->bgTex);
      this->renderWin.setFramerateLimit(60);
      this->mainBoard.getTransformable().setPosition(42.f, 137.f);
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
         this->renderWin.display();
      }
   }
} // namespace gc_game