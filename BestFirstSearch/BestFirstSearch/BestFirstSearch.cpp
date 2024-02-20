#include"Node.h"
#include <iostream>
using namespace std;

//vector<vector<int>>goal_state = { {1, 2, 3, 0}, {9, 5, 6, 4}, {12, 10, 7, 8}, {13, 14, 15, 11} };
vector<vector<int>>goal_state = { {1, 2, 3, 0}, {9, 5, 6, 4}, {12, 10, 7, 8}, {13, 14, 15, 11} };

void print_state(vector<vector<int>>state) {
    for (int i = 0;i < puzzle_size;i++) {
        for (int j = 0;j < puzzle_size;j++) {
            if (state[i][j] > 10) {
                std::cout << state[i][j] << " ";
            }
            else {
                std::cout << state[i][j] << "  ";
            }
        }
        std::cout << endl;
    }
}
int main() {
    //vector<vector<int>>init_state = { {1,5,13,3},{9,0,7,15},{14,2,12,8},{6,10,4,11} };
    /*vector<vector<int>>init_state = { {1,5,13,9},{7,11,14,4},{15,0,2,12},{3,6,8,10} };*/
    /*vector<vector<int>>init_state = { {8,2,6,4},{0,13,3,14},{1,7,12,5},{10,11,9,15} };*/
    vector<vector<int>>init_state = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 0, 10, 11 }, { 12, 13, 14, 15 } };


    BFS object;
    Puzzle p;
    /*std::cout << p.heuristic(init_state, goal_state);*/
    pair<int, int>UCS = { 1,0 }, GBFS = { 0,1 }, A_star = { 1,1 };
    std::cout << "Running Uniform Cost Search..." << endl;
    object.BestFirstSearch(init_state, goal_state , UCS.first , UCS.second);
    std::cout << "Running Greedy Best First Search..." << endl;
    object.BestFirstSearch(init_state, goal_state, GBFS.first, GBFS.second);
    std::cout << "Running A_star search..." << endl;
    object.BestFirstSearch(init_state, goal_state, A_star.first, A_star.second);

    RBFS recursive_object;
    std::cout << "Running Recursive Best First search..." << endl;
    recursive_object.RBFSFunction(init_state, goal_state, A_star.first, A_star.second);

    return 0;
};
