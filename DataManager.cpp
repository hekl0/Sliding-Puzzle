#include <main.h>

void DataManager::addItem(int mode, string name, int score) {
    bool isEmpty = true;
    for (int i = 0; i < name.length(); i++)
        if (name[i] != ' ') isEmpty = false;
    if (isEmpty) name += ".";

    vector< pair<string, int> > data = getAllItem(mode);
    data.push_back(make_pair(name, score));

    for (int i = 0; i < data.size(); i++)
        for (int j = i+1; j < data.size(); j++)
            if (data[i].second > data[j].second)
                swap(data[i], data[j]);

    ofstream file;
    if (mode == 0) file.open("Data/easy.txt", ios::out);
    if (mode == 1) file.open("Data/medium.txt", ios::out);
    if (mode == 2) file.open("Data/hard.txt", ios::out);

    for (int i = 0; i < min((int)data.size(), 3); i++) {
        file << data[i].first << endl;
        file << data[i].second << endl;
    }

    file.close();
}

vector< pair<string, int> > DataManager::getAllItem(int mode) {
    vector< pair<string, int> > res;

    ifstream file;
    if (mode == 0) file.open("Data/easy.txt", ios::in);
    if (mode == 1) file.open("Data/medium.txt", ios::in);
    if (mode == 2) file.open("Data/hard.txt", ios::in);

    string name;
    int score;
    while (file >> name && file >> score) {
        pair<string, int> data = make_pair(name, score);
        res.push_back(data);
    }

    file.close();

    return res;
}

bool DataManager::haveLastGame() {
    bool res;

    ifstream file;
    file.open("Data/last_game.txt", ios::in);

    file >> res;

    file.close();

    return res;
}

string DataManager::dataLastGame(GAME& gameValue) {
    bool havingLastGame;
    int sizeTable;
    string res;

    ifstream file;
    file.open("Data/last_game.txt", ios::in);
    //check if having last game
    file >> havingLastGame;

    //get table size
    file >> sizeTable;
    gameValue.width = sizeTable;
    gameValue.height = sizeTable;

    //get table value
    for (int i = 0; i < sizeTable; i++)
        for (int j = 0; j < sizeTable; j++) {
            file >> gameValue.table[i][j];
            if (gameValue.table[i][j] == 0)
                gameValue.pos0 = {i, j};
        }

    //get score
    file >> gameValue.score;

    //get image source
    file >> res;

    file.close();

    return res;
}

void DataManager::saveLastGame(bool isUnfinish, GAME gameValue, string src) {
    cout << src << endl;
    ofstream file;
    file.open("Data/last_game.txt", ios::out);

    file << isUnfinish << endl;
    file << gameValue.height << endl;
    for (int i = 0; i < gameValue.height; i++)
        for (int j = 0; j < gameValue.width; j++)
            file << gameValue.table[i][j] << " ";
    file << endl;
    file << gameValue.score << endl;
    file << src << endl;

    file.close();
}
