//
// Created by Killua on 2017/10/25.
//

#ifndef UNTITLED_MAZE_H
#define UNTITLED_MAZE_H

#include <vector>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <iostream>


using namespace std;
//迷宫的小格子
struct Grid {
    vector<int> adjGrids;
    int root;
};

class Maze {
public:

    Maze(int n, int st, int en) : size(n), start(st), end(en) {
        maze.reserve(size * size);
        for (int i = 0; i < size * size; ++i) {
            Grid g;
            g.root = i;
            noUnicomIndex.push_back(i);
            maze.push_back(g);
        }

        maze[start].root = -1;
        visited.insert(start);
    }

    ~Maze() {}

    void generateMaze() {
        while (visited.size() < maze.size()) {
            int index1 = getRanGridInx();
            int index2 = getDirInx(index1);

            if (index2 == -1) {
                continue;
            }

            //相当于打通一堵墙
            maze[index1].adjGrids.push_back(index2);
            maze[index2].adjGrids.push_back(index1);
            unionGrid(index1, index2);
        }
    }

    //打印迷宫
    void printMaze() {
        int xLine[5][5] = {0};
        int yLine[5][5] = {0};
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int rootIndex = i * 5 + j;
                auto adjs = maze[rootIndex].adjGrids;
                for (auto index : adjs) {
                    if (index - rootIndex == 5) {
                        xLine[index / 5 - 1][index % 5] = 1;
                    }
                    if (index - rootIndex == 1) {
                        yLine[index / 5][index % 5 - 1] = 1;
                    }
                }
            }
        }

        cout << "    -- -- -- --" << endl;
        for (int i = 0; i < size; i++) {
            cout << "|";
            for (int j = 0; j < size; j++) {
                if (i <= 1)
                    cout << " ";
                if (yLine[i][j] != 1) {

                    cout << i * 5 + j << "|";
                } else {
                    cout << i * 5 + j << " ";
                }
            }
            cout << endl;
            if (i == size - 1) {
                cout << " -- -- -- --" << endl;
                break;
            }

            for (int j = 0; j < size; j++) {
                cout << " ";
                if (xLine[i][j] != 1) {
                    cout << "--";
                } else {
                    cout << "  ";
                }
            }
            cout << endl;
        }

    }

private:

    set<int> visited;
    const int size = 5;
    const int start = 0;
    const int end = 0;
    //格子周围
    const int transform[4][2] = {{-1, 0},
                                 {1,  0},
                                 {0,  -1},
                                 {0,  1}};

    vector<Grid> maze;
    vector<int> noUnicomIndex;//减少随机次数

    //随机得到一个格子
    int getRanGridInx() {
        srand((unsigned) time(NULL));

        return noUnicomIndex[rand() % noUnicomIndex.size()];
    }

    //随机格子上的方向
    int getDirInx(int gridIndex) {
        int dirIndex;
        int x = gridIndex % size;
        int y = gridIndex / size;

        vector<int> dirVecs;

        for (int i = 0; i < 4; ++i) {
            int xx = x + transform[i][0];
            int yy = y + transform[i][1];
            if (xx >= 0 && xx < size && yy >= 0 && yy < size) {
                dirIndex = getIndex(xx, yy);

                if (maze[dirIndex].root != maze[gridIndex].root) {
                    dirVecs.push_back(dirIndex);
                }
            }
        }

        //邻接格子都已经相通
        if (dirVecs.empty()) {
            auto itr = noUnicomIndex.begin();
            for (; itr != noUnicomIndex.end(); ++itr) {
                if (*itr == gridIndex) {
                    noUnicomIndex.erase(itr);
                    break;
                }
            }
            return -1;
        }

        srand((unsigned) time(nullptr));

        dirIndex = dirVecs[rand() % dirVecs.size()];

        return dirIndex;
    }

    //得到vector下标
    int getIndex(int x, int y) {
        return y * 5 + x;
    }

    void unionGrid(int index1, int index2) {
        int root1 = maze[index1].root;
        int root2 = maze[index2].root;

        if (root1 < root2) {
            changeRoot(root1, root2, index1);
        } else {
            changeRoot(root2, root1, index2);
        }
    }


    void changeRoot(int root1, int root2, int father) {

        maze[root2].root = root1;
        if (root1 == -1) {
            visited.insert(root2);
        }

        for (int i : maze[root2].adjGrids) {
            if (i != father) {
                changeRoot(root1, i, root2);
            }
        }
    }

};


#endif //UNTITLED_MAZE_H
