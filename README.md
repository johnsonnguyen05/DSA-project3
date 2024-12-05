
# Movie Recommendation System

## Overview

This program is a movie recommendation system that uses **graph algorithms** to suggest movies based on user input. It processes a dataset of movies, builds a graph where nodes represent movies and edges represent weighted relationships between them, and provides recommendations using either **Dijkstra's algorithm** or a **random walk** approach.

---

## Features

- **Graph Construction**:
  - Movies are represented as nodes.
  - Similarities between movies (calculated using custom weight functions) determine the edges and their weights.

- **Recommendation Algorithms**:
  - **Dijkstra's Algorithm**:
    Finds the shortest paths to the top-N most similar movies based on edge weights.
  - **Random Walk**:
    Randomly traverses the graph to explore movies related to the input movie.

- **Dataset Parsing**:
  Processes a TSV file containing movie data such as titles, genres, year, runtime, ratings, and number of ratings.

---

## Prerequisites

### Language and Tools
- **C++17 or later**
- A C++ compiler (e.g., `g++` or `clang`).
- OpenMP library for parallel processing (optional but recommended).

### Dataset
- Place a TSV file named `movies_filtered.tsv` in the `resources/datasets` directory. The file should have the following columns:
  ```
  ID    Type    Title    AltTitle    Year    Runtime    Genres    Rating    NumOfRatings
  ```

---

## Installation

1. Clone or download the repository.
2. Ensure your environment meets the prerequisites.
3. Navigate to the project directory and compile the program using:
   ```bash
   g++ -std=c++17 -fopenmp main.cpp -o movie_recommendation
   ```

---

## Usage

1. Run the program:
   ```bash
   ./movie_recommendation
   ```

2. Enter the title of a movie you like when prompted.

3. Select the algorithm for recommendations:
   - Press `D` for Dijkstra's algorithm.
   - Press `R` for Random Walk.

4. The program outputs a list of recommended movies based on your choice.

---

## Code Structure

### Key Classes and Functions
- **Node Class**:
  - Represents a movie with attributes like `id`, `title`, `genres`, `rating`, etc.

- **Graph Class**:
  - A graph structure storing movies as nodes and their relationships as weighted edges.
  - Includes methods for adding edges and displaying the graph.

- **Recommendation Functions**:
  - `dijkstra`: Finds the top-N most similar movies using shortest path computation.
  - `randomWalk`: Randomly selects connected movies for exploration.

---

## Example Output

Example of a program run:
```plaintext
Enter a movie that you like: Inception

SUCCESS! Movie found.

Select searching algorithm. (D) for Dijkstra's algorithm or (R) for random walk.
D

You have chosen Dijkstra's algorithm.
Top recommendations based on shortest path:
1. Interstellar
2. The Dark Knight
3. Memento
```

---

## Contributions

Contributions are welcome! Please feel free to open issues or submit pull requests for bug fixes, enhancements, or suggestions.

---

## License

This project is licensed under the MIT License.
