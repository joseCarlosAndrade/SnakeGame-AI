#ifndef SNAKE_H
#define SNAKE_H
#include<iostream>
#include<vector>
#include<map>
#include"neural_network.hpp"


namespace SnakeGame 
{
    /* Generic game object parent class that will be inherited by all child game elements. */
    class GameObject {
        private:
        
            std::string name;

        public:
            int x, y;
            int getX();
            int getY(); 
            void setPosition(int x, int y);
            
    };

    /* Food object. It's actually that simple, really. */
    class Food : public GameObject {
        public:
            Food(int x, int y);
            
    };

    typedef enum e_snake_direction {UP, DOWN, LEFT, RIGHT} Direction; /* Snake directio. */
    typedef enum e_snake_state {ALIVE, DEAD, GROWING} SnakeState; /* Snake state on game. */
    typedef unsigned int SNAKE_VIEW_AREA; /* View area that the snake may perceive as input to neural networks. */

    /* The most complex snake you'll ever see. Represents what a snake is in-game. Each snake holds
    information regarding itself and its external attributes such as its brain (neural network), fitness value and so on. 
    The collision is also treated by the snake itself. */
    class Snake : public GameObject {
        private:
            /* Since the collision is handled by the snakes, it has to know the map size. */
            int maxW, maxH;

            /* Its current size. Each snake is a linked list, where the head points to the second body part and so on, until
            the last tail points to `NULL`. However, it's still important to keep track of the snake size to avoid recursive operations
            to get infinitely long or segmentation faults. */
            int size; 

            /* The area (odd number) that the snake head can see around its head. It's used as input to feed the neural network. */
            SNAKE_VIEW_AREA snakeView;

            /* Current body part direction, which maps to a int number. */
            Direction snakeDirection;

            /* This snakes food. Yes, each snake has its own food, so it doesn't have to treat collision with other snakes. */
            Food* thisFood;

            /* This snakes brain. It's a simples neural network with `SNAKE_INPUTS` number of inputs and `SNAKE_OUTPUTS` outputs. */
            NeuralNetwork::SingleNetwork * snakeBrain;

            /* Handles the direction mapping. */
            std::map<int , SnakeGame::Direction> directionMap;

            /* Each head have a timer, which is needed to kill snakes that don't eat for too long. */
            uint32_t snakeTimer;
        


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

            void moveSnake();
            bool isCollidingBody(int x, int y) ;

            uint32_t getSnakeTimer();
            void setSnakeTimer(uint32_t timer);
            
    };



}
#endif