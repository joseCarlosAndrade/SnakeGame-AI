/* # Snake Game with neural networks being trained using genetic algorithm.

Author: jota ce (Github: joseCarlosAndrade)
2023
*/

#ifndef SDL_AI_SNAKE_H
#define SDL_AI_SNAKE_H

#include<SDL2/SDL.h>
#include<iostream>
#include"neural_network.hpp"
#include"snake.hpp"
#include<ctime>

//#define FPS 10 // it is not the snake movements fps, because the inputs are also based on this fps
#define SNAKE_INPUTS 28 // number of inputs
#define SNAKE_OUTPUTS 4 // number of outputs

#define SNAKE_TIMEOUT 100 // seconds that a snake can be alive without getting food

/* Defines the snake behavior.
`RANDOM` random movements from all snakes
`NEURAL_NETWORK` each snake will take decision about its direction from the neural network */
typedef enum {RANDOM, NEURAL_NETWORK} snake_behavior;

/* ### Game core class. 
It handles graphics display and processing using SDL2 and the snake game itself.
It's also responsible for invoking the neural network operations and fitness evaluation of every snake per iteration and
for calling crossover and mutation methods from `NeuralNetwork::NetworkContainer` class. */
class Game {
    private:
        // game variables
        SDL_Renderer * renderer;
        SDL_Window * window;
        int width, height, w_squares, h_squares;
        int universal_s;
        int FPS;

        // loop variables
        bool running, fullscreen, stopSimulation;
        int maxIterations;

        // sdl game variables
        int frameCount, timerFPS, lastFrame, fps;
        int lastTime;

        // snake and network variables
        snake_behavior behavior; // type of behavior, RANDOM or NEURAL_NETWORK

        NeuralNetwork::CONTAINER_SIZE n_snakes; // number of snakes on each iteration

        std::vector<SnakeGame::Snake> snakes; // vector that stores all snake objects

        NeuralNetwork::NetworkContainer *snakeContainer; // container that stores all snakes neural networks

        NeuralNetwork::NetworkContainer *nextSnakeContainer; // container that stores all snakes neural network for the next iteration

        SnakeGame::SNAKE_VIEW_AREA snake_view; // odd number that represents the size of the area that the snake can view

        std::map<int , SnakeGame::Direction> directionMap; // map that translate the neural network ouput to snake directions

        unsigned int bestSnakesNumber; // number of snakes to be chosen as best players on each iteration 

        int deadSnakes; // number of dead snakes to end each iteration

        int iterationCount; // number of iterations

        std::string lastBrain; // path to the last brain saved

        std::string pathToSave; // path to save current brain
        
    
    public:
        Game(SnakeGame::SNAKE_VIEW_AREA view, snake_behavior bh, 
        unsigned int best_snake_number =1, int g_height = 500, int g_width = 500, int gw_squares = 25, int gh_squares = 25) ;
        ~Game();

        void setMaxIterations(int iterations);

        /* Initializes all snakes with random networks */
        void initSnakes(NeuralNetwork::CONTAINER_SIZE n_s);

        /* Handles window & keyboard inputs */
        void input();

        /* Updates everything on the screen (frame) */
        void update();

        /* Updates all snakes (it's called in update()) */
        void updateSnakes();

        /* Puts eveything that has been updated into the screen */
        void draw();

        /* Does one single iteration of the training process.
        In other words, runs one single round with the pre defined game settings, saving the best networks
        through evolutionary algorithms. */
        bool iterateOnce(bool saveToFile=true);

        /* Plays the game! It uses the specified settings so far, such as snake numbers,
        max iteration count, crossover type, mutation and so on.
        Please make sure to specify a maximum iteration count before calling this method. */
        void playGame(uint n_snakes );

        int calculateFps(); // TODO: FIX THIS IT DOESNT MAKE ANY SENSE AND ITS CLEARLY NOT WORKING PROPERLY

        /* Handles the food positioning (called from draw()) */
        void drawFood(int step, SnakeGame::Food * food);

        /* Handles the snakes positioning (called from draw()) */
        void drawSnakes(int step, SnakeGame::Snake &snake, int i);

        /* Spawns a single snake with the file path to the brain to be used.
        If left as default, uses a random neural network */
        void spawnSingleSnakeAI(std::string brain_file_path = "!");


        //TODO:
        // void makeSelection();
        // void doCrossover();
        // void resetSnakesAndNN();
};

// /* Data type to handle snake fitness selection */
// typedef struct snakeProperties {
//     int fitness;
//     int index;
//     SnakeGame::Snake *snake;
// } SnakeProperties;

#endif