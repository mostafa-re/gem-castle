#include <string>
#include <climits>
#include <unistd.h>
#include "window.hpp"

int main()
{
   char hostname[HOST_NAME_MAX];
   char username[LOGIN_NAME_MAX];
   gethostname(hostname, HOST_NAME_MAX);
   getlogin_r(username, LOGIN_NAME_MAX);
   std::string playerName = std::string(username) + '@' + std::string(hostname);
   gc_game::Window::getHandler().setPlayerName(playerName);
   gc_game::Window::getHandler().run();
}