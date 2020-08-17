#include "board.hpp"

int main()
{
   sf::RenderWindow window(sf::VideoMode(581, 681), "Test window", sf::Style::Close);
   window.setFramerateLimit(60);
   window.requestFocus();
   sf::Event event;

   sf::Texture t;
   t.loadFromFile("../assets/bg_image.png");
   sf::Sprite s;
   s.setTexture(t);

   gc_game::Board brd(9);
   brd.getTransformable().setPosition(42.f, 137.f);

   while (window.isOpen())
   {
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
         {
            window.close();
         }
         else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
         {
            brd.mouseClick(event.mouseButton);
         }
      }
      window.clear(sf::Color::White);
      window.draw(s);
      window.draw(brd);
      window.display();
   }
}