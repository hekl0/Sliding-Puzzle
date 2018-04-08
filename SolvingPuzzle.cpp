#include<vector>
#include<queue>
#include<set>
#include<map>
#include<main.h>
#include <iostream>

struct compare {
    bool operator () (State a, State b) const {
        return a.cost + SolvingPuzzle::HeuristicCost(a) >= b.cost + SolvingPuzzle::HeuristicCost(b);
    }
};

priority_queue<State, vector<State>, compare> heap;
set<State> visited;

string SolvingPuzzle::solvePuzzle(GAME gameValue) {
    State temp(gameValue.height, 0, gameValue.table, gameValue.pos0, "");

    if (gameValue.height == 4)
        restruct(temp);

    heap.push(temp);
    visited.insert(temp);

    //A* algorithm
    State node;
    while (true) {
        node = heap.top();
        heap.pop();

        neighbor(node);


        if (HeuristicCost(node) == 0) break;
    }

    visited.clear();
    heap = priority_queue<State, vector<State>, compare>();

    //optimize solution
    string data = node.lastMove;
    do {
        node.lastMove = data;
        data = "";
        for (int i = 0; i < node.lastMove.length(); i++) {
            if (i < node.lastMove.length()-1) {
                if (node.lastMove[i] == 'R' && node.lastMove[i+1] == 'L') continue;
                if (node.lastMove[i] == 'D' && node.lastMove[i+1] == 'U') continue;
                if (node.lastMove[i] == 'L' && node.lastMove[i+1] == 'R') continue;
                if (node.lastMove[i] == 'U' && node.lastMove[i+1] == 'D') continue;
            }
            if (i > 0) {
                if (node.lastMove[i-1] == 'R' && node.lastMove[i] == 'L') continue;
                if (node.lastMove[i-1] == 'D' && node.lastMove[i] == 'U') continue;
                if (node.lastMove[i-1] == 'L' && node.lastMove[i] == 'R') continue;
                if (node.lastMove[i-1] == 'U' && node.lastMove[i] == 'D') continue;
            }
            data += node.lastMove[i];
        }
    } while (data != node.lastMove);
    node.lastMove = data;

    return node.lastMove;
}

void SolvingPuzzle::neighbor(State node) {
    State temp;
    if (node.emptySlot.X > 0) {
        temp = node;
        temp.emptySlot.X--;
        temp.lastMove += "D";
        swap(temp.table[node.emptySlot.X][node.emptySlot.Y],
             temp.table[temp.emptySlot.X][temp.emptySlot.Y]);
        temp.cost++;

        if (visited.count(temp) == 0) {
            heap.push(temp);
            visited.insert(temp);
        }
    }

    if (node.emptySlot.X < node.sizeN - 1) {
        temp = node;
        temp.emptySlot.X++;
        temp.lastMove += "U";
        swap(temp.table[node.emptySlot.X][node.emptySlot.Y],
             temp.table[temp.emptySlot.X][temp.emptySlot.Y]);
        temp.cost++;

        if (visited.count(temp) == 0) {
            heap.push(temp);
            visited.insert(temp);
        }
    }

    if (node.emptySlot.Y > 0) {
        temp = node;
        temp.emptySlot.Y--;
        temp.lastMove += "R";
        swap(temp.table[node.emptySlot.X][node.emptySlot.Y],
             temp.table[temp.emptySlot.X][temp.emptySlot.Y]);
        temp.cost++;
        if (visited.count(temp) == 0) {
            heap.push(temp);
            visited.insert(temp);
        }
    }

    if (node.emptySlot.Y < node.sizeN - 1) {
        temp = node;
        temp.emptySlot.Y++;
        temp.lastMove += "L";
        swap(temp.table[node.emptySlot.X][node.emptySlot.Y],
             temp.table[temp.emptySlot.X][temp.emptySlot.Y]);
        temp.cost++;

        if (visited.count(temp) == 0) {
            heap.push(temp);
            visited.insert(temp);
        }
    }
}

int SolvingPuzzle::HeuristicCost(State node) {
    int res = 0;

    for (int i = 0; i < node.sizeN; i++)
        for (int j = 0; j < node.sizeN; j++)
            if (node.table[i][j] != 0) {
                int temp = abs(i - (node.table[i][j]-1) / node.sizeN) + abs(j - (node.table[i][j]-1) % node.sizeN);
                if (node.sizeN == 4)
                    if (node.table[i][j] == 1 || node.table[i][j] == 2 ||
                        node.table[i][j] == 3 || node.table[i][j] == 4 ||
                        node.table[i][j] == 5 || node.table[i][j] == 9 ||
                        node.table[i][j] == 13) temp *= 1000;
                res += temp;
            }

    return res;
}

void SolvingPuzzle::moveBlock(string moves, State &node) {
    for (int i = 0; i < moves.length(); i++) {
        node.lastMove += moves[i];

        if (moves[i] == 'L') {
            swap(node.table[node.emptySlot.X][node.emptySlot.Y], node.table[node.emptySlot.X][node.emptySlot.Y+1]);
            node.emptySlot.Y++;
        }

        if (moves[i] == 'R') {
            swap(node.table[node.emptySlot.X][node.emptySlot.Y], node.table[node.emptySlot.X][node.emptySlot.Y-1]);
            node.emptySlot.Y--;
        }

        if (moves[i] == 'U') {
            swap(node.table[node.emptySlot.X][node.emptySlot.Y], node.table[node.emptySlot.X+1][node.emptySlot.Y]);
            node.emptySlot.X++;
        }

        if (moves[i] == 'D') {
            swap(node.table[node.emptySlot.X][node.emptySlot.Y], node.table[node.emptySlot.X-1][node.emptySlot.Y]);
            node.emptySlot.X--;
        }
    }
}

COORD SolvingPuzzle::locate(int value, State node) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (node.table[i][j] == value)
                return {i,j};
}

void SolvingPuzzle::restruct(State &node) {
    COORD target;
    //move 1
    if (locate(1, node).X != 0) {
        while (node.emptySlot.X != locate(1, node).X - 1)
            if (node.emptySlot.X < locate(1, node).X - 1) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != locate(1, node).Y)
            if (node.emptySlot.Y < locate(1, node).Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (locate(1, node).X != 0) {
            moveBlock("U", node);
            if (locate(1, node).X != 0)
                if (locate(1, node).Y != 3) moveBlock("LDDR", node);
                else moveBlock("RDDL", node);
        }
    }

    if (locate(1, node).Y != 0) {
        while (node.emptySlot.Y != locate(1, node).Y - 1)
            if (node.emptySlot.Y < locate(1, node).Y - 1) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != locate(1, node).X)
            moveBlock("D", node);
        while (locate(1, node).Y != 0) {
            moveBlock("L", node);
            if (locate(1, node).Y != 0) moveBlock("URRD", node);
        }
    }
    //end 1

    //move 2
    if (locate(2, node).Y != 1) {
        if (locate(2, node).Y == 0) target = {locate(2, node).X, 1};
        else target = {locate(2, node).X, locate(2, node).Y-1};
        while (node.emptySlot.Y != target.Y)
            if (node.emptySlot.Y < target.Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != target.X)
            if (node.emptySlot.X < target.X) moveBlock("U", node);
            else moveBlock("D", node);
        while (locate(2, node).Y != 1) {
            if (node.emptySlot.Y > locate(2, node).Y) moveBlock("R", node);
            else moveBlock("L", node);
            if (locate(2, node).Y != 1)
                if (locate(2, node).X != 3) moveBlock("URRD", node);
                else moveBlock("DRRU", node);
        }
    }

    if (locate(2, node).X != 0) {
        if (node.emptySlot.X == locate(2, node).X)
            if (node.emptySlot.X != 3) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != locate(2, node).Y)
            if (node.emptySlot.Y < locate(2, node).Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != locate(2, node).X-1)
            if (node.emptySlot.X < locate(2, node).X-1) moveBlock("U", node);
            else moveBlock("D", node);
        while (locate(2, node).X != 0) {
            moveBlock("U", node);
            if (locate(2, node).X != 0) moveBlock("LDDR", node);
        }
    }
    //end 2

    //move 4
    if (node.emptySlot.X == 0) moveBlock("U", node);
    if (locate(4, node).Y != 2) {
        if (locate(4, node).Y < 2) target = {locate(4, node).X, locate(4, node).Y+1};
        else target = {locate(4, node).X, 2};
        while (node.emptySlot.Y != target.Y)
            if (node.emptySlot.Y < target.Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != target.X)
            if (node.emptySlot.X < target.X) moveBlock("U", node);
            else moveBlock("D", node);
        while (locate(4, node).Y != 2) {
            if (node.emptySlot.Y > locate(4, node).Y) moveBlock("R", node);
            else moveBlock("L", node);
            if (locate(4, node).Y != 2)
                if (locate(4, node).X != 3) moveBlock("ULLD", node);
                else moveBlock("DLLU", node);
        }
    }

    if (locate(4, node).X != 0) {
        if (node.emptySlot.X == locate(4, node).X)
            if (node.emptySlot.X != 3) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != locate(4, node).Y)
            if (node.emptySlot.Y < locate(4, node).Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != locate(4, node).X-1)
            if (node.emptySlot.X < locate(4, node).X-1) moveBlock("U", node);
            else moveBlock("D", node);
        while (locate(4, node).X != 0) {
            moveBlock("U", node);
            if (locate(4, node).X != 0) moveBlock("LDDR", node);
        }
    }
    //end 4

    //move 3
    if (node.emptySlot.X == 0) moveBlock("U", node);
    if (node.table[0][3] == 3) {
        if (node.emptySlot.Y != 3) moveBlock("L", node);
        moveBlock("DRUULDRULDRDLURU", node);
    }

    if (locate(3, node).Y != 2) {
        if (locate(3, node).Y < 2) target = {locate(3, node).X, locate(3, node).Y+1};
        else target = {locate(3, node).X, 2};
        while (node.emptySlot.Y != target.Y)
            if (node.emptySlot.Y < target.Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != target.X)
            if (node.emptySlot.X < target.X) moveBlock("U", node);
            else moveBlock("D", node);
        while (locate(3, node).Y != 2) {
            if (node.emptySlot.Y > locate(3, node).Y) moveBlock("R", node);
            else moveBlock("L", node);
            if (locate(3, node).Y != 2)
                if (locate(3, node).X != 3) moveBlock("ULLD", node);
                else moveBlock("DLLU", node);
        }
    }

    if (locate(3, node).X != 1) {
        if (node.emptySlot.X == locate(3, node).X)
            if (node.emptySlot.X != 3) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != locate(3, node).Y)
            if (node.emptySlot.Y < locate(3, node).Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != locate(3, node).X-1)
            if (node.emptySlot.X < locate(3, node).X-1) moveBlock("U", node);
            else moveBlock("D", node);
        while (locate(3, node).X != 1) {
            moveBlock("U", node);
            if (locate(3, node).X != 1) moveBlock("LDDR", node);
        }
    }
    //end 3

    //finish first row
    if (node.emptySlot.X == 1) moveBlock("U", node);
    while (node.emptySlot.Y != 3) moveBlock("L", node);
    while (node.emptySlot.X != 0) moveBlock("D", node);
    moveBlock("RU", node);
    //

    //move 5
    if (locate(5, node).X != 1) {
        while (node.emptySlot.X < locate(5, node).X - 1) moveBlock("U", node);
        while (node.emptySlot.Y != locate(5, node).Y)
            if (node.emptySlot.Y < locate(5, node).Y) moveBlock("L", node);
            else moveBlock("R", node);
        while (locate(5, node).X != 1) {
            moveBlock("U", node);
            if (locate(5, node).X != 1)
                if (locate(5, node).Y != 3) moveBlock("LDDR", node);
                else moveBlock("RDDL", node);
        }
    }

    if (locate(5, node).Y != 0) {
        if (locate(5, node).Y == node.emptySlot.Y)
            if (node.emptySlot.Y != 3) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != 1) moveBlock("D", node);
        while (node.emptySlot.Y != locate(5, node).Y-1)
            if (node.emptySlot.Y < locate(5, node).Y-1) moveBlock("L", node);
            else moveBlock("R", node);
        while (locate(5, node).Y != 0) {
            moveBlock("L", node);
            if (locate(5, node).Y != 0) moveBlock("URRD", node);
        }
    }
    //end 5

    //move 13
    if (node.emptySlot.Y == 0) moveBlock("L", node);
    if (locate(13, node).X != 2) {
        target = {2, locate(13, node).Y};
        while (node.emptySlot.X != target.X)
            if (node.emptySlot.X < target.X) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != target.Y)
            if (node.emptySlot.Y < target.Y) moveBlock("L", node);
            else moveBlock("R", node);
        if (locate(13, node).X != 2)
            if (node.emptySlot.X > locate(13, node).X) moveBlock("D", node);
            else moveBlock("U", node);
    }

    if (locate(13, node).Y != 0) {
        if (node.emptySlot.Y == locate(13, node).Y)
            if (node.emptySlot.Y != 3) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != locate(13, node).X)
            if (node.emptySlot.X < locate(13, node).X) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != locate(13, node).Y-1)
            if (node.emptySlot.Y < locate(13, node).Y-1) moveBlock("L", node);
            else moveBlock("R", node);
        while (locate(13, node).Y != 0) {
            moveBlock("L", node);
            if (locate(13, node).Y != 0) moveBlock("URRD", node);
        }
    }
    //end 13

    //move 9
    if (node.emptySlot.Y == 0) moveBlock("L", node);
    if (node.table[3][0] == 9) {
        if (node.emptySlot.X != 3) moveBlock("U", node);
        moveBlock("RDLLURDLURDRULDL", node);
    }

    if (locate(9, node).X != 2) {
        target = {2, locate(9, node).Y};
        while (node.emptySlot.X != target.X)
            if (node.emptySlot.X < target.X) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != target.Y)
            if (node.emptySlot.Y < target.Y) moveBlock("L", node);
            else moveBlock("R", node);
        if (locate(9, node).X != 2)
            if (node.emptySlot.X > locate(9, node).X) moveBlock("D", node);
            else moveBlock("U", node);
    }

    if (locate(9, node).Y != 1) {
        if (node.emptySlot.Y == locate(9, node).Y)
            if (node.emptySlot.Y != 3) moveBlock("L", node);
            else moveBlock("R", node);
        while (node.emptySlot.X != locate(9, node).X)
            if (node.emptySlot.X < locate(9, node).X) moveBlock("U", node);
            else moveBlock("D", node);
        while (node.emptySlot.Y != locate(9, node).Y-1)
            if (node.emptySlot.Y < locate(9, node).Y-1) moveBlock("L", node);
            else moveBlock("R", node);
        while (locate(9, node).Y != 1) {
            moveBlock("L", node);
            if (locate(9, node).Y != 1) moveBlock("URRD", node);
        }
    }
    //end 9

    //finish first column
    if (node.emptySlot.Y == 1) moveBlock("L", node);
    while (node.emptySlot.X != 3) moveBlock("U", node);
    while (node.emptySlot.Y != 0) moveBlock("R", node);
    moveBlock("DL", node);
    //
}

