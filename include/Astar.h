#include <bits/stdc++.h>

#include <SFML/System/Vector2.hpp>
#include <list>

using namespace std;

// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int>> pPair;

// A structure to hold the neccesary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= mCol-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};

class Astar {
   private:
    int mRow;
    int mCol;
    list<sf::Vector2f> path;
    std::vector<std::vector<int>> grid;

    // A Utility Function to check whether given cell (row, col)
    // is a valid cell or not.
    bool isValid(int row, int col);

    // A Utility Function to check whether the given cell is
    // blocked or not
    bool isUnBlocked(int row, int col);
    bool isDiagonalUnBlocked(int rowSrc, int colSrc, int rowDest, int colDest);

    // A Utility Function to check whether destination cell has
    // been reached or not
    bool isDestination(int row, int col, Pair dest);

    // A Utility Function to calculate the 'h' heuristics.
    double calculateHValue(int row, int col, Pair dest);

    // A Utility Function to trace the path from the source
    // to destination
    void tracePath(vector<vector<cell>> cellDetails, Pair dest);

   public:
    Astar(std::vector<std::vector<int>> collisionMap) {
        grid = collisionMap;
        mRow = grid[0].size();
        mCol = grid.size();
    }

    Astar() {
        mRow = 0;
        mCol = 0;
    }

    void setCollisionMap(std::vector<std::vector<int>> collisionMap) {
        grid = collisionMap;
        mRow = grid[0].size();
        mCol = grid.size();
    }
    // A Function to find the shortest path between
    // a given source cell to a destination cell according
    // to A* Search Algorithm
    void aStarSearch(Pair src, Pair dest);

    list<sf::Vector2f> getPath() { return path; }
};
