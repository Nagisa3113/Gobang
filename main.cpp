#include "Gobang.h"

extern int searchDepth;
extern pair<int, int> bestStep;


//Select Search Algorithm
pair<int, int> SearchBestStep(const Gobang *s, int depth) {
    bestStep = make_pair(-1, -1);
    Gobang unrealboard = *s;
//    NegaMax(&unrealboard, depth);
//    AlphaBeta(&unrealboard, depth, MINNUM, MAXNUM);
//    AspirationSearch(&unrealboard, depth, MINNUM, MAXNUM);
//    PrincipalVariationSearch(&unrealboard, depth, MINNUM, MAXNUM);
    bestStep = MCTS(&unrealboard);
    return bestStep;
}

void HumanMove(Gobang *gobang) {
    int i, j;
    do {
        cout << "input position x and y:";
        cin >> i >> j;
    } while (!gobang->MakeMove({i - 1, j - 1}));
}

void AIMove(Gobang *gobang) {
    char nextTurn;
    cout << ">>> press Return to next step" << endl;
    do {
        nextTurn = cin.get();
    } while (nextTurn != '\n');
    cout << "please wait..." << endl;
    auto bestStep = SearchBestStep(gobang, searchDepth);
    gobang->MakeMove(bestStep);
    gobang->Display();
    cout << "===" << ((gobang->turn == 1) ? "Black" : "White")
         << "'s Step is " << bestStep.first + 1 << "," << bestStep.second + 1 << endl;
}

int main() {
    Gobang gobang;
    gobang.MakeMove({8, 6});
    gobang.MakeMove({7, 6});


    gobang.Display();
    while (gobang.isGameOver == 0) {
//        HumanMove(&gobang);
        AIMove(&gobang);
    }

    return 0;
}