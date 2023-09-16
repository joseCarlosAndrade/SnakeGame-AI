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
    assert(input.size() == _n_input);

    Eigen::MatrixXf input_e(input.size(), 1);
    
    for(int i = 0; i < _n_input ;i++) {
        input_e(i, 0) = input[i];
    }

    Eigen::MatrixXf output = calculateOutput(input_e);

    std::vector<float> output_e;
    output_e.reserve(output.size());

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

