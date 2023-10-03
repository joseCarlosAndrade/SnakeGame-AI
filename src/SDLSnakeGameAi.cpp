#include<SDL2/SDL.h>
#include<iostream>
#include"../include/neural_network.hpp"
#include"../include/snake.hpp"
#include"../include/SDLSnakeGameAi.hpp"
#include<map>

//timing handling
#include<unistd.h>
#include<ctime>
#include<chrono>
#include<fstream>


Game::Game(SnakeGame::SNAKE_VIEW_AREA view, snake_behavior bh, 
        unsigned int best_snake_number, int g_height, int g_width, int gw_squares, int gh_squares) 
    :  height(g_height), width(g_width), w_squares(gw_squares), h_squares(gh_squares), 
    snake_view(view), behavior(bh), bestSnakesNumber(best_snake_number)
    {
        iterationCount = 0;
        pathToSave = "data/";
        lastBrain = "$no$";

        nextSnakeContainer = nullptr;

        // intializing game attributes
        running = 1;
        fullscreen = 0;
        lastTime = 0;
        FPS = 10;

        deadSnakes = 0;

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
    delete nextSnakeContainer;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// intantiates all snakes
void Game::initSnakes(NeuralNetwork::CONTAINER_SIZE n_s) {
    
    // snakes boiiii (and their foods)
    n_snakes = n_s;
    deadSnakes = 0;

    std::srand(std::time(nullptr));
    int food_x, food_y;

    this->deadSnakes = 0;
    snakes.reserve(n_snakes);

    // initializing container of neural networks
    snakeContainer = new NeuralNetwork::NetworkContainer(n_snakes, 27, 4);
    assert(snakeContainer != NULL);

    if(lastBrain!="$no$" && nextSnakeContainer != nullptr) {
        for (int i = 0; i < n_snakes; i++) {
            snakeContainer->getNeuralNetwork(i)->copyNetwork(*(nextSnakeContainer->getNeuralNetwork(i)));
        }
    }

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
        //std::cout << " Entering update snakes.. ";
        int index = 0;
        SnakeGame::Direction thisDirecion;

        int i =0;
        // for(auto& snake : snakes) {

        for ( std::vector<SnakeGame::Snake>::iterator it = snakes.begin(); it != snakes.end() ; it++) {
            
            // usleep(1000000 / 200);
            
            SnakeGame::Snake *snake = it.base();
        // }
            //std::cout << snake->getThisBrain()->getData(NeuralNetwork::BIAS) << std::endl;
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

        //std::cout << " !!! updated all snakes!!!!  ";
   
}

// main loop for one round
void Game::iterateOnce(bool saveToFile) {
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
            //sstd::cout << "updating all of them:" <<std::endl;


            this->updateSnakes();
 

            lastSnakeTimer = snakeTimer;
        }
        //std::cout << "Drawing snakes: " ;
        this->draw();
        //std::cout << n_snakes - this->deadSnakes << " snakes drawn." << std::endl;
    }
    std::cout << "loop terminated " << std::endl;


    // evaluate fitness from all snake neural networks (choose best N_BEST_SNAKES)
    // SnakeProperties *best_snake_structs[100];
    SnakeProperties * best_snake_structs = new SnakeProperties[100];
    for (int i = 0; i < bestSnakesNumber; i++) {
        best_snake_structs[i].fitness = 0;
        best_snake_structs[i].index = -1;
        
    }


    std::cout << "struct created" << std::endl;

    int cur_index=0;
    int this_fitness=0;
    for (int i = 0; i < bestSnakesNumber; i++) { 
        cur_index = 0;
        for (auto sn : snakes) {
            this_fitness = sn.getThisBrain()->fitnessOperation(NeuralNetwork::GET); // all of this to access the snake brain fitness..
            std::cout << "f: " <<  this_fitness << "; ";
            if (this_fitness > best_snake_structs[i].fitness) {
                std::cout << std::endl << "best found: " <<  this_fitness << "; " << std::endl;
                // *best_snake_structs[i] = {&sn, this_fitness, cur_index};
                best_snake_structs[i].snake = &sn;
                best_snake_structs[i].fitness = this_fitness;
                best_snake_structs[i].index = cur_index;
            }
            cur_index++;
        }
    }
    std::cout << "fitness evaluated. Best snake and fitnes: " << std::endl;
    for ( int i = 0; i < bestSnakesNumber; i++) {
        std::cout << "Snake index: " << best_snake_structs[i].index << std::endl;
        std::cout << "Fitness value: " << best_snake_structs[i].fitness <<std::endl;
    }

    NeuralNetwork::SingleNetwork *itBrain_ = new NeuralNetwork::SingleNetwork(27, 4); // brain to be used in crossover TODO: implement crossover with multiple brains

    if (saveToFile) {

        auto time = std::chrono::system_clock::now();
        std::time_t savingTime_t = std::chrono::system_clock::to_time_t(time);
        std::string final_time = std::ctime(&savingTime_t);
        
        for(auto &c : final_time) {
            if (c == ' ') c = '_'; // replacing spaces with _ to avoid name errors
            if (c == ':') c = '-';
        }

        
        // save networks from best N_BEST_SNAKES snakes on file 
        for ( int i = 0; i < bestSnakesNumber; i ++) {
            // std::cout << "Saving snake " << i << " to file" << std::endl;
            NeuralNetwork::SingleNetwork *itBrain = best_snake_structs[i].snake->getThisBrain();
            // saveToFile(* itBrain );

            std::string brain_name_str = "data/temp/brain_";
            // brain_name_str.append("data/temp/brain_");
            brain_name_str.append(std::to_string(i));
            brain_name_str.append("_" + final_time);
            int newl = brain_name_str.find_first_of('\n');
            brain_name_str.at(newl) = '.';
            brain_name_str.append("b");

            lastBrain = brain_name_str;

            std::cout << "file: " << brain_name_str << std::endl;
            itBrain->saveNetworkToFile(brain_name_str);
            itBrain_->copyNetwork(*itBrain);
        }
        
        // do crossover with CROSSOVER_PROBABILITY and LEARNING_RATE to repopulate neural network container
       
        std::cout << "snake file saved" << std::endl;
    }
    if (iterationCount == 0) {
        nextSnakeContainer = new NeuralNetwork::NetworkContainer(n_snakes, 27, 4);
    }
    snakeContainer->doCrossover(*itBrain_, nextSnakeContainer, NeuralNetwork::AVERAGE);
    nextSnakeContainer->mutateContainer(0.1, 0.1);


    // deallocate snakes
    std::vector<SnakeGame::Snake>().swap(this->snakes);
    delete snakeContainer;
    delete best_snake_structs;
    
    std::cout << "incrementing iteration count " << std::endl;
    iterationCount++;
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


void Game::spawnSingleSnakeAI(std::string brain_file_path) {
    deadSnakes = 0;

    std::srand(std::time(nullptr));
    int food_x, food_y;

    // NeuralNetwork::SingleNetwork* singleNetwork = new NeuralNetwork::SingleNetwork(27, 4);
    snakeContainer = new NeuralNetwork::NetworkContainer(1, 27, 4);
    snakeContainer->getNeuralNetwork(0)->fillMatrices(brain_file_path);
    

    if(brain_file_path != "!")
    // singleNetwork->fillMatrices(brain_file_path);
    
    food_x = std::rand() % w_squares;
    food_y = std::rand() % h_squares;

    SnakeGame::Snake singleSnake(w_squares/2, h_squares/2, w_squares, h_squares, true, true, food_x, food_y, snake_view);
    singleSnake.setThisBrain( snakeContainer->getNeuralNetwork(0) );
    assert(singleSnake.getThisBrain() != NULL);

    snakes.push_back(singleSnake);
    
    // iterateOnce();
}