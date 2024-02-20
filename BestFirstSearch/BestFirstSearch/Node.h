#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<stack>
#include<memory>
#include<unordered_set>

using namespace std;

const int puzzle_size = 4;
namespace std {	
	template<>
	struct hash<vector<vector<int>>> {
		size_t operator()(const vector<vector<int>>& v) const {
			size_t seed = v.size();
			for (auto& i : v) {
				for (auto& j : i) {
					seed ^= hash<int>()(j) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				}
			}
			return seed;
		}
	};
	template<>
	struct equal_to<vector<vector<int>>> {
		bool operator()(const vector<vector<int>>& lhs, const vector<vector<int>>& rhs) const {
			return lhs == rhs;
		}
	};
}

class Puzzle {
	protected :
		struct Node {
			vector<vector<int>>state;
			shared_ptr<Node> parent;
			pair<int, int> zPos;

			Node(vector<vector<int>>s, shared_ptr<Node>suc, pair<int, int> z) :
				state(s),parent(move(suc)), zPos(z) {}
		};
	public :
		pair<int,int> findZero(vector<vector<int>>state) {
			pair<int, int> zPos = { 0,0 };
			for (int i = 0;i < puzzle_size;i++) {
				for (int j = 0;j < puzzle_size;j++) {
					if (state[i][j] == 0) {
						return { i,j };
					}
				}
			}
			return zPos;
		}

		bool valid_move(pair<int , int>curpos) {
			return curpos.first >= 0 && curpos.first < puzzle_size&& curpos.second >= 0 && curpos.second < puzzle_size;
		}

		vector<vector<int>> moveblank(vector<vector<int>>&state , pair<int , int>move) {
			vector<vector<int>>child_state;
			pair<int, int>zPos = findZero(state);
			child_state.assign(state.begin(), state.end());

			int new_x = zPos.first + move.first;
			int new_y = zPos.second + move.second;

			if (valid_move({new_x , new_y})) {
				swap(child_state[zPos.first][zPos.second] , child_state[new_x][new_y]);
			}
			else {
				return {};
			}

			return child_state;
		}
		int heuristic(vector<vector<int>>& curstate, vector<vector<int>>& final_state) {
			int distance = 0;
			vector<pair<int, int>>a(puzzle_size * puzzle_size), b(puzzle_size * puzzle_size);

			for (int i = 0;i < puzzle_size;i++) {
				for (int j = 0;j < puzzle_size;j++) {
					a[curstate[i][j]] = { i,j };
					b[final_state[i][j]] = { i,j };
				}
			}
			for (int i = 0;i < puzzle_size * puzzle_size;i++) {
				distance += Manhattan(a[i], b[i]);
			}
			return distance;
		}

		int Manhattan(pair<int, int>a, pair<int, int>b) {
			return  abs(a.first - b.first) + abs(a.second - b.second);
		}

		void traceSolution(vector<shared_ptr<Node>>sol, shared_ptr<Node> g , size_t maxstate) {
			shared_ptr<Node> curr = g;
			sol.push_back(g);
			while (curr->parent != NULL) {
				curr = curr->parent;
				sol.push_back(curr);
			}
			reverse(sol.begin(), sol.end());

			std::cout << "左上是(0,0) ， <右，下>為正" << endl;
			std::cout << "printing solution : " << endl;				

			for (int i = 0;i < sol.size();i++) {
				std::cout << "0的位置 : " << sol[i]->zPos.first<< "," << sol[i]->zPos.second << endl;
			}				
			std::cout <<"number of movements from the initial state to the goal state : " << sol.size() - 1 << endl;
			std::cout << "maximum number of states ever saved in the memory : " << maxstate << endl;
			std::cout << endl;
		}

		bool isGoalState(vector<vector<int>>& cur_state, vector<vector<int>>& goal_state) {
			for (int i = 0;i < puzzle_size;i++) {
				for (int j = 0;j < puzzle_size;j++) {
					if (cur_state[i][j] != goal_state[i][j]) {
						return false;
					}
				}
			}
			return true;
		}
};

class BFS : Puzzle{
	protected:
		struct BFSNode :  Node{
			int total_cost = 0;
			int path_cost = 0;
			BFSNode(vector<vector<int>>state, shared_ptr<BFSNode>p, pair<int, int>pos, int t_c, int p_c) :
				Node(move(state), move(p), pos), total_cost(t_c), path_cost(p_c) {}
		};
		struct CompareCost {
			bool operator()(shared_ptr<BFSNode> a, shared_ptr<BFSNode> b) const {
				return a->total_cost > b->total_cost;
			}
		};		
	public:	
		bool BestFirstSearch(vector<vector<int>>&init_state , vector<vector<int>>&goal_state , int w1 , int w2) {			
			unordered_set<vector<vector<int>>> reached;			

			priority_queue<shared_ptr<BFSNode>, vector<shared_ptr<BFSNode>>, CompareCost>pq;			
			shared_ptr<BFSNode> initial = make_shared<BFSNode>(init_state, nullptr, findZero(init_state), heuristic(init_state, goal_state), 0);
			pq.push(initial);

			vector<shared_ptr<Node>>sol;
			vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };//上、下、左、右

			while (!pq.empty()) {
				shared_ptr<BFSNode> curnode = pq.top();
				pq.pop();
				reached.insert(curnode->state);

				if (isGoalState(curnode->state, goal_state)) {
					traceSolution(sol,curnode,reached.size());
					return true;
				}				

				for (const auto& dir : directions) {
					vector<vector<int>> newState = moveblank(curnode->state, dir);
					if (newState.empty()) {
						continue;
					}
					pair<int, int> newPos = findZero(newState);
					int path_cost = curnode->path_cost + 1;
					int total_cost = w1 * path_cost  + w2 * heuristic(newState, goal_state);

					if (reached.find(newState) == reached.end()) {
						shared_ptr<BFSNode> newNode = make_shared<BFSNode>(newState, curnode, newPos, total_cost, path_cost);
						pq.push(newNode);
					}
				}
			}			
			return false;
		}
};

class RBFS : Puzzle {
	protected :
		struct RBFSNode : Node {
			int total_cost = 0;
			int path_cost = 0;			
			RBFSNode(vector<vector<int>> state, shared_ptr<RBFSNode> p, pair<int, int> pos, int t_c, int p_c) :
				Node(move(state), move(p), pos), total_cost(t_c), path_cost(p_c) {}
		};		
		struct CompareCost {
			bool operator()(shared_ptr<RBFSNode> a, shared_ptr<RBFSNode> b) const {
				return a->total_cost > b->total_cost;
			}
		};

	public : 
		unordered_set<vector<vector<int>>> reached;
		vector<shared_ptr<Node>>sol;
		size_t max_state = 0;
		pair <bool,int> RBFSrecursive(shared_ptr<RBFSNode> &curnode, vector<vector<int>> &goal_state, int w1, int w2 , int &f_limit , size_t count) {
			if (isGoalState(curnode->state, goal_state)) {		
				traceSolution(sol, curnode, max_state);				
				return { true , curnode->total_cost};
			}

			priority_queue<shared_ptr<RBFSNode>, vector<shared_ptr<RBFSNode>>, CompareCost>successors;
			vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };			

			for (const auto& dir : directions) {
				vector<vector<int>> nextState = moveblank(curnode->state, dir);
				if (nextState.empty()) {
					continue;
				}

				pair<int, int> newPos = findZero(nextState);

				int path_cost = curnode->path_cost + 1;
				int total_cost = w1 * path_cost + w2 * heuristic(nextState, goal_state);
				int max_f = max(total_cost, curnode->total_cost);

				shared_ptr<RBFSNode> nextNode = make_shared<RBFSNode>(nextState, curnode, newPos, max_f, path_cost);
				
				successors.push(nextNode);
				count++;
				max_state = max(max_state, count);
			}

			while (!successors.empty()) {
				shared_ptr<RBFSNode>best = successors.top();
				successors.pop();
				
				if (best->total_cost > f_limit) {
					return { false , best->total_cost };
				}
				int min_f = f_limit;
				if (!successors.empty()) {
					shared_ptr<RBFSNode>alternative = successors.top();
					min_f = min(f_limit, alternative->total_cost);
				}				
				
				pair<bool , int> found = RBFSrecursive(best, goal_state, w1, w2, min_f , count);
				best->total_cost = found.second;
				successors.push(best);

				if (found.first) {						
					return { true , found.second};
				}
			}

			return { false , INT_MAX};
		}

		pair<bool , int> RBFSFunction(vector<vector<int>>& init_state, vector<vector<int>>& goal_state, int w1, int w2) {
			int maxF = INT_MAX;
			pair<int, int>zpos = findZero(init_state);
			int h = heuristic(init_state, goal_state);
			shared_ptr<RBFSNode> initial = make_shared<RBFSNode>(init_state, nullptr, zpos, h , 0);
			int count = 0;
			return RBFSrecursive(initial, goal_state, w1 , w2, maxF , count);
		}
		
};
#endif

