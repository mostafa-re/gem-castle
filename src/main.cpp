#include "black_gem.hpp"
#include "move_anim.hpp"

int main()
{
   sf::RenderWindow window(sf::VideoMode(800, 600), "Test window", sf::Style::Close);
   window.setFramerateLimit(60);
   window.requestFocus();
   sf::Event event;

   gc_game::BlackGem g1;
   gc_game::BlackGem g2;
   g2.move(sf::Vector2f(55.f, 0.f));
   gc_game::MoveAnim m(std::chrono::milliseconds(1000), 0.3f);

   sf::Texture t;
   t.loadFromFile("../assets/click.png");
   sf::Sprite s;
   s.setTexture(t);

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

      if (s.getPosition() != sf::Vector2f(80.f, 500.f))
      {
         if (m(g1, sf::Vector2f(80.f, 500.f)) && m(s, sf::Vector2f(80.f, 500.f)))
         {
            m.reset();
         }
      }
   }
}