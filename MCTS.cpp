//
// Created by nagisa on 2020/10/13.
//

#include "Gobang.h"

#define MAXSTEP 2000

int TreeNode::C = 2;

double TreeNode::GetUCB() const {
    double ucb;
    if (visitcount != 0)
        ucb = (wincount / visitcount) + sqrt(C * log(parentNode->visitcount) / visitcount);
    else ucb = 0;
    return ucb;
}

bool TreeNode::IsFullExpand() const {
    return untriedActions.empty();
}

bool TreeNode::IsRootNode() const {
    return parentNode == nullptr;
}


pair<int, int> TreeNode::GetRandomAction() {
    auto action = untriedActions.top();
    untriedActions.pop();
    return action;
}

void TreeNode::Backpropagation(bool isWin) {
    this->visitcount++;
    if (isWin) {
        this->wincount++;
    }
    if (!this->IsRootNode()) {
        this->parentNode->Backpropagation(isWin);
    }
}

TreeNode *TreeNode::Select() {
    double maxNum = 0;
    TreeNode *best = nullptr;
    for (auto &node:this->childNodes) {
        double num = node.GetUCB();
        if (num > maxNum) {
            maxNum = num;
            best = &node;
        }
    }
    return best;
}

TreeNode *TreeNode::Expand() {
    auto action = GetRandomAction();
    Gobang unreal = this->state;
    unreal.MakeMove(action);
    TreeNode childNode = TreeNode(&unreal, this);
    this->childNodes.push_back(childNode);
    return &childNodes.back();
}

int TreeNode::Rollout() const {
    Gobang unreal = state;
    while (unreal.isGameOver == 0) {
        auto nextStep = unreal.GetRandomStep();
        unreal.MakeMove(nextStep);
    }
    return unreal.isGameOver;
}

TreeNode *TreeNode::SimulationPolicy() {
    TreeNode *currentNode = this;
    while (true) {
        if (currentNode->state.isGameOver != 0)
            break;
        if (currentNode->IsFullExpand())
            currentNode = currentNode->Select();
        else
            return currentNode->Expand();
    }
    return currentNode;
}


pair<int, int> MCTS(Gobang *gobang) {
    TreeNode root = TreeNode(gobang, nullptr);
    TreeNode currentNode = root;
    int MAXSTEPS = MAXSTEP;

    for (int i = 0; i < MAXSTEPS; i++) {
        TreeNode *leafNode = currentNode.SimulationPolicy();
        int isWin = leafNode->Rollout();
        leafNode->Backpropagation(isWin == gobang->turn);
        cout << flush << "\r" << "MCTS: "<<setw(4) << i << '/' << setw(4) << MAXSTEPS - 1;
    }

    TreeNode *bestNode = currentNode.Select();
    cout << endl << "visitcount: " << bestNode->visitcount << " wincount: " << bestNode->wincount << endl;
    return bestNode->step;
}
