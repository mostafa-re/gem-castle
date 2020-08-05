#include "black_gem.hpp"

int main()
{
   sf::RenderWindow window(sf::VideoMode(800, 600), "Test window", sf::Style::Close);
   window.setFramerateLimit(60);
   window.requestFocus();
   sf::Event event;

   gc_game::BlackGem g1;
   gc_game::BlackGem g2;
   g1.move(sf::Vector2f(10.f, 10.f));
   g2.move(sf::Vector2f(80.f, 10.f));

   sf::Texture t;
   t.loadFromFile("../assets/click.png");
   sf::Sprite s;
   s.setTexture(t);
   s.move(sf::Vector2f(5.f, 5.f));

   while (window.isOpen())
   {
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
      }
      window.clear(sf::Color::White);
      window.draw(g1);
      window.draw(g2);
      window.draw(s);
      window.display();
   }
}