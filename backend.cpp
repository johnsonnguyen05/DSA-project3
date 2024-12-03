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
    Node(const string &id, const string &title, bool isAdult, const string &year, string runtime,
         const vector<string> &genres, float rating, int numOfRatings)
        : id(id), title(title), isAdult(isAdult), year(year), runtime(runtime),
          genres(genres), rating(rating), numOfRatings(numOfRatings) {}

    void display() const {
        cout << "ID: " << id << "\nTitle: " << title << "\nAdult: " << isAdult
             << "\nYear: " << year << "\nRuntime: " << runtime
             << "\nGenres: ";
        for (const auto &genre : genres) {
            cout << genre << " ";
        }
        cout << "\nRating: " << rating << "\nNumber of Ratings: " << numOfRatings << endl;
    }
};

vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

Node parseTSVRow(const string &row) {
    stringstream ss(row);
    string id, type, title, year, genresStr, isAdultStr, runtimeStr, ratingStr, numOfRatingsStr;

    // Read each tab-separated value
    getline(ss, id, '\t');          // ID
    getline(ss, type, '\t');        // Type (ignored)
    getline(ss, title, '\t');       // Title
    getline(ss, isAdultStr, '\t');  // Is Adult
    getline(ss, year, '\t');        // Year
    getline(ss, runtimeStr, '\t');  // Runtime
    getline(ss, genresStr, '\t');   // Genres
    getline(ss, ratingStr, '\t');   // Rating
    getline(ss, numOfRatingsStr);   // Number of Ratings

    // Convert values to appropriate types
    bool isAdult = stoi(isAdultStr);
    string runtime = runtimeStr;
    float rating = stof(ratingStr);
    int numOfRatings = stoi(numOfRatingsStr);
    vector<string> genres = split(genresStr, ',');

    return Node(id, title, isAdult, year, runtime, genres, rating, numOfRatings);
}

int main() {
    ifstream file("resources/datasets/dataset.final.tsv");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string row;
    while (getline(file, row)) {
        Node movie = parseTSVRow(row);
        //movie.display();
        //cout << "-----------------------------------" << endl;
    }

    file.close();

    return 0;
}