#include "backend.cpp"

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

    getline(ss, id, '\t');          // ID
    getline(ss, type, '\t');        // Type (ignored)
    getline(ss, title, '\t');       // Title
    getline(ss, isAdultStr, '\t');  // Is Adult
    getline(ss, year, '\t');        // Year
    getline(ss, runtimeStr, '\t');  // Runtime
    getline(ss, genresStr, '\t');   // Genres
    getline(ss, ratingStr, '\t');   // Rating
    getline(ss, numOfRatingsStr);   // Number of Ratings

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