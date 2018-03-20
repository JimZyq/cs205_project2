#ifndef INSTANCE_H
#define INSTANCE_H

#include <iostream>
#include <vector>

using namespace std;

class Instance
{
    private:
        double type;                    //type of instance (1 or 2)
        vector<double> features;        //holds all of the features per row
        
    public:
        Instance();                     //constructor
        void append_feature(double);    //appends features to vector
        void set_type(double);          //set function for class type
        void print();
        double get_type();
        vector<double> get_features();
        
};


#endif

class Problem
{
    private:
        vector<Instance> dataset;           //vector that holds the dataset
        
    public: 
        Problem(vector<Instance>);          //constructor
        int dataset_size();
        double Nearest_N(vector<int>);      //returns % based on features used
};