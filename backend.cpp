#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <cmath>
#include <algorithm>
#include <limits>
#include <functional>
#include <queue>
using namespace std;

class Node {
public:
    string id;
    string title;
    string altTitle;
    string year;
    string runtime;
    vector<string> genres;
    float rating;
    int numOfRatings;
    Node() {}
    Node(string id, string title, string isAdult, string year, string runtime, vector<string> genres, float rating, int numOfRatings) {
        this->id = id;
        this->title = title;
        this->altTitle = altTitle;
        this->year = year;
        this->runtime = runtime;
        this->genres = genres;
        this->rating = rating;
        this->numOfRatings = numOfRatings;
    }
    void display() {
        cout << "ID: " << id << "\nTitle: " << title << "\nAltTitle: " << altTitle << "\nYear: " << year << "\nRuntime: " << runtime << "\nGenres: ";
        for (auto genre : genres) {
            cout << genre << " ";
        }
        cout << "\nRating: " << rating << "\nNumber of Ratings: " << numOfRatings << endl;
    }
};

class Graph {
public:
    unordered_map<string, vector<pair<string, float>>> adjList; // Map Node ID to a list of pairs (Neighbor ID, Weight)
public:
    void addEdge(const string &node1, const string &node2, float weight) {
        adjList[node1].push_back({node2, weight});
        adjList[node2].push_back({node1, weight});
    }
    void displayGraph() {
        for (auto &node : adjList) {
            cout << "Node " << node.first << ":\n";
            for (auto &neighbor : node.second) {
                cout << "  -> " << neighbor.first << " (Weight: " << neighbor.second << ")\n";
            }
        }
    }
};