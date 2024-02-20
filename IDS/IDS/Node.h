#ifndef INCLUDE_NODE_H_
#define INCLUDE_NODE_H_

#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<stack>
#include<memory>

using namespace std;
class Node {
	public:		
		int depth = 0;
		string move = "";
		vector<int>state;
		vector<Node*>children;
		Node* succ;

		Node(vector<int>s, Node* suc) {
			state.assign(s.begin(), s.end());
			succ = suc;
		}

		bool operator!=(const shared_ptr<Node>& a) {
			bool result = false;
			for (int i = 0;i < a->state.size();i++) {
				if (state[i] != a->state[i]) {
					result = true;
				}
			}
			return result;
		}

		bool operator==(const shared_ptr<Node>& a) {
			bool result = true;
			for (int i = 0;i < a->state.size();i++) {
				if (state[i] != a->state[i]) {
					result = false;
				}
			}
			return result;
		}

		void printstate();
		int findZero();
		void moveUp();
		void moveDown();
		void moveRight();
		void moveLeft();
};

#endif
