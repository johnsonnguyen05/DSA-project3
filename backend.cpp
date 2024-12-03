#include <iostream>
#include <vector>
using namespace std;

class Node {
    private:
        string id;
        string title;
        //bool isAdult;
        string year; //tertiary 
        int runtime; //fourth
        vector<string> genres; //first, division
        float rating; 
        int numOfRatings; 
        float ratingValue; //secondary, calculate with ratings and numOfRatings. log(ratings * numOfRatings),,, divide by weight
};

class Graph {
    private:
        
};