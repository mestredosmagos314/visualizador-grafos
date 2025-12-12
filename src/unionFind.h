#ifndef __UNIONFIND_H__
#define __UNIONFIND_H__

#include <algorithm>
#include <vector>

using namespace std;

class UnionFind{
    public:

    int tamanho;
    UnionFind(int);
    int achar(int);
    bool juntar(int, int);

    public:

    vector<int> pai, tamanho_comp;
};

#endif