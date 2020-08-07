#include "board.hpp"

int main()
{
   sf::RenderWindow window(sf::VideoMode(800, 600), "Test window", sf::Style::Close);
   window.setFramerateLimit(60);
   window.requestFocus();
   sf::Event event;

   gc_game::Board brd(9);
   brd.getTransformable().move(sf::Vector2f(130.f, 30.f));

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
      window.draw(brd);
      window.display();
   }
}