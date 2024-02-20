#include<iostream>
#include<vector>
#include<unordered_set>
#include<queue>
#include<stack>
#include<unordered_set>
#include"Node.h"
using namespace std;
//vector<int>goal_state = { 7, 14, 11, 5 , 8, 12, 13, 10 , 2, 0, 4, 15 , 6, 3, 9, 1 };
//vector<int>goal_state = { 1,2,3,4,5,6,7,8,9,0,11,12,13,10,14,15 };
vector<int>goal_state = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 11, 12, 13, 14, 15 };

namespace std {
    template<>
    struct hash<vector<int>> {
        size_t operator()(const vector<int>& v) const {
            size_t seed = v.size();
            for (auto& i : v) {
                seed ^= hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);                
            }
            return seed;
        }
    };
    template<>
    struct equal_to<vector<int>> {
        bool operator()(const vector<int>& lhs, const vector<int>& rhs) const {
            return lhs == rhs;
        }
    };
}

bool isGoalState(vector<int>&cur_state) {    
    for (int i = 0;i < 16;i++) {        
        if (cur_state[i] != goal_state[i]) {
            return false;
        }
    }
    return true;
}

bool existed(stack<Node*>s , Node* n) {   
    while (!s.empty()) {
        if (s.top()->state == n->state) {
            return true;
        }
        s.pop();
    }
    return false;
}

void traceSolution(vector<Node*> &sol, Node* g) {
    Node* curr = g;
    sol.push_back(g);
    while (curr->succ != NULL) {
        curr = curr->succ;
        sol.push_back(curr);
    }
    reverse(sol.begin(), sol.end());
    cout << "printing solution : " << endl;
    for (auto i : sol) {
        cout << i->move;
    }
    cout << endl;
}

bool depthLimitedSearch(int limit , stack<Node*>s , unordered_set<vector<int>>&total_state , vector<Node*>solution) {    
    while (!s.empty()) {       
        Node* curnode = s.top();       
        s.pop();
        
        if (isGoalState(curnode->state)) {
            cout << "find solution in steps " << limit << endl;
            traceSolution(solution, curnode);
            return true;
        }        
        
        if (curnode->depth < limit) {
            curnode->moveUp(); 
            curnode->moveRight();          
            curnode->moveDown();
            curnode->moveLeft();
            
            for (auto i : curnode->children) {
                if (total_state.find(i->state) == total_state.end()) {
                    s.push(i);
                    total_state.insert(curnode->state);
                }
            }
        }                     
    }
    return false;
}

int main(){
    /*vector<int>init_state;
    int num;
    for (int i = 0;i < 16;i++) {
       cin >> num;
       init_state.push_back(num);
    }*/
    stack<Node*>s;    
    /*vector<int>init_state = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };*/
    /*vector<int>init_state = { 7, 11, 5, 0 ,  8, 14, 12, 10 ,  2, 4, 13, 15 ,  6, 3, 9, 1 };*/
    /*vector<int>init_state = { 1,5,13,3,9,0,7,15,14,2,12,8,6,10,4,11 };*/
    vector<int>init_state = { 1, 2, 3, 0,  9, 5, 6, 4 ,  12, 10, 7, 8 ,  13, 14, 15, 11 };
    vector<Node*>sol;    
    Node init = Node(init_state, NULL);            

    int depthLimit = 0;
    bool found = false;
    cout << "start" << endl;
    
    while (!found) {
        unordered_set<vector<int>>total_state;
        init.depth = 0;
        s.push(&init);
        found = depthLimitedSearch(depthLimit,s,total_state,sol);        
        
        if (found) {
            cout << endl;            
            cout << "max number of states : " << total_state.size() << endl;
        }
        else {
            cout << "no solution within the depth limit " << depthLimit<< endl;
            depthLimit++;
        }        
    }    
}


