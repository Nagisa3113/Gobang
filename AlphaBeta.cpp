//
// Created by nagisa on 2020/10/13.
//

#include "Gobang.h"

int searchDepth = 4;
pair<int, int> bestStep;

pair<int, int> directions[8] = {
        {0,  -1},
        {1,  -1},
        {1,  0},
        {1,  1},
        {0,  1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
};

vector<pair<int, vector<string>>> rules = {
        {0, {"011110"}},
        {0, {"211110", "011112", "10111",  "11101",  "11011"}},
        {0, {"01110",  "010110", "011010"}},
        {0, {"001112", "211100", "010112", "211010", "011012", "210110", "0100110", "0101010", "2011102"}},
        {0, {"001100", "01010",  "010010"}},
        {0, {"000112", "21100",  "001012", "210100", "010012", "210010"}}
};

void Gobang::Display() const {
    for (int temp = 0; temp < LENGTH; temp++) {
        if (temp == 0) cout << "  ";
        cout << setw(2) << temp + 1;
        if (temp == 4 || temp == 9) cout << " |";
        if (temp == LENGTH - 1)cout << endl;
    }
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (j == 0)cout << setw(2) << i + 1 << "|";
            int point = board[i][j];
            cout << ((point == 1) ? "●" : (point == -1) ? "○" : "+");
            cout << ' ';
            if (j == 4 || j == 9) cout << "| ";
        }
        cout <<
             endl;
        if (i == 4 || i == 9)
            cout << "   ---------------------------------" <<
                 endl;
    }
}

bool Gobang::MakeMove(pair<int, int> step) {
    if (isGameOver != 0) {
        cout << "invalid input because Game has been over!!!" << endl;
        return false;
    }

    int i = step.first;
    int j = step.second;
    if (i < 0 || i > LENGTH || j < 0 || j > LENGTH || board[i][j] != 0) {
        cout << "invalid value" << endl;
        return false;
    } else {
        history.push(step);
        board[i][j] = turn;
        turn = -turn;
    }
    this->isGameOver = CheckGameOver();
    return true;
}

bool Gobang::UnmakeMove() {
    if (history.empty()) {
        cout << "invalid" << endl;
        return false;
    } else {
        auto lastStep = history.top();
        board[lastStep.first][lastStep.second] = 0;
        turn = -turn;
        history.pop();
        return true;
    }

}

vector<pair<int, int>> Gobang::GetNearbySteps() const {
    vector<pair<int, int>> steps;
    auto h = this->history;
    while (!h.empty()) {
        auto step = h.top();
        for (auto dir:directions) {
            int dx = step.first + dir.first;
            int dy = step.second + dir.second;
            if (dx >= 0 && dx < LENGTH && dy >= 0 && dy < LENGTH) {
                if (board[dx][dy] == 0) {
                    steps.emplace_back(dx, dy);
                }
            }
        }
        h.pop();
    }
    return steps;
}

int Calculate(const vector<string> &arrays) {
    int count = 0;
    for (auto &rule:rules) {
        rule.first = 0;
    }
    for (auto str:arrays) {
        if (str.find(('1')) == str.npos) {
            continue;
        }
        for (auto &rule:rules) {
            for (auto ruleRegex:rule.second) {
                if (str.find(ruleRegex) != str.npos) {
                    rule.first++;
                }
            }
        }
    }

    if (rules[0].first >= 2) return 9050;
    if (rules[1].first > 0 && rules[2].first > 0) return 9020;
    if (rules[2].first >= 2) return 9010;
    count += rules[0].first * 4000 + rules[1].first * 3000 + rules[2].first * 600
             + rules[3].first * 300 + rules[4].first * 100 + rules[5].first * 50;
    return count;
}

int Gobang::Evaluate() const {
    int side = turn;

    string str1, str2;
    vector<string> array1, array2;

    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            str1.append(to_string(CHECK(board[i][j], side)));
            str2.append(to_string(CHECK(board[i][j], -side)));
        }
        array1.push_back(str1);
        str1 = "";
        array2.push_back(str2);
        str2 = "";
    }
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            str1.append(to_string(CHECK(board[j][i], side)));
            str2.append(to_string(CHECK(board[j][i], -side)));
        }
        array1.push_back(str1);
        str1 = "";
        array2.push_back(str2);
        str2 = "";
    }

    int xx, yy;
    xx = 0;
    yy = 0;
    for (; xx < LENGTH && yy < LENGTH; xx++, yy++) {
        str1.append(to_string(CHECK(board[xx][yy], side)));
        str2.append(to_string(CHECK(board[xx][yy], -side)));
    }
    array1.push_back(str1);
    str1 = "";
    array2.push_back(str2);
    str2 = "";

    xx = LENGTH - 1;
    yy = 0;
    for (; xx >= 0 && yy < LENGTH; xx++, yy++) {
        str1.append(to_string(CHECK(board[xx][yy], side)));
        str2.append(to_string(CHECK(board[xx][yy], -side)));
    }
    array1.push_back(str1);
    str1 = "";
    array2.push_back(str2);
    str2 = "";

    for (int i = LENGTH - 1 - 1; i >= 0; i--) {
        xx = i;
        yy = 0;
        for (; xx >= 0 && yy < LENGTH; xx--, yy++) {
            str1.append(to_string(CHECK(board[xx][yy], side)));
            str2.append(to_string(CHECK(board[xx][yy], -side)));
        }
        array1.push_back(str1);
        str1 = "";
        array2.push_back(str2);
        str2 = "";
        xx = LENGTH - 1;
        yy = i;
        for (; xx >= 0 && yy < LENGTH; xx--, yy++) {
            str1.append(to_string(CHECK(board[xx][yy], side)));
            str2.append(to_string(CHECK(board[xx][yy], -side)));

        }
        array1.push_back(str1);
        str1 = "";
        array2.push_back(str2);
        str2 = "";
    }
    for (int i = 0 + 1; i < LENGTH; i++) {
        xx = 0;
        yy = i;
        for (; xx < LENGTH && yy < LENGTH; xx++, yy++) {
            str1.append(to_string(CHECK(board[xx][yy], side)));
            str2.append(to_string(CHECK(board[xx][yy], -side)));

        }
        array1.push_back(str1);
        str1 = "";
        array2.push_back(str2);
        str2 = "";
        xx = i;
        yy = 0;
        for (; xx < LENGTH && yy < LENGTH; xx++, yy++) {
            str1.append(to_string(CHECK(board[xx][yy], side)));
            str2.append(to_string(CHECK(board[xx][yy], -side)));

        }
        array1.push_back(str1);
        str1 = "";
        array2.push_back(str2);
        str2 = "";
    }

    return Calculate(array1) - Calculate(array2);
}

int Gobang::CheckGameOver() const {
    int x = history.top().first;
    int y = history.top().second;
    vector<string> arrays;
    string str;
    int flag = board[x][y];
    str = "";
    for (int i = 0; i < LENGTH;
         i++) {
        str.append(to_string(CHECK(flag, board[x][i])));
    }
    arrays.push_back(str);
    str = "";
    for (int i = 0; i < LENGTH;
         i++) {
        str.append(to_string(CHECK(flag, board[i][y])));
    }
    arrays.push_back(str);
    str = "";
    if (x >= y) {
        int xx = x - y;
        int yy = 0;
        for (; xx < LENGTH;) {
            str.append(to_string(CHECK(flag, board[xx++][yy++])));
        }
    } else {
        int xx = 0;
        int yy = y - x;
        for (; yy < LENGTH;) {
            str.append(to_string(CHECK(flag, board[xx++][yy++])));
        }
    }
    arrays.push_back(str);
    str = "";
    if (x + y <= LENGTH - 1) {
        int xx = x + y;
        int yy = 0;
        for (; xx >= 0;) {
            str.append(to_string(CHECK(flag, board[xx--][yy++])));
        }
    } else {
        int xx = LENGTH
                 - 1;
        int yy = x + y - (LENGTH
                          - 1);
        for (; yy < LENGTH;) {
            str.append(to_string(CHECK(flag, board[xx--][yy++])));
        }
    }
    arrays.push_back(str);

    for (auto a:arrays) {
        if (a.find("11111") != a.npos) {
            return -turn;
        }
    }
    return 0;
}

pair<int, int> Gobang::GetRandomStep() const {
    int x, y;
    default_random_engine e;
    uniform_int_distribution<int> dist(0, LENGTH - 1);
    while (true) {
        x = dist(e);
        y = dist(e);
        if (board[x][y] != 0) {
            continue;
        }
        for (auto dir:directions) {
            int dx = x + dir.first;
            int dy = y + dir.second;
            if (dx >= 0 && dx < LENGTH && dy >= 0 && dy < LENGTH) {
                if (board[dx][dy] != 0) {
                    return make_pair(x, y);
                }
            }
        }
    }
}

pair<int, int> Gobang::GetLastStep() const {
    return history.top();
}

int NegaMax(Gobang *s, int depth) {
    int best = MINNUM;
    int score;
    if (s->isGameOver != 0) {
        return MINNUM;
    }
    if (depth <= 0) {
        return s->Evaluate();
    }
    for (auto step : s->GetNearbySteps()) {
        s->MakeMove(step);
        score = -NegaMax(s, depth - 1);
        s->UnmakeMove();
        if (score > best) {
            best = score;
            if (depth == searchDepth) {
                bestStep = step;
            }
        }
    }
    return best;
}

int AlphaBeta(Gobang *s, int depth, int alpha, int beta) {
    int score;
    if (s->isGameOver != 0) {
        return MINNUM;
    }
    if (depth <= 0) {
        return s->Evaluate();
    }
    auto steplists = s->GetNearbySteps();
    int index = 0, count = steplists.size();
    for (auto step : steplists) {
        s->MakeMove(step);
        score = -AlphaBeta(s, depth - 1, -beta, -alpha);
        s->UnmakeMove();
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            if (depth == searchDepth) {
                bestStep = step;
            }
        }
        if (depth == searchDepth) {
            cout << flush << "\r" << setw(2) << ++index << "/" << count <<
                 " step(" << setw(2) << step.first << "," << setw(2) << step.second << ")"
                 << " bestscore: " << alpha;
            if (index == count) cout << endl;
        }

    }
    return alpha;
}