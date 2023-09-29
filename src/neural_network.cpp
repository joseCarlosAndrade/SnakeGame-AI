#include"../include/neural_network.hpp"
#include<fstream>
#include<iostream>

// Calculate output from input eigen::matrix 
Eigen::MatrixXf NeuralNetwork::SingleNetwork::calculateOutput(Eigen::MatrixXf input) {
    Eigen::MatrixXf output(this->_n_output, 1);
    assert(input.cols() == 1 && input.rows() == _n_input);

    output = _w0 * input;
    output += _b0;

    double sum = 0;
    for ( int i = 0; i < output.rows(); i ++) {
        
        sum += pow(EULER, (double)output(i, 0));
    } 
    for ( int i = 0; i < output.rows(); i ++) {
        output(i, 0) =  pow(EULER, output(i, 0)) / sum;
    }
    
    return output;
}

std::vector<float> NeuralNetwork::SingleNetwork::calculateOutput(std::vector<float> input) {
    assert(input.size() ==  (unsigned long int) _n_input);

    Eigen::MatrixXf input_e(input.size(), 1);
    
    for(int i = 0; i < _n_input ;i++) {
        input_e(i, 0) = input[i];
    }

    Eigen::MatrixXf output = calculateOutput(input_e);

    std::vector<float> output_e;
    // output_e.reserve(output.size());

    for(int i =0; i < output.size(); i++) {
        output_e.push_back(output(i, 0));
    } 

    return output_e;
}

// fill neural network from file
void NeuralNetwork::SingleNetwork::fillMatrices(std::string w_file_name, std::string b_file_name) {
    // assert(w_file_name!= NULL && b_file_name && NULL);

    std::string w_name = "data/initial/" + w_file_name;
    std::string b_name = "data/initial/" + b_file_name;

    std::fstream file_w, file_b;
    file_w.open(w_name, std::ios::in);
    file_b.open(b_name, std::ios::in);
    
    if (!file_w || !file_b ) {
        std::cerr << "Failed to open matrices files." << std::endl;
        exit(1);
    }

    for (int i = 0; i < this->_n_output; i++) {
        for (int j = 0; j < this->_n_input; j++) {
            file_w >> this->_w0(i, j);
        }
    }

    for ( int i = 0; i < this->_n_output; i++) {
        file_b >> this->_b0(i, 0);
    }

    // TODO: randomize elements from inital matrices
    
}

// fill neural network from file
void NeuralNetwork::SingleNetwork::fillMatrices(std::string brain_file_name) {
    // assert(w_file_name!= NULL && b_file_name && NULL)

    std::fstream brian_file;
    brian_file.open(brain_file_name, std::ios::in);
    // file_b.open(b_name, std::ios::in);
    
    if (!brian_file ) {
        std::cerr << "Failed to open matrix file: " << std::endl
        << brain_file_name << std::endl;
        exit(1);
    }

    for (int i = 0; i < this->_n_output; i++) {
        for (int j = 0; j < this->_n_input; j++) {
            brian_file >> this->_w0(i, j);
        }
    }

    for ( int i = 0; i < this->_n_output; i++) {
        brian_file >> this->_b0(i, 0);
    }

    // TODO: randomize elements from inital matrices
    
}

bool NeuralNetwork::SingleNetwork::saveNetworkToFile(std::string path_to_file) {
    std::fstream brain_file;


    // getting current time
    brain_file.open(path_to_file, std::fstream::out);
    
    if(!brain_file) {
        std::cerr << "ERROR: Could not open/create file. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < this->_n_output; i++) {
        for (int j = 0; j < this->_n_input; j++) {
            brain_file << this->_w0(i, j);
            brain_file << " ";
        }
        brain_file << std::endl;
    }
    

    for ( int i = 0; i < this->_n_output; i++) {
        brain_file << this->_b0(i, 0);
        brain_file << std::endl;

    }
    return true;

}

Eigen::MatrixXf NeuralNetwork::SingleNetwork::getData(NNData data_to_get) {
    switch (data_to_get)
    {
    case WEIGHTS:
        return this->_w0;
        break;
    
    case BIAS:
        return this->_b0;
        break;
    default:
        return this->_w0;
    }
}

unsigned int NeuralNetwork::SingleNetwork::fitnessOperation(FitnessOperationType operation, unsigned int operand=0) {
            switch (operation) {
                case GET:
                    return thisFitness;
                
                case SET:
                    // if (operand < 0) return 0; // prevents from setting fitness to below zero numbers
                    thisFitness = operand;
                    return 1;

                case INCREASE:
                    thisFitness += operand;
                    return 1;
                
                case DECREASE:
                    // if (thisFitness - operand < 0) thisFitness = 0;
                    // else 
                        thisFitness -= operand;
                    return 1;
                default:
                    return 0;
            }
        }

// fill neural network from array (overload)
// void NeuralNetwork::fill_matrices(float*w_array, float*b_array) {
//     Eigen::Map<Eigen::Matrix<float, this->_n_output, _n_input>> w_temp
// }

// // flushes the current data to the temp file at data/temp/
// void NeuralNetwork::SingleNetwork::flushCurrentData() {

// }

// // saves the best network so far at data/final
// void NeuralNetwork::SingleNetwork::saveBestNetwork() {

// }

