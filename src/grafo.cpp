#include "grafo.h"
#include "unionFind.h"
#include <iostream>
Grafo::Grafo(int n){
    tamanho = n;
    adjacencia.resize(tamanho);
}

int Grafo::get_tamanho(){
  return tamanho;
}
vector<array<int,3>> Grafo::get_arestas(){
  return arestas;
}
void Grafo::adicionar_aresta(int u, int v, int peso){
    adjacencia[u].emplace_back(v, peso);
    arestas.push_back({u, v, peso});
}

vector<int> Grafo::encontrar_sccs(){
    vector<vector<int>> inv_adjacencia(tamanho);
    for(int vertice = 0; vertice < tamanho; vertice++){
        for(auto [outro, peso] : adjacencia[vertice]){
            inv_adjacencia[outro].push_back(vertice);
        }
    }
    stack<int> pilha;
    vector<bool> visitado(tamanho, false);
    function <void(int)> dfs = [&](int vertice){
        visitado[vertice] = true;
        for(auto [outro, peso] : adjacencia[vertice]){
            if(!visitado[outro]){
                dfs(outro);
            }
        }
        pilha.push(vertice);
    };
    vector<int> componente(tamanho);
    function<void(int, int)> scc = [&](int vertice, int c){
        visitado[vertice] = true;
        componente[vertice] = c;
        for(auto outro : inv_adjacencia[vertice]){
            if(!visitado[outro]){
                scc(outro, c);
            }
        }
    };
    for(int vertice = 0; vertice < tamanho; vertice++){
        if(!visitado[vertice]){
            dfs(vertice);
        }
    }
    fill(visitado.begin(), visitado.end(), false);
    while(pilha.size()){
        int vertice = pilha.top();
        pilha.pop();
        if(!visitado[vertice]){
            scc(vertice, vertice);
        }
    }
    return componente;
}

pair<int,vector<array<int,3>>> Grafo::encontrar_arvore_geradora_minima(){
    UnionFind conjuntos(tamanho);
    auto comparador = [&](array<int,3> a, array<int,3> b){
        return a[2] < b[2];
    };
    sort(arestas.begin(), arestas.end(), comparador);
    int custo = 0;
    vector<array<int,3>> arestas_mst;
    for(auto aresta : arestas){
        int u = aresta[0], v = aresta[1], peso = aresta[2];
        if(conjuntos.juntar(u, v)){
            arestas_mst.push_back({u, v, peso});
            custo += peso;
        }
    }
    return make_pair(custo, arestas_mst);
}

pair<int,vector<pair<int,int>>> Grafo::encontrar_caminho_minimo(int inicio, int fim){
    const int inf = 1e9;
    vector<int> distancia(tamanho, inf);
    distancia[inicio] = 0;
    set<pair<int,int>> s;
    for(int i = 0; i < tamanho; i++){
        s.insert({distancia[i], i});
    }
    vector<int> prev(tamanho, -1);
    while(s.size() > 0){
        auto [d, vertice] = *s.begin();
        s.erase(s.begin());
        for(auto [outro, peso] : adjacencia[vertice]){
            if(d + peso < distancia[outro]){
                prev[outro] = vertice;
                s.erase({distancia[outro], outro});
                distancia[outro] = d+peso;
                s.insert({distancia[outro], outro});
            }
        }
    }
    vector<pair<int,int>> arestas_caminho_minimo;
    if(distancia[fim] == inf){
        return make_pair(inf, arestas_caminho_minimo);
    }
    vector <int> caminho = {fim};
    while(prev[caminho.back()] != -1){
        caminho.push_back(prev[caminho.back()]);
    }
    reverse(caminho.begin(), caminho.end());
    for(int i = 1; i < (int)caminho.size(); i++){
        arestas_caminho_minimo.emplace_back(caminho[i-1], caminho[i]);
    }
    return make_pair(distancia[fim], arestas_caminho_minimo);
}