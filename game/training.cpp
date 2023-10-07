#include"../include/SDLSnakeGameAi.hpp"

int main(int argc, char ** argv) {
    int number_of_snakes, iterations;
    float mutation_f=0.1;
    float max_change = 0.1;

    if (argc < 2) { 
        // default brain
        std::cout << "usage: \n./bin/mainGame {number of snakes} {n of iterations (optional)} {mut factor (optional)} {max change (optional)}" <<std::endl;
        exit(1);
    }
    else if (argc >= 2) {

        // number of snakes
        try {
            number_of_snakes = atoi (argv[1]);
        }
        catch (std::exception const &e) {
            std::cerr << "Error: invalid number of snakes input. " << e.what() << std::endl;
        }
       
        iterations = 1;
        if (argc >= 3) {
            try {
                iterations = std::stoi(argv[2]);
            }
            catch (std::exception const &e) {
                std::cerr << "Error: invalid iterations input. " << e.what() << std::endl;
            }
        }

        if ( argc >= 4) {
            try {
                mutation_f = std::atof(argv[3]); 
            }
            catch (std::exception const &e) {
                std::cerr << "Error: invalid mutation input. " << e.what() << std::endl;
            }
        }

        if ( argc >= 5) {
            try {
                max_change = std::atof(argv[4]);
            }
            catch (std::exception const &e) {
                std::cerr << "Error: invalid max change input. " << e.what() << std::endl;
            }
        }
        }
    
   

    Game game( 5, NEURAL_NETWORK, 1, 650, 900, 50, 36);
    game.setMaxIterations((int)iterations);
    // bool saveToFile = true;
    
    // Iterate once
    // game.initSnakes(number_of_snakes);
    // game.iterateOnce(saveToFile);

    game.playGame(number_of_snakes, mutation_f, max_change);
}