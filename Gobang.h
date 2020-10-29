//
// Created by nagisa on 2020/10/10.
//

#ifndef CHESS_GOBANG_H
#define CHESS_GOBANG_H

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <random>
#include <cmath>
#include "iomanip"

#define MAXNUM 30000
#define MINNUM -30000
#define LENGTH 15
#define CHECK(a, b) (a==b)?1:(a==-b)?2:0

using namespace std;

class Gobang {
public:
    Gobang() {
        for (int i = 0; i < LENGTH; i++) {
            for (int j = 0; j < LENGTH; ++j) {
                board[i][j] = 0;
            }
        }
        this->turn = 1;
        this->history = {};
        this->isGameOver = 0;
    }

public:
    bool MakeMove(pair<int, int> step);

    bool UnmakeMove();

    void Display() const;

    int Evaluate() const;

    vector<pair<int, int>> GetNearbySteps() const;

    pair<int, int> GetRandomStep() const;

    pair<int, int> GetLastStep() const;

    int CheckGameOver() const;

    int isGameOver;
    int turn;
    int board[LENGTH][LENGTH];
    stack<pair<int, int>> history;
};

class TreeNode {
public:
    TreeNode(Gobang *gobang, TreeNode *parent) {
        parentNode = parent;
        wincount = 0;
        visitcount = 0;
        this->state = *gobang;
        this->step = gobang->GetLastStep();
        auto steps = gobang->GetNearbySteps();
        std::random_device rd;
        std::default_random_engine g(rd());
        shuffle(steps.begin(), steps.end(), g);
        for (auto iter = steps.rbegin(); iter != steps.rend(); iter++) {
            pair<int, int> a = *iter;
            untriedActions.push(a);
        }
    }

    double GetUCB() const;

    bool IsFullExpand() const;

    bool IsRootNode() const;

    TreeNode *SimulationPolicy();

    pair<int, int> GetRandomAction();

    TreeNode *Select();

    TreeNode *Expand();

    int Rollout() const;

    void Backpropagation(bool isWin);


    static int C;
    Gobang state;
    int visitcount;
    int wincount;
    pair<int, int> step;
    TreeNode *parentNode;
    vector<TreeNode> childNodes;
    stack<pair<int, int>> untriedActions;
};

pair<int, int> MCTS(Gobang *gobang);

int NegaMax(Gobang *s, int depth);

int AlphaBeta(Gobang *s, int depth, int alpha, int beta);
void AspirationSearch(Gobang *s,int depth,int alpha, int beta);
int PrincipalVariationSearch(Gobang *s, int depth, int alpha, int beta);

//test
//    { 0, -1, 0, 0, 0, },
//    { 0, 1, -1, -1, 0, },
//    { -1, 1, -1, 1, 1, }
//    { 0, 0, 0, 0, 0, },
//    { 0, 0, 0, 0, 0, },
//


#endif //CHESS_GOBANG_H
