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
        bool running, fullscreen;

        int frameCount, timerFPS, lastFrame, fps;
        int lastTime;

        // snake and network variables
        snake_behavior behavior; // type of behavior, RANDOM or NEURAL_NETWORK

        NeuralNetwork::CONTAINER_SIZE n_snakes; // number of snakes on each iteration

        std::vector<SnakeGame::Snake> snakes; // vector that stores all snake objects

        NeuralNetwork::NetworkContainer *snakeContainer; // container that stores all snakes neural networks

        SnakeGame::SNAKE_VIEW_AREA snake_view; // odd number that represents the size of the area that the snake can view

        std::map<int , SnakeGame::Direction> directionMap; // map that translate the neural network ouput to snake directions

        unsigned int bestSnakesNumber; // number of snakes to be chosen as best players on each iteration 

        int deadSnakes; // number of dead snakes to end each iteration

        int iterationCount; // number of iterations

        std::string pathToSave;
        
    
    public:
        Game(NeuralNetwork::CONTAINER_SIZE n_s, SnakeGame::SNAKE_VIEW_AREA view, snake_behavior bh, 
        unsigned int best_snake_number =1, int g_height = 500, int g_width = 500, int gw_squares = 25, int gh_squares = 25) ;
        ~Game();

        void initSnakes();

        void input();
        void update();
        void updateSnakes();
        void draw();

        /* Does one single iteration of the training process.
        In other words, runs one single round with the pre defined game settings, saving the best networks
        through evolutionary algorithms. */
        void iterateOnce();

        int calculateFps(); // TODO: FIX THIS IT DOESNT MAKE ANY SENSE AND ITS CLEARLY NOT WORKING PROPERLY

        void drawFood(int step, SnakeGame::Food * food);

        void drawSnakes(int step, SnakeGame::Snake &snake, int i);


        bool saveToFile(NeuralNetwork::SingleNetwork &network);

        //TODO:
        // void makeSelection();
        // void doCrossover();
        // void resetSnakesAndNN();
};

/* Data type to handle snake selection */
typedef struct snakeProperties {
    SnakeGame::Snake *snake;
    int fitness;
    int index;
} SnakeProperties;

#endif