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

float calculateWeight(const Node &node1, const Node &node2) {
    // Example weight calculation based on genre similarity and rating difference
    set<string> genres1(node1.genres.begin(), node1.genres.end());
    set<string> genres2(node2.genres.begin(), node2.genres.end());

    // Calculate intersection size (common genres)
    vector<string> commonGenres;
    set_intersection(genres1.begin(), genres1.end(), genres2.begin(), genres2.end(),
                     back_inserter(commonGenres));
    int commonGenreCount = commonGenres.size();

    // Calculate rating difference
    float ratingDifference = fabs(node1.rating - node2.rating);

    // Define weight as a combination of these metrics
    return 1.0 / (1 + commonGenreCount + ratingDifference); // Higher similarity => Lower weight
}

int main() {
    ifstream file("resources/datasets/dataset.final.tsv");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string row;
    vector<Node> nodes;    // Store all nodes
    Graph movieGraph;      // Graph object

    while (getline(file, row)) {
        Node movie = parseTSVRow(row);
        nodes.push_back(movie); // Store the node in a vector
        //movie.display();
        //cout << "-----------------------------------" << endl;
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            float weight = calculateWeight(nodes[i], nodes[j]);
            movieGraph.addEdge(nodes[i].id, nodes[j].id, weight);
        }
    }

    file.close();

    movieGraph.displayGraph();

    return 0;
}