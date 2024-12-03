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
    // Dijkstra's algorithm to find the shortest path from source node
    vector<string> dijkstra(const string &startNode, int numNodes, const vector<Node> &nodes) {
        // Distance map to store the shortest distance to each node
        unordered_map<string, float> dist;
        // Priority queue (min-heap) to get the node with the smallest distance
        priority_queue<pair<float, string>, vector<pair<float, string>>, greater<pair<float, string>>> pq;
        
        // Initialize distances to infinity, except the start node
        for (auto &node : adjList) {
            dist[node.first] = numeric_limits<float>::infinity();
        }
        dist[startNode] = 0;
        
        // Push the start node with a distance of 0 into the priority queue
        pq.push({0, startNode});
        
        // Result vector to store the first 'numNodes' nodes
        vector<string> result;
        
        while (!pq.empty() && result.size() < numNodes) {
            string currentNode = pq.top().second;
            pq.pop();
            
            // If this node is already processed (i.e., added to result), skip it
            if (dist[currentNode] != pq.top().first) {
                continue;
            }

            // Find the movie title from the node ID
            string movieTitle;
            for (const Node &node : nodes) {
                if (node.id == currentNode) {
                    movieTitle = node.title;
                    break;
                }
            }
            result.push_back(movieTitle);
            
            // Explore neighbors of the current node
            for (auto &neighbor : adjList[currentNode]) {
                string neighborNode = neighbor.first;
                float weight = neighbor.second;
                
                // Calculate the distance to the neighbor
                float newDist = dist[currentNode] + weight;
                
                // If a shorter path is found, update the distance and add it to the priority queue
                if (newDist < dist[neighborNode]) {
                    dist[neighborNode] = newDist;
                    pq.push({newDist, neighborNode});
                }
            }
        }
        
        return result;
    }
};