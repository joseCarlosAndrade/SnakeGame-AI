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
        unsigned int best_snake_number, int g_height, int g_width,
         int gw_squares, int gh_squares) 
    :  height(g_height), width(g_width), w_squares(gw_squares), h_squares(gh_squares), 
    snake_view(view), behavior(bh), bestSnakesNumber(best_snake_number)
    {
        iterationCount = 0;
        maxIterations = 1; // set a max value by default
        pathToSave = "data/";
        lastBrain = "$no$";

        nextSnakeContainer = nullptr;

        lastBestFitnes = -1;

        // intializing game attributes
        running = 1;
        stopSimulation = false;
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

        // defining default mutation rates
        maxMutationChange = 0.1;
        mutationChance = 0.1;

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

void Game::setMaxIterations(int iterations) {
    if (iterations <1) {
        std::cerr << "Invalid number of iterations. Please use a positive integer." <<std::endl;
        exit(EXIT_FAILURE);
    }
    
    maxIterations = iterations;
    std::cout << "Setting maximum iteration to " << maxIterations  << std::endl;
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
    snakeContainer = new NeuralNetwork::NetworkContainer(n_snakes, SNAKE_INPUTS, SNAKE_OUTPUTS);
    assert(snakeContainer != NULL);

    if(lastBrain!="$no$" && nextSnakeContainer != nullptr) {
        std::cout << "Using new brains!!" << std::endl;
        for (int i = 0; i < n_snakes; i++) {
            snakeContainer->getNeuralNetwork(i)->copyNetwork(*(nextSnakeContainer->getNeuralNetwork(i)));
        }
    }

    Uint32 nowTime = SDL_GetTicks();

    for ( int i = 0; i < n_snakes; i++) {
        food_x = std::rand() % w_squares;
        food_y = std::rand() % h_squares;
        SnakeGame::Snake c_snake(w_squares/2, h_squares/2, w_squares, h_squares, true, true, food_x, food_y, snake_view);
        c_snake.setThisBrain(snakeContainer->getNeuralNetwork(i));
        assert(c_snake.getThisBrain() != NULL);

        c_snake.setSnakeTimer(nowTime);
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
        if (keystates[SDL_SCANCODE_ESCAPE])  {
            stopSimulation = true;
            } // quits
       
        if (keystates[SDL_SCANCODE_LEFT] && FPS > 3) FPS--;
        else if (keystates[SDL_SCANCODE_RIGHT] && FPS < 50) FPS++;
        else if(keystates[SDL_SCANCODE_S]) running = false;
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
        Uint32 nowTimer = SDL_GetTicks();

        for ( std::vector<SnakeGame::Snake>::iterator it = snakes.begin(); it != snakes.end() ; it++) {
            
            // usleep(1000000 / 200);
            
            SnakeGame::Snake *snake = it.base();

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
                }

            if (snake->state!=SnakeGame::DEAD) {
                snake->moveAllSnakes();

                // if the snake ate a food, reset its timer
                if (snake->checkCollision() == SnakeGame::GROWING)
                    snake->setSnakeTimer(SDL_GetTicks());

                // timeout for each snake
                Uint32 timePassed = nowTimer - snake->getSnakeTimer();
                if( (timePassed)>= (SNAKE_TIMEOUT+snake->getSize())*1000 && timePassed <SNAKE_TIMEOUT*10000) {
                    snake->state = SnakeGame::DEAD;
                    // std::cout << "Snake died due to timeout : " << (nowTimer - snake->getSnakeTimer())/1000 << std::endl;
                }
                    

                if(snake->state==SnakeGame::DEAD) {
                    this->deadSnakes++;
                    snake->changeAllStates();
                }
            }
        }    
}

// main loop for one round
bool Game::iterateOnce(bool saveToFile) {

    int drawTimer = 0;
    int snakeTimer = 0, lastSnakeTimer = 0;
    int first = 1;

    int gameUniversalTimer = SDL_GetTicks();
    running = true;
    // std::srand(std::time(nullptr));
    std::cout << std::endl <<"==============================="
    <<" Iteration: " << iterationCount<<" ==============================="
     <<std::endl << std::endl;

    while(running) {
        // if (SDL_GetTicks() >= gameUniversalTimer+(100*1000)) {
        //     std::cout << "Iteration timeout. " << std::endl;
        //     running =false;
        //     break;
        // }
        if(stopSimulation) {
            running =false;
            return false;
        }
        // int a = std::rand();
        // std::cout << std::endl<< a << " should match ";

        SDL_Delay(10);
        // std::cout << a << std::endl;
        if (this->deadSnakes == n_snakes) { 
            // std::cout << this->deadSnakes << " snakes dead. Game over" << std::endl;
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
        this->draw();
    }

    // evaluating fitness

    int max_index=-1;
    int this_fitness=0;
    int max_fitness = 0;

    for (int i = 0; i < bestSnakesNumber; i++) { 
        int cur_index = 0;
        for (auto sn : snakes) {
            this_fitness = sn.getThisBrain()->fitnessOperation(NeuralNetwork::GET); // all of this to access the snake brain fitness..
            // std::cout << "f: " <<  this_fitness << "; ";
            if (this_fitness > max_fitness) {
                std::cout << "Best found-> " <<  this_fitness << "; " << std::endl;

                max_fitness = this_fitness;
                max_index = cur_index;         
            }

            cur_index++;
        }
    }

    // showing best fitness
    for ( int i = 0; i < bestSnakesNumber; i++) {
        std::cout <<std::endl <<  "Snake index: " << max_index << std::endl;
        std::cout << "Fitness value: " << max_fitness <<std::endl;
    }

    NeuralNetwork::SingleNetwork itBrain_(SNAKE_INPUTS, SNAKE_OUTPUTS);
    if (saveToFile && max_index != -1) {
        auto time = std::chrono::system_clock::now();
        std::time_t savingTime_t = std::chrono::system_clock::to_time_t(time);
        std::string final_time = std::ctime(&savingTime_t);
        
        
        for(auto &c : final_time) {
            if (c == ' ') c = '_'; // replacing spaces with _ to avoid name errors
            if (c == ':') c = '-';
        }


        // save networks from best N_BEST_SNAKES snakes on file 
        for ( int i = 0; i < bestSnakesNumber; i ++) {
            itBrain_.copyNetwork(*(snakes.at(max_index).getThisBrain() ));
            // saveToFile(* itBrain );
            std::string brain_name_str = "";
            if(iterationCount < maxIterations-1)
                brain_name_str.append("data/temp/brain_") ;
            else {
                brain_name_str.append("data/final/brain_");

                std::cout<< "Saving final brain on data/final/" << std::endl;
            }
                

            // brain_name_str.append("data/temp/brain_");
            brain_name_str.append(std::to_string(i));
            brain_name_str.append("_" + final_time);
            int newl = brain_name_str.find_first_of('\n');
            brain_name_str.at(newl) = '.';
            brain_name_str.append("b");

            lastBrain = brain_name_str;

            std::cout << std::endl << "Brain saved on file: " << brain_name_str << std::endl;
            itBrain_.saveNetworkToFile(brain_name_str);
        }       
    }

    if ( max_index != -1 && saveToFile) {
        nextSnakeContainer == nullptr;
        if (iterationCount == 0) {
            
            nextSnakeContainer = new NeuralNetwork::NetworkContainer(n_snakes, SNAKE_INPUTS,SNAKE_OUTPUTS);
            if(nextSnakeContainer==NULL) {
                std::cerr << "Bad allocation on next container. Exiting..." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        std::cout << "Doing crossover.. " << std::endl;

        if (max_fitness > lastBestFitness) {
            snakeContainer->doCrossover(itBrain_, nextSnakeContainer, NeuralNetwork::AVERAGE);
            lastBestFitness = max_fitness;
            lastBestNetwork.copyNetwork(itBrain_);
        }

        else {
            std::cout << "New fitness above last score.. Using last best brain. " << std::endl;
            snakeContainer->doCrossover(lastBestNetwork, nextSnakeContainer, NeuralNetwork::AVERAGE);
        }

        std::cout << "Mutating.." << std::endl; 
        nextSnakeContainer->mutateContainer(mutationChance, maxMutationChange);

        delete snakeContainer;
    }

    // deallocate snakes
    std::vector<SnakeGame::Snake>().swap(this->snakes);
    
    std::cout << std::endl << "Incrementing iteration count " << std::endl;
    iterationCount++;
    return true;
}

void Game::playGame(uint n_snakes, float mutation_f, float max_change ) {
    mutationChance = mutation_f;
    maxMutationChange = max_change;

    std::cout << std::endl << "---> INITIALIZING GAME! :) <---" << std::endl << std::endl <<

    "Total of snakes: " << n_snakes << std::endl <<
    "Iterations: " << maxIterations << std::endl <<
    "Mutation chance (percentage of nodes that will mutate): " << mutationChance << std::endl <<
    "Maximum mutation change (absolute): " << maxMutationChange << std::endl << std::endl <<

    "Usefull tools:" << std::endl <<
    "Press S to skip the current iteration (and save temp brain)." << std::endl <<
    "Press Esc to end game (won't save anything more)." << std::endl << std::endl <<

    "Press Enter key to start!" << std::endl;

    char a;
    a =getchar();

    for (int i =0; i < maxIterations; i ++) {
        initSnakes(n_snakes);
        if(!iterateOnce(true)) break;
    }
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
    snakeContainer = new NeuralNetwork::NetworkContainer(1, SNAKE_INPUTS, SNAKE_OUTPUTS);
    snakeContainer->getNeuralNetwork(0)->fillMatrices(brain_file_path);
    

    if(brain_file_path != "!")
    // singleNetwork->fillMatrices(brain_file_path);
    
    food_x = std::rand() % w_squares;
    food_y = std::rand() % h_squares;

    SnakeGame::Snake singleSnake(w_squares/2, h_squares/2, w_squares, h_squares, true, true, food_x, food_y, snake_view);
    singleSnake.setThisBrain( snakeContainer->getNeuralNetwork(0) );
    assert(singleSnake.getThisBrain() != NULL);

    snakes.push_back(singleSnake);
    
}