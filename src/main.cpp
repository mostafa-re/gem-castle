#include "board.hpp"

int main()
{
   sf::RenderWindow window(sf::VideoMode(800, 600), "Test window", sf::Style::Close);
   window.setFramerateLimit(60);
   window.requestFocus();
   sf::Event event;

   gc_game::Board b(9);
   b.getTransformable().move(sf::Vector2f(130.f, 30.f));

   while (window.isOpen())
   {
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
      }
      window.clear(sf::Color::White);
      window.draw(b);
      window.display();
   }
}