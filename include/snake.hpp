#ifndef SNAKE_H
#define SNAKE_H
#include<iostream>
#include<vector>
#include<map>
#include"neural_network.hpp"


namespace SnakeGame 
{
    typedef enum e_snake_direction {UP, DOWN, LEFT, RIGHT} Direction; // Snake direction
    typedef enum e_snake_state {ALIVE, DEAD, VICTORY} SnakeState; // Snake in game state
    typedef unsigned int SNAKE_VIEW_AREA; // View area that the snake may perceive

    /* Generic game object parent class that will be inherited by all child game elements */
    class GameObject {
        private:
        
            std::string name;

        public:
            int x, y;
            int getX();
            int getY(); 
            void setPosition(int x, int y);
            
    };

    /* Food object. It's actually that simple, really */
    class Food : public GameObject {
        public:
            Food(int x, int y);
            
    };

    /* The most complex snake you'll ever see. Represents what a snake is in-game. Each snake holds
    information regarding itself and its external attributes such as its brain (neural network), fitness value and so on. */
    class Snake : public GameObject {
        private:
            int maxW, maxH;
            int size;
            SNAKE_VIEW_AREA snakeView;
            Direction snakeDirection;
            Food* thisFood;

            NeuralNetwork::SingleNetwork * snakeBrain;
            std::map<int , SnakeGame::Direction> directionMap;
        

            void moveSnake();
            bool isCollidingBody(int x, int y) ;

        public:
            Snake * nxtSnake;
            bool isHead;
            SnakeState state;

            Snake(int x, int y, int maxW, int maxH, bool isHead, bool own_food=false, int fx = 0, int fy = 0, SNAKE_VIEW_AREA view = 5);

            // snake game functions
            void setSnakePosition(int x, int y);
            void setSnakeDirection(Direction direction);
            Direction getSnakeDir();
            void moveAllSnakes();
            void addSnake();
            void changeAllStates();
            
            SnakeState checkCollision();
            SnakeState checkCollision(SnakeGame::Food *food);

            Food * getFoodPtr();

            // neural network functions
            std::vector<float> getInputs();
            NeuralNetwork::SingleNetwork * getThisBrain();
            void setThisBrain(NeuralNetwork::SingleNetwork * single);
            void takeDecision();
            
    };



}
#endif