#include "board.hpp"

int main()
{
   sf::RenderWindow window(sf::VideoMode(800, 600), "Test window", sf::Style::Close);
   window.setFramerateLimit(60);
   window.requestFocus();
   sf::Event event;

   gc_game::BlackGem g1;
   gc_game::BlackGem g2;
   g2.getTransformable().move(sf::Vector2f(55.f, 0.f));
   gc_game::MoveAnim m(std::chrono::milliseconds(1000), 0.3f);

   sf::Texture gembox;
   gembox.loadFromFile("../assets/gembox.png");
   sf::Texture click;
   click.loadFromFile("../assets/click.png");
   sf::Sprite box;

   while (window.isOpen())
   {
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
      }
      window.clear(sf::Color::White);
      box.setTexture(gembox);
      box.setPosition(sf::Vector2f(0.f, 0.f));
      window.draw(box);
      box.setPosition(sf::Vector2f(55.f, 0.f));
      window.draw(box);
      window.draw(g1);
      window.draw(g2);
      box.setTexture(click);
      window.draw(box);
      window.display();

      if (g1.getTransformable().getPosition() != sf::Vector2f(80.f, 500.f))
      {
         if (m(g1.getTransformable(), sf::Vector2f(80.f, 500.f)))
         {
            m.reset();
         }
      }
   }
}