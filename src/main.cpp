#include "Vector.h"
#include "Game.h"

int main(int argc, char* argv[])
{

    Game::CreateGame({ 10, 20 });
    Game::Get().Start();
    Game::DeleteGame();
    
    return 0;
}
