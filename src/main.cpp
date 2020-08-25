#include <string>
#include <climits>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include "game.hpp"

int main()
{
   try
   {
      char hostname[HOST_NAME_MAX];
      char username[LOGIN_NAME_MAX];
      gethostname(hostname, HOST_NAME_MAX);
      getlogin_r(username, LOGIN_NAME_MAX);
      std::string playerName = std::string(username) + '@' + std::string(hostname);
      gc_game::Game::getHandler().setPlayerName(playerName);
      gc_game::Game::getHandler().run();
   }
   catch (std::exception &ex)
   {
      std::cerr << "-> Sorry, an fatal error leads the program to be terminated :(\n"
                << "-> Here is the ERR_COMMET:\n=> "
                << ex.what() << '\n';
      exit(EXIT_FAILURE);
   }
   catch (...)
   {
      std::cerr << "-> Ooh, an UNKNOWN fatal error leads the program to be terminated :/\n"
                << "-> No ERR_COMMET availbale!\n";
      exit(EXIT_FAILURE);
   }
}