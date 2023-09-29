#include<SDL2/SDL.h>
#include<iostream>
#include"../include/neural_network.hpp"
#include"../include/snake.hpp"
#include"../include/SDLSnakeGameAi.hpp"
#include<map>
#include<unistd.h>


Game::Game(NeuralNetwork::CONTAINER_SIZE n_s, SnakeGame::SNAKE_VIEW_AREA view, snake_behavior bh, 
        unsigned int best_snake_number, int g_height, int g_width, int gw_squares, int gh_squares) 
    : n_snakes(n_s), height(g_height), width(g_width), w_squares(gw_squares), h_squares(gh_squares), 
    snake_view(view), behavior(bh), bestSnakesNumber(best_snake_number)
    {
        // initializing container of neural networks
        snakeContainer = new NeuralNetwork::NetworkContainer(n_s, 27, 4);
        assert(snakeContainer != NULL);

        iterationCount = 0;
        pathToSave = "data/";

        // intializing game attributes
        running = 1;
        fullscreen = 0;
        lastTime = 0;
        FPS = 10;

        // snake map
        directionMap[0] = SnakeGame::RIGHT;
        directionMap[1] = SnakeGame::DOWN;
        directionMap[2] = SnakeGame::LEFT;
        directionMap[3] = SnakeGame::UP;

        universal_s = (int) 5*width/(6*w_squares-1);


        if(SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed to initialize sdl2" << std::endl;
        
        if(SDL_CreateWindowAndRenderer(width, height,  SDL_RENDERER_ACCELERATED, &window, &renderer) < 0) {
            std::cout << "Failed to create window and renderer" << std::endl;
            exit(1); 
        }

        SDL_SetWindowTitle(window, "my littles snakes");
        SDL_ShowCursor(SDL_TRUE);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    }

Game::~Game() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// intantiates all snakes
void Game::initSnakes() {
    // snakes boiiii (and their foods)
    std::srand(std::time(nullptr));
    int food_x, food_y;

    this->deadSnakes = 0;
    snakes.reserve(n_snakes);

    for ( int i = 0; i < n_snakes; i++) {
        food_x = std::rand() % w_squares;
        food_y = std::rand() % h_squares;
        SnakeGame::Snake c_snake(w_squares/2, h_squares/2, w_squares, h_squares, true, true, food_x, food_y, snake_view);
        c_snake.setThisBrain(snakeContainer->getNeuralNetwork(i));
        assert(c_snake.getThisBrain() != NULL);
        snakes.push_back(c_snake);
    }
}

// proccess input events, like quitting, reseting
void Game::input() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) running = false;
        
        const Uint8*keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_ESCAPE]) running =false; // quits
       
        if (keystates[SDL_SCANCODE_LEFT] && FPS > 3) FPS--;
        else if (keystates[SDL_SCANCODE_RIGHT] && FPS < 50) FPS++;
    }
    
}

void Game::update() {
    // not really necessary for this
}

// update all snakes on game
void Game::updateSnakes() {
        std::cout << " Entering update snakes.. ";
        int index = 0;
        SnakeGame::Direction thisDirecion;

        int i =0;
        // for(auto& snake : snakes) {

        for ( std::vector<SnakeGame::Snake>::iterator it = snakes.begin(); it != snakes.end() ; it++) {
            
            // usleep(1000000 / 200);
            
            SnakeGame::Snake *snake = it.base();
        // }
            
            if (i++ >= n_snakes) break; // for safety
            
            if (snake->state == SnakeGame::DEAD) {
                // snake_iterator++;
                // ++it;
                continue;
            }
            
            
            if (behavior == RANDOM) {
                index = std::rand() % 4;
                thisDirecion = directionMap[index];
                snake->setSnakeDirection(thisDirecion);
            }
            else {
                // 
                snake->takeDecision();
// 
                // 
                }
                        
            // }
            
            // std::cout << i++ << " " << map[i] << std::endl;
             // all moving down for now

             // **** ia role
            
            // lastSnakeTimer = snakeTimer;
            if (snake->state!=SnakeGame::DEAD) {
                snake->moveAllSnakes();
                snake->checkCollision();

                // std::cout << " OPS! THIS DIED ";
                if(snake->state==SnakeGame::DEAD) {
                    this->deadSnakes++;
                    snake->changeAllStates();
                }
            }
            // delete snake;
            // snake = NULL;

            // snake_iterator++;
            // ++it;
        }   

        std::cout << " !!! updated all snakes!!!!  ";
   
}

// main loop for one round
void Game::iterateOnce() {
    //loop variables
    int drawTimer = 0;
    int snakeTimer = 0, lastSnakeTimer = 0;
    int first = 1;

    
    // std::srand(std::time(nullptr));
    while(running) {
        // int a = std::rand();
        // std::cout << std::endl<< a << " should match ";

        SDL_Delay(10);
        // std::cout << a << std::endl;
        if (this->deadSnakes == n_snakes) { 
            std::cout << this->deadSnakes << " snakes dead. Game over" << std::endl;
            break;
        }
        snakeTimer++;
        int fps = this->calculateFps();


        this->input();
        // this->update();
        if (snakeTimer - lastSnakeTimer >= fps/6 && snakeTimer > FPS + 10) {
            std::cout << "updating all of them:" <<std::endl;

            // int i = 0;
            // usleep(1000000*2);

            // for(int i = 0; i < n_snakes; i ++) {
                // std::cout << "nn : " << i << " "; 
                // auto network = snakeContainer->getNeuralNetwork(i);
            // for (auto &network : snakeContainer->getNeuralNetworks()) {
                
                // if (snakes[i].state != SnakeGame::DEAD) {
                //     auto inputs = snakes[i].getInputs(); // PROBLEM IS HERE
                //     // std::vector<float> inputs;
                //     // for ( int o = 0; o < 27; o++) {
                //     //     inputs.push_back(std::rand() % 10);

                //     // }
                //     auto outputs = network->calculateOutput(inputs);
                //     for ( auto e : inputs) {
                //         std::cout << e << " ";
                //     }
                //     std::cout << std::endl;
                    
                // }
                // auto outputs = network->calculateOutput(inputs);
                // std::cout << i++ << ": ";
    
                
                // std::cout << std::endl;
                // i++;
                // }


            this->updateSnakes();
 

            lastSnakeTimer = snakeTimer;
        }
        std::cout << "Drawing snakes: " ;
        this->draw();
        std::cout << n_snakes - this->deadSnakes << " snakes drawn." << std::endl;
    }
    std::cout << "loop terminated" << std::endl;


    // evaluate fitness from all snake neural networks (choose best N_BEST_SNAKES)
    SnakeProperties *best_snake_structs[100];
    // for ( int j = 0; j < bestSnakesNumber; j ++) {
        // std::cout << "creating property for snake: " << j << std::endl;
        // (best_snake_structs)[j] = {NULL, 0, 0}; // initializing all with 0 and NULL   
        // best_snake_structs[j]->fitness = 0; // intializing all fitness as 0
    // }
    std::cout << "struct created" << std::endl;

    
    for (int i = 0; i < bestSnakesNumber; i++) { 
        int cur_index = 0;
        for (auto sn : snakes) {
            int this_fitness = sn.getThisBrain()->fitnessOperation(NeuralNetwork::GET); // all of this to access the snake brain fitness..
            if (this_fitness > best_snake_structs[i]->fitness) {
                // *best_snake_structs[i] = {&sn, this_fitness, cur_index};
                best_snake_structs[i]->snake = &sn;
                best_snake_structs[i]->fitness = this_fitness;
                best_snake_structs[i]->index = cur_index;
            }
            cur_index++;
        }
    }
    std::cout << "fitness evaluated" << std::endl;

    // save networks from best N_BEST_SNAKES snakes on file 
    for ( int i = 0; i < bestSnakesNumber; i ++) {
        std::cout << "saving snake " << i << " to file" << std::endl;
        NeuralNetwork::SingleNetwork *itBrain = best_snake_structs[i]->snake->getThisBrain();
        saveToFile(* itBrain );
    }
     
    // do crossover with CROSSOVER_PROBABILITY and LEARNING_RATE to repopulate neural network container

    std::cout << "snake file saved" << std::endl;
    // deallocate snakes
    std::vector<SnakeGame::Snake>().swap(this->snakes);
    
    std::cout << "incrementing iteration count " << std::endl;
    iterationCount++;
}

bool Game::saveToFile(NeuralNetwork::SingleNetwork &network) {
    // save network to file

    // weights
    return true;
}

int Game::calculateFps() {
    lastFrame = SDL_GetTicks();
        if (lastFrame >= lastTime+1000) {
            // a second has passed
            lastTime=lastFrame;
            fps=frameCount;
            frameCount = 0;
        }

        frameCount++;
        int timerFPS = SDL_GetTicks() - lastFrame;
        if(timerFPS<1000/FPS) {
            SDL_Delay(1000/FPS - timerFPS);
        }
        return fps;
}



void Game::draw() { // TODO: OPTIMIZE SO THAT IT DOESNT REDRAW ON A PIECE THAT IS ALREADY OCCUPIED
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 0);

    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = width;
    rect.h = height;
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
 
    // it automates the square distribution based on the settings specified on the define area
    // trust me it works
    int step = (int)(6*universal_s)/5 ;

    rect.w = rect.h = (int)universal_s;

    for ( int i = 0; i < w_squares; i++) {
        
        for (int j = 0 ; j < h_squares; j++ ){
            rect.x = step*i + universal_s/5; 
            rect.y = step*j + universal_s/5;
        
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    // drawing each snake

    // std::vector<SnakeGame::Snake>::iterator snake_iterator = snakes.begin();

    // while(snake_iterator != snakes.end()) {
    for ( auto &snake : this->snakes) {
        if (snake.state == SnakeGame::DEAD) continue;
        drawSnakes(step, snake, 0);
        // if snake not dead, draw its food
        if (snake.state != SnakeGame::DEAD) drawFood(step, snake.getFoodPtr());
        // snake_iterator++;
    }

    // puts everything into screen
    SDL_RenderPresent(renderer);
}


void Game::drawFood(int step, SnakeGame::Food *food) {
    SDL_SetRenderDrawColor(renderer, 230, 0, 50, 255);
    SDL_Rect food_r;
    food_r.w = food_r.h = (int)universal_s;
    food_r.x = food->x * step + universal_s/5;
    food_r.y = food->y * step + universal_s/5;
    SDL_RenderFillRect(renderer, &food_r);
}

// draw snakes recursively
void Game::drawSnakes(int step, SnakeGame::Snake &snake, int i) {
        if (i > 100) i = 100;
    
        if(snake.state==SnakeGame::ALIVE)
            SDL_SetRenderDrawColor(renderer, 0 + i*0.7, 255 - snake.isHead*30 - i*1.5, 0 + snake.isHead*70, 255);
    
        else if (snake.state==SnakeGame::DEAD)
            SDL_SetRenderDrawColor(renderer, 255 - i*0.7, 0, 0 + snake.isHead*70, 255);

        SDL_Rect snake_r;
        snake_r.w = snake_r.h = (int)universal_s;
        snake_r.x = snake.getX() * step + universal_s/5;
        snake_r.y = snake.getY() * step + universal_s/5;
        SDL_RenderFillRect(renderer, &snake_r);

        if (snake.nxtSnake == NULL) return;

        drawSnakes(step, *(snake.nxtSnake), ++i);

}


//   ignore
// std::vector<float> in = snakes[0].getInputs();
// int k =0 ;
// std::cout << "what the first snake is seeing:" << std::endl <<  "distance from food x and y: ";
// for ( int i = 0; i < snake_view*snake_view + 2; i++) {
    
//     if ( i >1) {
//         if(i ==2) std::cout << std::endl;

//         if (k >=5) { std::cout << std::endl; k=0;}
//         k++;
//     }
//     std::cout << in[i] << " ";
//     // if ( i%snake_view == snake_view-1) std::cout
// }
// std::cout << std::endl << std::endl;

// std::cout << "snake 1 output from neural network: " << std::endl;
// auto output = snakeContainer->getNeuralNetwork(0)->calculateOutput(in);

// for (int i = 0; i < 4; i++) {
//     std::cout << output[i] << " ";
// }
// std::cout << std::endl;