#include<iostream>
#include"../include/neural_network.hpp"
#include<ctime>
#include<chrono>

// #include <chrono>
// #include <thread>
#include<unistd.h>

// #define SINGLE
#define MULTIPLE

int main(int argc, char ** argv) {

    #ifdef SINGLE
    NeuralNetwork::SingleNetwork nn(10, 4);

    std::cout << "Matrices generated randomly: " << std::endl;

    std::cout << "w: " << std::endl << nn.getData(NeuralNetwork::WEIGHTS)
    << std::endl << "b " << std::endl << nn.getData(NeuralNetwork::BIAS) << std::endl;

    std::string w_name = "w0.txt";
    std::string b_name = "b0.txt"; 

    nn.fillMatrices(w_name, b_name);

    std::cout << "Matrices generated after file reading: " << std::endl;

    std::cout << "w: " << std::endl << nn.getData(NeuralNetwork::WEIGHTS)
    << std::endl << "b " << std::endl << nn.getData(NeuralNetwork::BIAS) << std::endl;    

    Eigen::MatrixXf input(10, 1);
    input << 1.23, 7, 12, -3.54, 0.05, 0, 1.05, -1.09, 8, 10;
    std::cout << "Inserting input: " << std::endl << input
    << std::endl << "Its output: " << std::endl << nn.calculateOutput(input) << std::endl;

    #else
    std::cout << "Initializing container of neural network " << std::endl;
    // NeuralNetwork::NetworkContainer container(4, 3, 3);

    NeuralNetwork::NetworkContainer container(1000, 27, 4);

    // for ( int i = 0 ; i < 100; i++)
    // {
    //     NeuralNetwork::SingleNetwork* network = new NeuralNetwork::SingleNetwork(27, 3, true);

    //     container.setNeuralNetwork(i, network);
    // }
    // container.printAllNetworks();
    // std::srand(std::time(nullptr));
    // std::vector<float> inputs;
    // inputs.reserve(27);
    // for (int i = 0 ; i < 27; i++) {
    //     inputs.push_back(0);
    // } 

    // while(true) {
    //     std::cout << "s: " ;
    //     for (int i = 0 ; i < 27; i++) {
    //         inputs[i] = std::rand() % 100;
    //     } 
    //     int i = 0;
    //     for (auto &network : container.getNeuralNetworks()) {
    //         auto outputs = network->calculateOutput(inputs);
    //         std::cout << i++ << ": ";

    //         for(auto e : outputs) {
    //             std::cout << e << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     // std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    //     usleep(1000000 / 3);
        
    // }

    // auto time = std::chrono::system_clock::now();
    // std::time_t savingTime_t = std::chrono::system_clock::to_time_t(time);
    // std::string final_time = std::ctime(&savingTime_t);

    // for(auto &c : final_time) {
    //     if (c == ' ') c='_';
    // }

    // std::cout << final_time << std::endl;
    // std::string w_name_final = "data/temp/" + ;

    std::string brain_path = "data/temp/brain_0_Fri_Sep_29_15-31-13_2023";
    NeuralNetwork::SingleNetwork singleNetwork(27, 4);
    singleNetwork.fillMatrices(brain_path);
    std::cout << "file read: " << singleNetwork.getData(NeuralNetwork::WEIGHTS)
    << std::endl << "bias: " << singleNetwork.getData(NeuralNetwork::BIAS) << std::endl;
    #endif


}