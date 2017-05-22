#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;
#define INF 1234
//#define endl "\n"


int moon[9][9] = {
                    {-INF,    1,    2,   -1,    4,    1,    4,    2,   -2},
                    {  -2,    4,    1,    0,   -1,   -1,    0,    1,    2},
                    {   1,    2,   -1,    1, -INF,    4,    2,   -1,    1},
                    {   0,    2,    0,   -2,    2,    1, -INF,    2,   -1},
                    {  -2,    1,    4,    1,    0,   -1,    4,    1,   -2},
                    {  -1, -INF,    1,   -1,    0,    1,    2,   -2,    0},
                    {   2,    4,   -2,    1,    0,    2,   -1, -INF,    2},
                    {   1,    0,    2,    4,   -1,   -2,    1,    0,    1},
                    {   0,   -2,   -1,    1, -INF,    1,    2,   -1,    4}
};


bool isValid(int x, int y)
{
    if(x < 0 || x >= 9)
        return false;
    if(y < 0 || y >= 9)
        return false;

    if(moon[x][y] <= -INF)
        return false;

    return true;
}

// Eight possible moves from each cell
int dx[8]   = { -1,  -2,  -2,  -1,   1,   2,   2,   1};
int dy[8]   = { -2,  -1,   1,   2,   2,   1,  -1,  -2};


// ph - path h - move horizontally and then vertically
// ph1(x,y) - first cell in the path
// ph2(x,y) - second cell in the path
// destination cell will be (dx,dy)
int ph1x[8]  = {  0,   0,   0,   0,   0,   0,   0,   0};
int ph1y[8]  = { -1,  -1,   1,   1,   1,   1,  -1,  -1};

int ph2x[8]  = {  0,  -1,  -1,   0,   0,   1,   1,   0};
int ph2y[8]  = { -2,  -1,   1,   2,   2,   1,  -1,  -2};

// pv - path v - move vertically and then horizontally
int pv1x[8]  = { -1,  -1,  -1,  -1,   1,   1,   1,   1};
int pv1y[8]  = {  0,   0,   0,   0,   0,   0,   0,   0};

int pv2x[8]  = { -1,  -2,  -2,  -1,   1,   2,   2,   1};
int pv2y[8]  = { -1,   0,   0,   1,   1,   0,   0,  -1};


// cost/prize of moving from cell (x,y) in one of the eight directions
int pathCost(int x, int y, int dir)
{
    int cost = -INF;
    if(isValid(x + ph1x[dir], y + ph1y[dir]) && isValid(x + ph2x[dir], y + ph2y[dir]) && isValid(x + dx[dir], y + dy[dir])) {
        int horizontalCost = moon[x + ph1x[dir]][y + ph1y[dir]] + moon[x + ph2x[dir]][y + ph2y[dir]] + moon[x + dx[dir]][y + dy[dir]];
        cost = max(cost, horizontalCost);
    }

    if(isValid(x + pv1x[dir], y + pv1y[dir]) && isValid(x + pv2x[dir], y + pv2y[dir]) && isValid(x + dx[dir], y + dy[dir])) {
        int verticalCost = moon[x + pv1x[dir]][y + pv1y[dir]] + moon[x + pv2x[dir]][y + pv2y[dir]] + moon[x + dx[dir]][y + dy[dir]];
        cost = max(cost, verticalCost);
    }
    
    return cost;
}


bool visited[9][9];
int cost[9][9][8];
int validNeighbours[9][9];
vector< pair<int, int> > path;
int prize;
int maxPrize;

void initializeGraph()
{
    for(int x = 0; x < 9; x++)
        for(int y = 0; y < 9; y++)
            visited[x][y] = false;

    visited[0][0] = visited[2][4] = visited[3][6] = visited[5][1] = visited[6][7] = visited[8][4] = true;

    for(int x = 0; x < 9; x++) {
        for(int y = 0; y < 9; y++) {
            if(!isValid(x, y))
                continue;
            for(int dir = 0; dir < 8; dir++) {
                cost[x][y][dir] = pathCost(x, y, dir);
                if(cost[x][y][dir] > -INF)
                    validNeighbours[x][y]++;
            }
        }
    }


    prize = maxPrize = 0;
}


void printGraph()
{
    for(int x = 0; x < 9; x++)
        for(int y = 0; y < 9; y++) {
            cout << x << ',' << y << '\t';
            for(int dir = 0; dir < 8; dir++)
                cout << cost[x][y][dir] << '\t';
            cout << endl;
        }

}

string pathHorizontalFirst[8] = {"W,W,N", "W,N,N", "E,N,N", "E,E,N", "E,E,S", "E,S,S", "W,S,S", "W,W,S"};
string pathVerticalFirst[8] = {"N,W,W", "N,N,W", "N,N,E", "N,E,E", "S,E,E", "S,S,E", "S,S,W", "S,W,W"};

bool isHorizontalBetter(int x, int y, int dir)
{
    int cost = -INF;
    if(isValid(x + ph1x[dir], y + ph1y[dir]) && isValid(x + ph2x[dir], y + ph2y[dir]) && isValid(x + dx[dir], y + dy[dir])) {
        int horizontalCost = moon[x + ph1x[dir]][y + ph1y[dir]] + moon[x + ph2x[dir]][y + ph2y[dir]] + moon[x + dx[dir]][y + dy[dir]];
        if(horizontalCost <= -INF)
            return false;
        cost = max(cost, horizontalCost);
    }

    if(isValid(x + pv1x[dir], y + pv1y[dir]) && isValid(x + pv2x[dir], y + pv2y[dir]) && isValid(x + dx[dir], y + dy[dir])) {
        int verticalCost = moon[x + pv1x[dir]][y + pv1y[dir]] + moon[x + pv2x[dir]][y + pv2y[dir]] + moon[x + dx[dir]][y + dy[dir]];
        if(verticalCost >= cost)
            return false;
        cost = max(cost, verticalCost);
    }

    return true;    
}
string printBestPath(int x, int y, int dir)
{
    if(isHorizontalBetter(x, y, dir))
        return pathHorizontalFirst[dir];
    else
        return pathVerticalFirst[dir];
}

void printSolution(int prize, bool visited[9][9], vector< pair<int, int> > path)
{
    cout << "reached destination" << endl;
    for(int x = 0; x < 9; x++) {
        for(int y = 0; y < 9; y++)
            cout << visited[x][y] << ' ';
        cout << endl;
    }

    cout << "Prize " << prize << " depth " << path.size() << ' ' << endl;
    for(int i = 0; i < path.size(); i++)
        cout << '(' << path[i].first << ',' << path[i].second << ')' << ',';
    cout << endl << endl;

    cout << "Final solution is ";
    int curX = 4, curY = 4;
    for(int i = 0; i < path.size(); i++) {
        for(int dir = 0; dir < 8; dir++) {
            int newX = curX + dx[dir];
            int newY = curY + dy[dir];
            if(isValid(newX, newY) && newX == path[i].first && newY == path[i].second) {
                cout << printBestPath(path[i].first, path[i].second, dir) << " | ";
                break;
            }
        }
        curX = path[i].first;
        curY = path[i].second;
    }
    cout << endl << endl;


}

int dirOrder[8] = {1, 7, 3, 2, 4, 6, 5, 0};

inline bool comparePairBySecond(pair<int, int> a, pair<int, int> b)
{
    return a.second > b.second;
}

inline bool comparePairBySecondMin(pair<int, int> a, pair<int, int> b)
{
    return a.second < b.second;
}

int RANDOM_DEPTH;

void PATH(int x, int y, int prize, bool visited[9][9], vector< pair<int, int> > path)
{
    
    int depth = path.size();

    vector< pair<int, int> > p;
    if(depth > 6 && depth < RANDOM_DEPTH) {
        srand(time(0));
        random_shuffle(dirOrder, dirOrder + 8);
    }

    else {
        for(int d = 0; d < 8; d++) {
            int newX = x + dx[d];
            int newY = y + dy[d];
            int vn = validNeighbours[newX][newY];
            p.push_back(make_pair(d, vn));
        }
        
        // sort by least neighbours and then left rotate 
        sort(p.begin(), p.end(), comparePairBySecondMin);

        int numDir = 0;
//        if(depth < 50 && p[0].second < 1 && numDir < 6) {
//            rotate(p.begin(), p.begin() + 1, p.end());
//            numDir++;
//        }
        if(depth < 60)
        for(int d = 0; d < 8; d++) {
            int newX = x + dx[d];
            int newY = y + dy[d];
            int vn = validNeighbours[newX][newY];
            int pc = cost[x][y][d];

            int score;
            if(vn == 0)
                score = -(INF << 12);
            else 
                score = (pc << 12) / vn;

            p[d] = make_pair(d, score);
        }

        
        // visit cells with max score first
        sort(p.begin(), p.end(), comparePairBySecond);
    }

    

    for(int d = 0; d < 8; d++) {
        int dir;
        if(depth > 8 && depth < RANDOM_DEPTH)
            dir = dirOrder[d];
        else
            dir = p[d].first;
        
        int newX, newY;
        newX = x + dx[dir];
        newY = y + dy[dir];

        if(!isValid(newX, newY))
            continue;
        if(cost[x][y][dir] <= -INF)
            continue;
  
        if(visited[newX][newY])
            continue;

        if(newX == 4 && newY == 4 && cost[x][y][dir] > -INF) {
            path.push_back(make_pair(newX, newY));
            prize += cost[x][y][dir];
            maxPrize = max(prize, maxPrize);
            if(prize >= maxPrize)
                printSolution(prize, visited, path);
//            else if(depth > 68)
//                cout << "prize " << prize << " depth " << depth << endl;
            prize -= cost[x][y][dir];
            path.pop_back();
            visited[4][4] = false;
            continue;
        }

        visited[newX][newY] = true;
        path.push_back(make_pair(newX, newY));
        prize += cost[x][y][dir];

        for(int newDir = 0; newDir < 8; newDir++) {
            int newNewX, newNewY;
            newNewX = newX + dx[newDir];
            newNewY = newY + dy[newDir];

            if(!isValid(newNewX, newNewY))
                continue;
    
            if(cost[newX][newY][newDir] <= -INF)
                continue;

            validNeighbours[newNewX][newNewY]--;
            if(validNeighbours[newNewX][newNewY] < 0)
                validNeighbours[newNewX][newNewY] = 0;
        }

        PATH(newX, newY, prize, visited, path);

        visited[newX][newY] = false;
        path.pop_back();
        prize -= cost[x][y][dir];

        for(int newDir = 0; newDir < 8; newDir++) {
            int newNewX, newNewY;
            newNewX = newX + dx[newDir];
            newNewY = newY + dy[newDir];

            if(!isValid(newNewX, newNewY))
                continue;
    
            if(cost[newX][newY][newDir] <= -INF)
                continue;

            validNeighbours[newNewX][newNewY]++;
        }
    }
}


int main()
{
    initializeGraph();
    printGraph();
    RANDOM_DEPTH = 4;

    //PATH(4, 4, prize, visited, path);

    for(int dir = 4; dir < 8; dir++) {
        int newX, newY;
        newX = 4 + dx[dir];
        newY = 4 + dy[dir];

        visited[newX][newY] = true;
        path.push_back(make_pair(newX, newY));
        prize += cost[4][4][dir];

        for(int dir = 4; dir < 8; dir++) {
            int newNewX, newNewY;
            newNewX = newX + dx[dir];
            newNewY = newY + dy[dir];

            visited[newNewX][newNewY] = true;
            path.push_back(make_pair(newNewX, newNewY));
            prize += cost[newX][newY][dir];

            PATH(newNewX, newNewY, prize, visited, path);
        }
    }
    return 0;
}

