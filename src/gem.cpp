#include "gem.hpp"

using namespace gc_game;

Gem::Gem(const std::string &path, size_t point) : point(point)
{
   this->sourceTex.loadFromFile(path);
   this->setTexture(this->sourceTex);
}