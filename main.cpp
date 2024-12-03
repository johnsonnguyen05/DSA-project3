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
    string id, type, title, year, genresStr, altTitleStr, runtimeStr, ratingStr, numOfRatingsStr;

    getline(ss, id, '\t');          // ID
    getline(ss, type, '\t');        // Type (ignored)
    getline(ss, title, '\t');       // Title
    getline(ss, altTitleStr, '\t');  // Alt Title
    getline(ss, year, '\t');        // Year
    getline(ss, runtimeStr, '\t');  // Runtime
    getline(ss, genresStr, '\t');   // Genres
    getline(ss, ratingStr, '\t');   // Rating
    getline(ss, numOfRatingsStr);   // Number of Ratings

    string altTitle = altTitleStr;
    string runtime = runtimeStr;
    float rating = stof(ratingStr);
    int numOfRatings = stoi(numOfRatingsStr);
    vector<string> genres = split(genresStr, ',');

    return Node(id, title, altTitle, year, runtime, genres, rating, numOfRatings);
}
float calculateWeight(const Node &node1, const Node &node2) {
    // Combine ratings and number of ratings for weight calculation
    float combinedMetric1 = node1.numOfRatings * node1.rating;
    float combinedMetric2 = node2.numOfRatings * node2.rating;

    // Avoid log(0) by ensuring the metric is > 0
    if (combinedMetric1 <= 0 || combinedMetric2 <= 0) {
        return 10000; // Assign max weight for unratable movies
    }

    // Compute the weight
    float logMetric1 = log(combinedMetric1);
    float logMetric2 = log(combinedMetric2);

    float weight1 = 10000 / (logMetric1 * 3);
    float weight2 = 10000 / (logMetric2 * 3);

    // Return the average weight of the two movies
    return (weight1 + weight2) / 2.0f;
}
// Function to run Dijkstra's algorithm
vector<pair<string, float>> dijkstra(const Graph &graph, const string &startNode, int topN = 5) {
    unordered_map<string, float> distances; // Distance to each node
    unordered_map<string, string> previous; // Previous node in path
    priority_queue<pair<float, string>, vector<pair<float, string>>, greater<>> pq;

    // Initialize distances
    for (const auto &node : graph.adjList) {
        distances[node.first] = numeric_limits<float>::infinity();
    }
    distances[startNode] = 0.0f;

    // Push the starting node into the priority queue
    pq.push({0.0f, startNode});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        if (currentDist > distances[currentNode]) {
            continue;
        }

        // Explore neighbors
        for (const auto &[neighbor, weight] : graph.adjList.at(currentNode)) {
            float newDist = currentDist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = currentNode;
                pq.push({newDist, neighbor});
            }
        }
    }

    // Collect the top N nodes with the shortest distances
    vector<pair<string, float>> results;
    for (const auto &[node, dist] : distances) {
        if (node != startNode && dist < numeric_limits<float>::infinity()) {
            results.push_back({node, dist});
        }
    }

    // Sort results by distance and take the top N
    sort(results.begin(), results.end(), [](const pair<string, float> &a, const pair<string, float> &b) {
        return a.second < b.second;
    });

    if (results.size() > topN) {
        results.resize(topN);
    }

    return results;
}

int main() {
    ifstream file("resources/datasets/movies_filtered.tsv");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string row;
    vector<Node> nodes;                   // Store all nodes
    unordered_map<string, vector<int>> genreMap; // Map genre to list of movie indices
    Graph movieGraph;                     // Graph object

    // Read nodes and populate genre map
    while (getline(file, row)) {
        Node movie = parseTSVRow(row);
        int index = nodes.size();
        nodes.push_back(movie); // Store the node in a vector

        // Add movie index to genre map for each genre
        for (const string &genre : movie.genres) {
            genreMap[genre].push_back(index);
        }
    }
    // Step 1: Build genre buckets
    cout << nodes.size() << endl;
    unordered_map<int, vector<int>> genreHashBuckets;
    for (int i = 0; i < nodes.size(); ++i) {
        int hashValue = 0;
        for (const string &genre : nodes[i].genres) {
            hashValue ^= hash<string>{}(genre); // XOR genre hashes
        }
    genreHashBuckets[hashValue].push_back(i);
    }

    // Step 2: Create edges only within the same bucket
    for (const auto &[hashValue, indices] : genreHashBuckets) {
        #pragma omp parallel for // Optional for multi-threading
        for (size_t i = 0; i < indices.size(); ++i) {
            int index1 = indices[i];

            for (size_t j = i + 1; j < indices.size(); ++j) {
                int index2 = indices[j];
                
                float weight = calculateWeight(nodes[index1], nodes[index2]); // Simplified or exact
                movieGraph.addEdge(nodes[index1].id, nodes[index2].id, weight);
            }
        }
    }
    //movieGraph.displayGraph();

    file.close();

    cout << "Enter movie that you like: ";
    string movie;
    Node movieNode;
    getline(cin, movie);

    bool isFound = false;

    for (auto node : nodes) {
        if (node.title == movie) {
            isFound = true;
            movieNode = node;
        }
    }

    for (auto key : movieGraph.adjList) {
        if (key.first == movieNode.id) {
            for (auto neighbor : key.second) {
                cout << neighbor.first << ' ' << neighbor.second << endl;
            }
        }
    }
    bool rw = false;
    bool dijsktra = false;
    if (isFound) {
        cout << endl << "SUCCESS! Movie found." << endl;
        movieNode.display();
        cout << endl;
        cout << "Select searching algorithim. (D) for Dijkstra's algorithm or (R) for random walk" << endl;
        char choice;
        cin >> choice;
        if (choice == 'D') {
            dijsktra = true;
        }
        if (choice == 'R') {
            rw = true;
        }
    }

    if (dijsktra) {
        cout << "You have chosen Dijkstra's algorithm." << endl;

        vector<pair<string, float>> recommendations = dijkstra(movieGraph, movieNode.id);

        cout << "Top recommendations based on shortest path:\n";
        for (const auto &[movieId, distance] : recommendations) {
            auto it = find_if(nodes.begin(), nodes.end(), [&](const Node &node) { return node.id == movieId; });
            if (it != nodes.end()) {
                cout << "Title: " << it->title << ", Distance: " << distance << endl;
            }
        }
    }

    if (rw) {
    cout << "You have chosen Random Walk." << endl;

    vector<string> visitedMovies = randomWalk(movieGraph, movieNode.id);

    cout << "Movies visited during the random walk (up to 5 movies):\n";
    for (const string &movieId : visitedMovies) {
        auto it = find_if(nodes.begin(), nodes.end(), [&](const Node &node) { return node.id == movieId; });
        if (it != nodes.end()) {
            cout << "Title: " << it->title << endl;
        }
    }
}
    
    if (!isFound) {
        cout << "ERROR! Could not find movie." << endl;
    }

    return 0;
}
