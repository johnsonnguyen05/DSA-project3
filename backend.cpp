#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Node {
private:
    string id;
    string title;
    bool isAdult;
    string year;
    string runtime;
    vector<string> genres;
    float rating;
    int numOfRatings;

public:
    Node(string id, string title, bool isAdult, string year, string runtime, vector<string> genres, float rating, int numOfRatings) {
        this->id = id;
        this->title = title;
        this->isAdult = isAdult;
        this->year = year;
        this->runtime = runtime;
        this->genres = genres;
        this->rating = rating;
        this->numOfRatings = numOfRatings;
    }

    void display() {
        cout << "ID: " << id << "\nTitle: " << title << "\nAdult: " << isAdult << "\nYear: " << year << "\nRuntime: " << runtime << "\nGenres: ";
        for (auto genre : genres) {
            cout << genre << " ";
        }
        cout << "\nRating: " << rating << "\nNumber of Ratings: " << numOfRatings << endl;
    }
};
