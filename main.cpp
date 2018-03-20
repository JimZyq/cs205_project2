#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include "main.h"

using namespace std;

struct Feature_Set{
    double accuracy;
    vector<int>feat_in;

    const bool operator <(const Feature_Set& rhs) const{
        return accuracy < rhs.accuracy;
    }

    const vector<int> inc_print(vector<int> b) const{
        vector <int> v = b;
        for(int i=0;i<v.size();++i){
            v.at(i)+=1;
        }
        return v;
    }

    const void print() const{
        vector <int> v = inc_print(feat_in);
        ostringstream oss;
        if(!v.empty()){
            copy(v.begin(),v.end()-1,ostream_iterator<int>(oss,","));
            oss << v.back();
        }
        cout << "feature set {" << oss.str() <<"} with accuracy: " << accuracy << endl;
    }
};

Instance::Instance()
{
    type = 0;
}

void Instance::append_feature(double f)
{
    features.push_back(f);
    return;
}

void Instance::set_type(double i)                   //setter fimcton for the type of an instance
{
    type = i;
    return;
}

void Instance::print()                              //prints the instances (mostly for testing)
{
    cout << "type: " << type << endl;
    
    for(int i = 0; i < features.size(); ++i)
    {
        cout << features.at(i) << " ";
    }cout << endl;
    
    return;
}

double Instance::get_type()                         //returns the type of the instance
{
    return type;
}

Problem::Problem(vector<Instance> v)                                            //constructor
{
    dataset = v;
}

int Problem::dataset_size()                                                     //returns the size of the dataset
{
    return dataset.size();
}

double Euclidian(vector<double> test, vector<double> train, vector<int> index)  //calculates Euclidian distance
{
    double dist = 0.0;
    
    for(int i = 0; i < index.size(); ++i)
    {
        dist += pow(test.at(index.at(i)) - train.at(index.at(i)), 2);
    }
    return dist;    //removed sqrt
}

double Problem::Nearest_N(vector<int> v)                                        //takes in the feature sets and calculates the %
{
    double correct = 0; double tot_count = 0;
    
    for(int i = 0; i < dataset.size(); ++i)
    {
        double s_dist = INT_MAX;
        Instance nn;
        
        for(int j = 0; j < dataset.size(); ++j)
        {
            if(j == i)
            {
                continue;
            }
            
            double temp = Euclidian(dataset.at(i).get_features(), dataset.at(j).get_features(), v);
           
            if(temp < s_dist)                                                   //keeps track of the nearest neighbor
            {
                s_dist = temp;
                nn = dataset.at(j);
            }
        }
    
        if(nn.get_type() == dataset.at(i).get_type())                           //calculate the % correct/ total
        {
            correct += 1;
        }
        tot_count += 1;
    }
    
    return (correct/tot_count);
}

vector<double> Instance::get_features()             //returns the features of the instance
{
    return features;
}

vector<Instance> parse(string input_file){
    ifstream infile;
    infile.open(input_file.c_str());

    vector(Instance) v;
    while(infile.good()){
        Instance inst;
        double type = INT_MAX;
        vector<double> features;
        double temp;

        string row;
        getline(infile, row);

        istringstream parse(row);

        parse >> type;
        if(type == INT_MAX){
            break;
        }

        inst.set_type(type);

        while(parse >> temp){
            inst.append_feature(temp_;)
        }
        v.push_back(inst);
    }
    return v;
}

bool is_in(int index, vector<int> v){
    for(int i=0; i<v.size(); ++i){
        if(index == v.at(i)){
            return true;
        }
    }
    return false;
}

void Forward(Problem prob, int size)                        //function to solve forward selection
{
    cout << "This dataset has " << size << " features with "
         << prob.dataset_size() << " Instances:" << endl << endl;
         
    priority_queue<Feature_Set> pri_que;                    //priority queue for the features set
    
    Feature_Set max;                                        //holds the value of the max accuracy per feature
    max.accuracy = 0;
    Feature_Set temp;
    vector<int> y;
    
    temp.feat_in = y;                                       //prints starting state of forward
    temp.accuracy = prob.Nearest_N(y);
    temp.print();
    
    bool warn = true;
    
    for(int j = 0; j < size; ++j)
    {
        for(int i = 0; i < size; ++i)
        {
            Feature_Set set;
            vector<int> sel_features = y;                   //selected features
            
            if(is_in(i,sel_features) == true)               //only push on featues if they arent in
            {
                continue;
            }
            sel_features.push_back(i);                      //push back on feature at a time onto selected features
            set.feat_in = sel_features;
            set.accuracy = prob.Nearest_N(sel_features);    //the set's accuracy set by NN(takes in features)
            pri_que.push(set);                              //push feature set onto the queue
        }
        temp = pri_que.top();
        if(temp.accuracy > max.accuracy)                    //keep track of the feature set with highest %
        {
            max = temp;
        }
        if(warn && temp.accuracy < max.accuracy)
        {
            warn = false;
            cout << endl<< "Warning: accuracy decreasing, continuing search..." << endl << endl;
        }
        temp.print();                                       //print
        y = temp.feat_in;
        
        while(!pri_que.empty())                             //clear entire queue, only concerned with highest %
        {
            pri_que.pop();
        }
    }
    cout <<endl<< "the best feature subset is "; max.print();
}


void Backward(Problem prob, int size)                        //function to solve forward selection
{
    cout << "This dataset has " << size << " features with "
         << prob.dataset_size() << " Instances:" << endl << endl;
         
    priority_queue<Feature_Set> pri_que;                    //priority queue for the features set
    
    Feature_Set max;                                        //holds the value of the max accuracy per feature
    max.accuracy = 0;
    Feature_Set temp;
    vector<int> y;
    
    for(int i = 0; i < size; ++i)
    {
        y.push_back(i);
    }
    temp.feat_in = y;                                       //prints out the starting state
    temp.accuracy = prob.Nearest_N(y);
    temp.print();
    
    bool warn = true;
    
    for(int j = 0; j < size; ++j)
    {
        for(int i = 0; i < size; ++i)
        {
            Feature_Set set;
            vector<int> sel_features = y;                   //selected features
            
            if(is_in(i,sel_features) == false)               //only push on featues if they arent in
            {
                continue;
            }
            sel_features.erase(remove(sel_features.begin(), sel_features.end(), i), sel_features.end());
            //sel_features.push_back(i);                      //push back on feature at a time onto selected features
            set.feat_in = sel_features;
            set.accuracy = prob.Nearest_N(sel_features);    //the set's accuracy set by NN(takes in features)
            pri_que.push(set);                              //push feature set onto the queue
        }
        temp = pri_que.top();
        if(temp.accuracy > max.accuracy)                    //keep track of the feature set with highest %
        {
            max = temp;
        }
        if(warn && temp.accuracy < max.accuracy)
        {
            warn = false;
            cout << endl<< "Warning: accuracy decreasing, continuing search..." << endl << endl;
        }
        temp.print();                                       //print
        y = temp.feat_in;
        
        while(!pri_que.empty())                             //clear entire queue, only concerned with highest %
        {
            pri_que.pop();
        }
    }
    cout <<endl<< "the best feature subset is "; max.print();
}

int main(int argc, char*argv[])
{
    if(argc !=2)
    {
        cout << "Error: Invalid program call" << endl;
        return 1;
    }
    string file_name(argv[0]);
    string input_file(argv[1]);

    vector<Instance> v = parse(input_file);
    int size = v.at(0).get_features().size();

    int input;
    cout << "Welcome to Kevin's Feature Selection Algorithm" << endl;
    cout << "Type the number of the selection algorithm you want" << endl;
    cout << "\t 1) Forward Selection" << endl;
    cout << "\t 2) Backward Elimination" << endl;
    cout << "\t 3) Kevin's Special Algorithm" << endl;

    cin >> input;

    if(input == 1)
    {
        Forward(prob,size);
    }
    else if(input == 2)
    {
        Backward(prob,size);
    }
    else
    {
        cout << "Jimmy's algorithm" << endl;
    }
}