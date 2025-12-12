#ifndef __GRAFO_H__
#define __GRAFO_H__

using namespace std;

#include <vector>
#include <array>
#include <functional>
#include <stack>
#include <algorithm>
#include <set>
#include <cassert>

class Grafo{
    public:

    Grafo(int);
    void adicionar_aresta(int, int, int);
    vector<int> encontrar_sccs();
    pair<int,vector<pair<int,int>>> encontrar_caminho_minimo(int, int);
    pair<int,vector<array<int,3>>> encontrar_arvore_geradora_minima();
    int get_tamanho();
    vector<array<int,3>> get_arestas();

    private:

    int tamanho;
    vector<vector<pair<int,int>>> adjacencia;
    vector<array<int,3>> arestas;
};

#endif