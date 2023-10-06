#include"../include/SDLSnakeGameAi.hpp"

int main(int argc, char ** argv) {
    int number_of_snakes, iterations;


    if (argc < 2) { 
        // default brain
        std::cout << "usage: \n./bin/mainGame {number of snakes} {number of iterations (optional)}" <<std::endl;
        exit(1);
    }
    else if (argc >= 2) {

        // number of snakes
        try {
            number_of_snakes = atoi (argv[1]);
        }
        catch (std::exception const &e) {
            std::cerr << "Error: invalid input. " << e.what() << std::endl;
        }
       
        iterations = 1;
        if (argc > 2) {
        try {
            iterations = std::stoi(argv[2]);
        }
        catch (std::exception const &e) {
            std::cerr << "Error: invalid input. " << e.what() << std::endl;
        }
    }
    }
   

    Game game( 5, NEURAL_NETWORK, 1, 650, 900, 50, 36);
    game.setMaxIterations((int)iterations);
    // bool saveToFile = true;
    
    // Iterate once
    // game.initSnakes(number_of_snakes);
    // game.iterateOnce(saveToFile);

    game.playGame(number_of_snakes);
}