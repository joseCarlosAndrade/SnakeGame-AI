#include"../include/SDLSnakeGameAi.hpp"

int main(int argc, char ** argv) {
    Game game(10, 5, NEURAL_NETWORK, 650, 900, 50, 36);
    game.initSnakes();

    game.iterateOnce();
}