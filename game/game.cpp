#include"../include/SDLSnakeGameAi.hpp"

int main(int argc, char ** argv) {
    int number_of_snakes;

    

    Game game( 5, NEURAL_NETWORK, 1, 650, 900, 50, 36);
    bool saveToFile = true;

    if (argc < 2) { 
        // default brain
        game.spawnSingleSnakeAI("data/temp/brain_0_Fri_Sep_29_15-50-21_2023.b");
        saveToFile = false;
    }
    else if (argc == 2) {
        // customized
        std::string chars;
        chars.insert(0, argv[1]);

        // number of snakes
        if (chars.length() < 10) {
            number_of_snakes = atoi (argv[1]);
            game.initSnakes(number_of_snakes);
        }
        else {
            std::string path = "data/temp/";
            game.spawnSingleSnakeAI( path + chars);
            saveToFile = false;
        }
    }

    game.iterateOnce(saveToFile);

}