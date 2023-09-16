#include<iostream>
#include"../include/neural_network.hpp"

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
    NeuralNetwork::NetworkContainer container(4, 3, 3);
    container.printAllNetworks();
    #endif


}