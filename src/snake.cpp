#include"../include/snake.hpp"
#include<ctime>
#include<cstdlib>
#include<eigen3/Eigen/Dense>

int SnakeGame::GameObject::getX() {
    return this->x;
}

int SnakeGame::GameObject::getY() {
    return this->y;
}

void SnakeGame::GameObject::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    
}

// constructor, initializes a snake object.
SnakeGame::Snake::Snake(int x, int y, int maxW, int maxH, bool isHead, bool own_food, int fx , int fy, SnakeGame::SNAKE_VIEW_AREA view ) 
 {
    // sets initial position
    Snake::setPosition(x, y);
    if (view%2==0) {
        std::cout << "Snake view size must be an odd value. " << std::endl;
        exit(EXIT_FAILURE);
    }
    this->snakeView = view;
    this->maxH = maxH;
    this->maxW = maxW;
    this->snakeDirection = RIGHT;
    this->isHead = isHead;
    this->state = SnakeGame::ALIVE;

    if (isHead)
        this->nxtSnake = new SnakeGame::Snake(x-1, y, maxW, maxH, false);

    else 
        this->nxtSnake = NULL;

    
    if (own_food) this->thisFood = new SnakeGame::Food(fx, fy);

    this->directionMap[0] = SnakeGame::RIGHT;
    this->directionMap[1] = SnakeGame::DOWN;
    this->directionMap[2] = SnakeGame::LEFT;
    this->directionMap[3] = SnakeGame::UP;
}

// changes snake direction if not forbidden
void SnakeGame::Snake::setSnakeDirection(Direction direction) {
    

    switch (this->snakeDirection)
    {
    case SnakeGame::UP:
        if (direction == SnakeGame::DOWN) return;
        break;
    
    case SnakeGame::DOWN:
        if (direction == SnakeGame::UP) return;
        break;

    case SnakeGame::LEFT:
        if (direction == SnakeGame::RIGHT) return;
        break;

    case SnakeGame::RIGHT:
        if (direction == SnakeGame::LEFT) return;
        break;
    }
    
    this->snakeDirection = direction;
}

// returns snake direction
SnakeGame::Direction SnakeGame::Snake::getSnakeDir() {
    return this->snakeDirection;
}

// moves the entire snake body
void SnakeGame::Snake::moveAllSnakes() {

    this->moveSnake();

    if (this->nxtSnake == NULL) return;

    this->nxtSnake->moveAllSnakes();
    this->nxtSnake->setSnakeDirection(this->snakeDirection);

}

// moves the part one block at direction && handles wall collision
void SnakeGame::Snake::moveSnake() {
    switch (this->snakeDirection)
    {
    case RIGHT:
        if (this->x == this->maxW-1)  {this->state = DEAD ;return;};
            this->x++;
        break;
    
    case UP:
        if (this->y ==0) {this->state = DEAD ;return;};
            this->y--;
        break;
    
    case LEFT:
        if (this->x ==0) {this->state = DEAD ;return;};
        this->x--;
        break;

    case DOWN:
        if (this->y == this->maxH-1) {this->state = DEAD ;return;};
        // std::cout << this->y << " " << this->maxH-1<< std::endl;
        this->y++;
        break;
    }
}

// adds a body part to the snake 
void SnakeGame::Snake::addSnake() {
    if (this->nxtSnake!=NULL) this->nxtSnake->addSnake();
    if(this->nxtSnake==NULL) {
    int cx, cy;

    switch (this->snakeDirection)
    {
    case SnakeGame::UP:
        cx = this->x;
        cy = this->y+1;
        break;
    
    case SnakeGame::DOWN:
        cx = this->x;
        cy = this->y-1;
        break;
    
    case SnakeGame::LEFT:
        cx = this->x+1;
        cy = this->y;
        break;
    
    case SnakeGame::RIGHT:
        cx = this->x-1;
        cy = this->y;
        break;
    }
    this->nxtSnake = new SnakeGame::Snake(cx, cy,  this->maxW, this->maxH, false);
    this->nxtSnake->snakeDirection = this->snakeDirection;

    // std::cout << "Adding new snake " << std::endl;
    }
}

// handle collision in general for a given food
SnakeGame::SnakeState SnakeGame::Snake::checkCollision(SnakeGame::Food* food) {
    // checks collision between its body
    // checks collision between food and snake    

    if (this->isHead && this->x == food->x && this->y == food->y) {
        this->addSnake();
        std::srand(std::time(nullptr));
        int x, y;
        do {  
            x = std::rand() % this->maxW;
            y = std::rand() % this->maxH;
        } while (isCollidingBody(x, y));
        food->setPosition(x, y);
    }
    SnakeGame::Snake* t_snake = this;
    do {
        t_snake = t_snake->nxtSnake;
        if (this->isHead&& this->x == t_snake->x && this->y== t_snake->y) {
            this->state = DEAD;
            return SnakeGame::DEAD;
        }
        
    } while (t_snake->nxtSnake != NULL);

    return SnakeGame::ALIVE;
}

// handle collision in general for its own food (overload so it can run with multiple instances of snakes)
SnakeGame::SnakeState SnakeGame::Snake::checkCollision() {
    // if head touched food
    if (this->isHead && this->x == thisFood->x && this->y == thisFood->y) {
        this->addSnake();
        std::srand(std::time(nullptr));
        int x, y;
        do {  
            x = std::rand() % this->maxW;
            y = std::rand() % this->maxH;
        } while (isCollidingBody(x, y));
        thisFood->setPosition(x, y);
    }

    // if head touched its body
    SnakeGame::Snake* t_snake = this;
    do {
        t_snake = t_snake->nxtSnake;
        if (this->isHead&& this->x == t_snake->x && this->y== t_snake->y) {
            this->state = SnakeGame::DEAD;
            return SnakeGame::DEAD;
        }
        
    } while (t_snake->nxtSnake != NULL);

    return SnakeGame::ALIVE; 
}

// returns a ptr to its food, since its a private attribute
SnakeGame::Food * SnakeGame::Snake::getFoodPtr() {
    return thisFood;
}

// ensures that new food location is not inside the snakes body
bool SnakeGame::Snake::isCollidingBody(int x, int y) {
    if (this->x == x && this->y==y) return true;
    if (this->nxtSnake == NULL) return false;
    return this->nxtSnake->isCollidingBody(x, y);
    // std::cout << "false " ;
    
}

// changes all body to dead
void SnakeGame::Snake::changeAllStates() {
    this->state = SnakeGame::DEAD;
    if (this->nxtSnake != NULL) this->nxtSnake->changeAllStates();
}

// gets a vector of inputs to feed nn
std::vector<float> SnakeGame::Snake::getInputs() {
    std::vector<float> inputs;
    inputs.push_back(thisFood->getX() - this->x ); // distance x to food
    inputs.push_back(thisFood->getY() - this->y ); // distance y to food

    // subtraction factor (check my notes for a better explanation!) but baically it transforms the base to the new snake_view
    // region.
    int d = snakeView/2; // if 5 -> v = 2
    
    Eigen::MatrixXd mat = Eigen::MatrixXd::Zero(snakeView, snakeView);

    auto thisSnake = this;

    while (1)  {
        int x_c = thisSnake->getX() - this->getX() + d;
        int y_c = thisSnake->getY() - this->getY() + d;
        
        if ((x_c >= 0 && x_c < snakeView ) && (y_c >= 0 && y_c < snakeView )) mat(x_c, y_c) = 1;

        if (thisSnake->nxtSnake == NULL) break;
        thisSnake = this->nxtSnake;
    } 
    
    // mat.transposeInPlace(); // transpose because eigen uses column > row order for indexing by default, which is not what i want
    for ( int i = 0; i < snakeView*snakeView; i ++) {
        // if (i==d*snakeView+d) continue; // skips head position, since it will always be a body part, its not necessary to feed nn with it
        inputs.push_back(mat(i));
    }

    return inputs;
}

void SnakeGame::Snake::setThisBrain(NeuralNetwork::SingleNetwork* single) {
    snakeBrain = single;
}

void SnakeGame::Snake::takeDecision() {
    // changes dierction based on output from neural network
    auto output = snakeBrain->calculateOutput(this->getInputs());
    int greater = 0;
    float greaterNum = 0;
    for (int i = 0; i<4; i++) {
        if (output[i] > greaterNum) {
            greaterNum = output[i];
            greater = i;
        }
    }
    std::cout << "outputs:  ";
    for (auto i : output) std::cout << i<< " ";
    std::cout  << std::endl;
    std::cout << "decision took: "<< directionMap[greater] << std::endl << std::endl;
    this->setSnakeDirection(directionMap[greater]);
}


// ifood
SnakeGame::Food::Food(int x, int y) {

    this->x = x;
    this->y = y;
}

