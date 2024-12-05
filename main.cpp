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

vector<pair<string, float>> dijkstra(const Graph &graph, const string &startNode, int topN = 5) {
    unordered_map<string, float> distances; 
    unordered_map<string, string> previous;
    priority_queue<pair<float, string>, vector<pair<float, string>>, greater<>> pq;

    for (const auto &node : graph.adjList) {
        distances[node.first] = numeric_limits<float>::infinity();
    }
    distances[startNode] = 0.0f;

    pq.push({0.0f, startNode});

    while (!pq.empty()) {
        auto [currentDist, currentNode] = pq.top();
        pq.pop();

        if (currentDist > distances[currentNode]) {
            continue;
        }

        for (const auto &[neighbor, weight] : graph.adjList.at(currentNode)) {
            float newDist = currentDist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = currentNode;
                pq.push({newDist, neighbor});
            }
        }
    }
    vector<pair<string, float>> results;
    for (const auto &[node, dist] : distances) {
        if (node != startNode && dist < numeric_limits<float>::infinity()) {
            results.push_back({node, dist});
        }
    }

    sort(results.begin(), results.end(), [](const pair<string, float> &a, const pair<string, float> &b) {
        return a.second < b.second;
    });

    if (results.size() > topN) {
        results.resize(topN);
    }

    return results;
}

Node parseTSVRow(const string &row) {
    stringstream ss(row);
    string id, type, title, year, genresStr, altTitleStr, runtimeStr, ratingStr, numOfRatingsStr;

    getline(ss, id, '\t');       
    getline(ss, type, '\t');      
    getline(ss, title, '\t');     
    getline(ss, altTitleStr, '\t');  
    getline(ss, year, '\t');      
    getline(ss, runtimeStr, '\t'); 
    getline(ss, genresStr, '\t');  
    getline(ss, ratingStr, '\t');   
    getline(ss, numOfRatingsStr);   

    string altTitle = altTitleStr;
    string runtime = runtimeStr;
    float rating = stof(ratingStr);
    int numOfRatings = stoi(numOfRatingsStr);
    vector<string> genres = split(genresStr, ',');

    return Node(id, title, altTitle, year, runtime, genres, rating, numOfRatings);
}
float calculateWeight(const Node &node1, const Node &node2) {
    float combinedMetric1 = node1.numOfRatings * node1.rating;
    float combinedMetric2 = node2.numOfRatings * node2.rating;

    if (combinedMetric1 <= 0 || combinedMetric2 <= 0) {
        return 10000;
    }

    // Compute the weight
    float logMetric1 = log(combinedMetric1);
    float logMetric2 = log(combinedMetric2);

    float weight1 = 10000 / (logMetric1 * 4) + abs(stoi(node1.year) - stoi(node2.year))/6 + abs(stoi(node1.runtime) - stoi(node2.runtime))/10;
    float weight2 = 10000 / (logMetric2 * 4) + abs(stoi(node1.year) - stoi(node2.year))/6 + abs(stoi(node1.runtime) - stoi(node2.runtime))/10;

    return (weight1 + weight2) / 2.0f;
} 
vector<string> randomWalk(const Graph &graph, const string &startNode, int maxMovies = 5) {
    vector<string> visitedMovies;
    string currentNode = startNode;

    srand(static_cast<unsigned>(time(0)));

    for (int step = 0; step < maxMovies + 1; ++step) {
        if (!graph.adjList.at(currentNode).empty()) {
            const auto &neighbors = graph.adjList.at(currentNode);
            int randomIndex = rand() % neighbors.size();
            currentNode = neighbors[randomIndex].first;
        }

        visitedMovies.push_back(currentNode);
    }

    visitedMovies.erase(visitedMovies.begin());

    return visitedMovies;
}


int main() {
    cout << "Welcome to iLoveMovies!" << endl;
    ifstream file("resources/datasets/movies_filtered.tsv");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string row;
    vector<Node> nodes;               
    unordered_map<string, vector<int>> genreMap; 
    Graph movieGraph;                   

    while (getline(file, row)) {
        Node movie = parseTSVRow(row);
        int index = nodes.size();
        nodes.push_back(movie);

        for (const string &genre : movie.genres) {
            genreMap[genre].push_back(index);
        }
    }
    cout << "Loading " << nodes.size() << " movies..." << endl;
    unordered_map<int, vector<int>> genreHashBuckets;
    for (int i = 0; i < nodes.size(); ++i) {
        int hashValue = 0;
        for (const string &genre : nodes[i].genres) {
            hashValue ^= hash<string>{}(genre);
        }
    genreHashBuckets[hashValue].push_back(i);
    }

    for (const auto &[hashValue, indices] : genreHashBuckets) {
        #pragma omp parallel for 
        for (size_t i = 0; i < indices.size(); ++i) {
            int index1 = indices[i];

            for (size_t j = i + 1; j < indices.size(); ++j) {
                int index2 = indices[j];
                
                float weight = calculateWeight(nodes[index1], nodes[index2]); 
                movieGraph.addEdge(nodes[index1].id, nodes[index2].id, weight);
            }
        }
    }

    file.close();

    cout << "Enter a movie that you like: ";
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

    /*
    for (auto key : movieGraph.adjList) {
        if (key.first == movieNode.id) {
            for (auto neighbor : key.second) {
                cout << neighbor.first << ' ' << neighbor.second << endl;
            }
        }
    }
    */
    bool rw = false;
    bool dijsktra = false;
    if (isFound) {
        cout << endl << "SUCCESS! Movie found." << endl;
        //movieNode.display();
        cout << endl;
        cout << "Select searching algorithim. (D) for Dijkstra's algorithm or (R) for random walk." << endl;
        char choice;
        cin >> choice;
        if (choice == 'D') {
            dijsktra = true;
        }
        if (choice == 'R') {
            rw = true;
        }
    }
    else {
        cout << endl << "ERROR! Could not find movie." << endl;
        return 0;
    }

    if (dijsktra) {
        cout << "You have chosen Dijkstra's algorithm." << endl;

        vector<pair<string, float>> recommendations = dijkstra(movieGraph, movieNode.id);

        cout << "Top recommendations based on shortest path:\n";
        for (const auto &[movieId, distance] : recommendations) {
            auto it = find_if(nodes.begin(), nodes.end(), [&](const Node &node) { return node.id == movieId; });
            if (it != nodes.end()) {
                cout << "Title: " << it->title << endl;
            }
        }
    }

    if (rw) {
        cout << "You have chosen Random Walk." << endl;

        vector<string> visitedMovies = randomWalk(movieGraph, movieNode.id);

        cout << "5 random movies you might enjoy:\n";
        for (const string &movieId : visitedMovies) {
            auto it = find_if(nodes.begin(), nodes.end(), [&](const Node &node) { return node.id == movieId; });
            if (it != nodes.end()) {
                cout << "Title: " << it->title << endl;
            }
        }
    }

    return 0;
}
