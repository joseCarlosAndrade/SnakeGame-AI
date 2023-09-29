#include"../include/SDLSnakeGameAi.hpp"

int main(int argc, char ** argv) {
    int number_of_snakes;

    if (argc < 2) number_of_snakes= 20;
    else number_of_snakes = atoi (argv[1]);

    Game game(number_of_snakes, 5, NEURAL_NETWORK, 1, 650, 900, 50, 36);
    game.initSnakes();

    game.iterateOnce();
}