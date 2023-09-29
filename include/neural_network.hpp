#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include<eigen3/Eigen/Dense>
#include<iostream>
#include<vector>

#include<chrono>
#include<ctime>

#define EULER 2.71828

typedef unsigned int fitness_value;

namespace NeuralNetwork {

/* Type of Data to be retrieved */
typedef enum {WEIGHTS, BIAS} NNData;

/* Crossover type. 
`AVERAGE`: takes the average of the two values.
`AVERAGE_NEAR_BEST`: Takes a value that is in between the two, but closer to the best.
`UNIFORM_CROSSOVER`: Takes the entire value from the networks but randomly picked one another. */
typedef enum {AVERAGE, AVERAGE_NEAR_BEST, UNIFORM_CROSSOVER} CrossoverType;
typedef enum {GET, SET, INCREASE, DECREASE} FitnessOperationType;

typedef unsigned int CONTAINER_SIZE; // Defines the size of neural network container

/* ### Single Network Data
    Represents a single brain (neural network) with only 2 layers so far: input and output. 
    It takes care of the most essencial matrix operations intended for ai use.
 */
class SingleNetwork {
    private:
        const int _n_input, _n_output;
        Eigen::MatrixXf _w0, _b0;
        fitness_value thisFitness; // This neural network fitness value
    
    public:
        
        SingleNetwork();

        /* Initializes a single network with the given input and output numbers. If the matrices won't 
        be loaded ou defined externaly, use random=true to initialize it with random values between -2,2. */
        SingleNetwork(const int n_inputs, const int n_outputs, bool random=true) :   
            _n_input(n_inputs),
            _n_output(n_outputs)
        {
            thisFitness = 0;

            if (random) {
                _w0 = Eigen::MatrixXf::Random(n_outputs, n_inputs)*2; // generates random values between -2 and 2
                _b0 = Eigen::MatrixXf::Random(n_outputs, 1)*2;
            }
            else {
                _w0.setOnes();
                _b0.setOnes(); 
            }
        }

        /* Returns an Eigen::Matrix as output given for the given input matrix of the same type. Note: It has to match the 
        number of inputs and outputs defined on the network. */
        Eigen::MatrixXf calculateOutput(Eigen::MatrixXf input);

        /* Returns a std::vector<float> as output given for the given input matrix of the same type. Note: It has to match the 
        number of inputs and outputs defined on the network. */
        std::vector<float> calculateOutput(std::vector<float> input);

        /* Fills the weight and bias matrices from the given files */
        void fillMatrices(std::string w_file_name, std::string b_file_name);
    
        /* Fills the weight and bias matrices from the given brain file */
        void fillMatrices(std::string brain_file_name);

        // void flushCurrentData();
        // void saveBestNetwork();

        /* Gets the requested data from the network */
        Eigen::MatrixXf getData(NNData data_to_get);

        /* Save the current brain on the specified path and file name */
        bool saveNetworkToFile(std::string path_to_file);

        /* Intermediate to allow the specified fitness operation type. */
        unsigned int fitnessOperation(FitnessOperationType operation, unsigned int operand=0);

};



/* Defines a container of network object. It holds CONTAINER_SIZE neural networks with it. It makes easier to
handle several brains. */
class NetworkContainer {
    private:
        std::vector<SingleNetwork*> networks;
        CONTAINER_SIZE  n_networks;

        const int _n_input, _n_output;
    
    public:
        /* Creates a container that stores a single network of input size `n_inputs` and output `n_outputs`. */
        NetworkContainer(const int n_inputs, const int n_outputs) :
            _n_input(n_inputs), _n_output(n_outputs) {
            n_networks = 1;

        }
        /* Creates a container of `size_t` size. Each neural network will have `n_inputs` as number of inputs and `n_outputs` as output. */
        NetworkContainer(CONTAINER_SIZE size_t, const int n_inputs, const int n_outputs) : 
            n_networks(size_t), _n_input(n_inputs), _n_output(n_outputs) 
            {
            // assert(size_t!=0);

            networks.reserve(size_t);
            for (uint i = 0; i < size_t; i++) {
                networks.push_back(new SingleNetwork(n_inputs, n_outputs)); // initializes each network randomly

            }  
        }

        ~NetworkContainer() {
            for (auto n : networks) {
                delete n;
            }
            
            // deallocate networks to free memory
            std::vector<SingleNetwork*>().swap(networks);
        }

        /* Setter */
        void setNeuralNetwork(uint index, SingleNetwork* network) {
            assert(index < n_networks);
            networks.at(index) = network;
        }

        /* Returns a pointer to the network from the `index` position on the container. */
        SingleNetwork * getNeuralNetwork(uint index) {
            assert(index <n_networks);

            return networks[index];
        }

        std::vector<NeuralNetwork::SingleNetwork*> getNeuralNetworks() {
            return networks;
        }
        
        /* Prints all network. For debugging pourpose, I do not recommend to use in real implementations, but it may be
        usefull to debug. */
        void printAllNetworks () {
            std::vector<SingleNetwork*>::iterator iterator;
            iterator = networks.begin();
            
            while(iterator != networks.end()) {
                std::cout << "Printing information on network: " << (iterator) - networks.begin() << std::endl
                << "Weights: " << std::endl << (*iterator)->getData(WEIGHTS) << std::endl <<std::endl
                << "Bias: " << std::endl << (*iterator)->getData(BIAS) << std::endl <<std::endl;

                iterator++;
            }
        }

        /* Does crossover from the specified type on the entire container with the `bestNetworks` container passed as argument. */
        // void doCrossover(NetworkContainer bestNetworks, CrossoverType type = AVERAGE) {
            
        // }   

        // /* Mutates the entire container with the mutation factor `mutation_p`. */
        // void mutateContainer(float mutation_p) {

        // } 
        
};

}

#endif