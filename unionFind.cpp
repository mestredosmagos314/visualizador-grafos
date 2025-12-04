#include "unionFind.h"

UnionFind::UnionFind(int n){
    tamanho = n;
    pai.resize(tamanho);
    tamanho_comp.resize(tamanho);
    for(int i = 0; i < tamanho; i++){
        pai[i] = i;
        tamanho_comp[i] = 1;
    }
}

int UnionFind::achar(int vertice){
    if(pai[vertice] == vertice){
        return vertice;
    }
    return pai[vertice] = achar(pai[vertice]);
}

bool UnionFind::juntar(int a, int b){
    a = achar(a), b = achar(b);
    if(a == b) return false;
    if(tamanho_comp[a] < tamanho_comp[b]) swap(a,b);
    pai[b] = a;
    tamanho_comp[a] += tamanho_comp[b];
    return true;
}