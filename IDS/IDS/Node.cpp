#include "Node.h"
#include<iostream>
using namespace std;

void Node::printstate() {	
	for (int i = 0;i < 16;i++) {
		if (i % 4 == 0) {
			cout << endl;
		}

		if (state[i] < 10) {
			cout << state[i] << "  ";
		}
		else {
			cout << state[i] << " ";
		}				
	}
	cout << endl;
}

int Node::findZero() {
	int zPos = 0;
	for (int i = 0;i < 16;i++) {
		if (state[i] == 0) {
			zPos = i;
			break;
		}
	}	
	return zPos;
}

void Node::moveUp() {
	int zPos = findZero();	

	if (zPos != 0 && zPos != 1 && zPos != 2 && zPos != 3) {
		vector<int> temp;
		temp.assign(state.begin(), state.end());

		swap(temp[zPos], temp[zPos - 4]);
		Node* child = new Node(temp, this);
		child->move = "up ";
		child->depth = depth + 1;
		children.push_back(child);
	}
}
void Node::moveDown() {
	int zPos = findZero();	

	if (zPos != 12 && zPos != 13 && zPos != 14 && zPos != 15) {
		vector<int> temp;
		temp.assign(state.begin(), state.end());

		swap(temp[zPos], temp[zPos + 4]);
		Node* child = new Node(temp, this);
		child->move = "down ";
		child->depth = depth + 1;		

		children.push_back(child);
	}	
}

void Node::moveRight() {
	int zPos = findZero();	

	if (zPos != 3 && zPos != 7 && zPos != 11 && zPos != 15) {
		vector<int> temp;
		temp.assign(state.begin(), state.end());

		swap(temp[zPos], temp[zPos + 1]);
		Node* child = new Node(temp, this);
		child->move = "right ";
		child->depth = depth + 1;
		children.push_back(child);
	}		
}
void Node::moveLeft() {
	int zPos = findZero();	

	if (zPos != 0 && zPos != 4 && zPos != 8 && zPos != 12) {
		vector<int> temp;
		temp.assign(state.begin(), state.end());

		swap(temp[zPos], temp[zPos - 1]);
		Node* child = new Node(temp, this);
		child->move = "left ";
		child->depth = depth + 1;
		children.push_back(child);
	}	
}