#include <iostream>
#include <limits>
#include <vector>
#include <random>
#include <ctime>
#include <map>
#include <string>
#include <set>

#include "grafo.h"
#include "ponte.h"

using namespace std;

void limpar_buffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int ler_inteiro(string mensagem, int min = numeric_limits<int>::min(), int max = numeric_limits<int>::max()) {
    int valor;
    while (true) {
        cout << mensagem;
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                return valor;
            } else {
                cout << "Erro: O valor deve estar entre " << min << " e " << max << ".\n";
            }
        } else {
            cout << "Erro: Entrada invalida. Por favor, digite um numero inteiro.\n";
            limpar_buffer();
        }
    }
}

bool ler_sim_nao(string mensagem) {
    char resposta;
    while (true) {
        cout << mensagem << " (s/n): ";
        cin >> resposta;
        if (resposta == 's' || resposta == 'S') return true;
        if (resposta == 'n' || resposta == 'N') return false;
        
        cout << "Erro: Responda com 's' para sim ou 'n' para nao.\n";
        limpar_buffer();
    }
}

void criar_grafo_manual(Grafo*& grafo, bool& direcionado, int& n) {
    if (grafo != nullptr) delete grafo;

    n = ler_inteiro("Digite o numero de vertices do grafo: ", 1, 100000);
    direcionado = ler_sim_nao("O grafo eh direcionado?");

    grafo = new Grafo(n);

    int num_arestas = ler_inteiro("Digite o numero de arestas: ", 0, n * (n - 1));

    cout << "Digite as arestas no formato: Origem Destino Peso\n";
    cout << "(Vertices devem ser entre 0 e " << n - 1 << ")\n";

    for (int i = 0; i < num_arestas; i++) {
        cout << "Aresta " << i + 1 << ":\n";
        int u = ler_inteiro("  Origem: ", 0, n - 1);
        int v = ler_inteiro("  Destino: ", 0, n - 1);
        int peso = ler_inteiro("  Peso: ", -10000, 10000);

        grafo->adicionar_aresta(u, v, peso);
        if (!direcionado) {
            grafo->adicionar_aresta(v, u, peso);
        }
    }
    cout << "Grafo criado com sucesso!\n";
}

void criar_grafo_aleatorio(Grafo*& grafo, bool& direcionado, int& n) {
    if (grafo != nullptr) delete grafo;

    n = ler_inteiro("Digite o numero de vertices: ", 1, 10);
    direcionado = ler_sim_nao("O grafo eh direcionado?");
    int max_arestas = direcionado ? n * (n - 1) : (n * (n - 1)) / 2;
    int num_arestas = ler_inteiro("Digite o numero de arestas (Max " + to_string(max_arestas) + "): ", 0, max_arestas);
    int peso_min = ler_inteiro("Peso minimo: ");
    int peso_max = ler_inteiro("Peso maximo: ", peso_min);

    grafo = new Grafo(n);

    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> dist_vertice(0, n - 1);
    uniform_int_distribution<int> dist_peso(peso_min, peso_max);

    int arestas_criadas = 0;
    set<pair<int,int>> arestas;
    while (arestas_criadas < num_arestas) {
        int u = dist_vertice(rng);
        int v = dist_vertice(rng);
        if(arestas.find({u, v}) != arestas.end()) continue;
        if (u == v) continue;

        int p = dist_peso(rng);
        grafo->adicionar_aresta(u, v, p);
        arestas.insert({u, v});
        if (!direcionado) {
            arestas.insert({v, u});
            grafo->adicionar_aresta(v, u, p);
        }
        arestas_criadas++;
    }
    cout << "Grafo aleatorio gerado com sucesso!\n";
}

void visualizar_grafo_geral(Grafo& g, PyObject* pModule, bool direcionado) {
    int n = g.get_tamanho();
    vector<array<int, 3>> todas_arestas = g.get_arestas();

    chamar_plotar_grafo(pModule, n, todas_arestas, direcionado);
}

void visualizar_caminho_minimo(Grafo& g, PyObject* pModule, int origem, int destino, bool direcionado) {
    pair<int, vector<pair<int, int>>> resultado = g.encontrar_caminho_minimo(origem, destino);
    if(resultado.first >= 1e9){
      cout << "Nao existe nenhum caminho entre esses dois vertices\n";
      return;
    }
    cout << "O custo do caminho minimo entre esses vertices e " << resultado.first << "\n";
    cout << "Desenhando o caminho minimo.\n";
    vector<pair<int, int>> arestas_caminho = resultado.second;

    set<pair<int, int>> set_caminho(arestas_caminho.begin(), arestas_caminho.end());

    vector<array<int,3>> todas_arestas = g.get_arestas(); 
    int n = g.get_tamanho();
  
    vector<array<int, 4>> dados_visualizacao;

    for (auto& a : todas_arestas) {
        int u = a[0];
        int v = a[1];
        int w = a[2];
        
        int cor = 0; // Cinza default
        if (set_caminho.count({u, v}) || set_caminho.count({v, u})) {
          cor = 1; // Vermelho (Caminho)
        }
        dados_visualizacao.push_back({u, v, w, cor});
    }

    chamar_plotar_arestas(pModule, n, dados_visualizacao, direcionado);
}

void visualizar_arvore_geradora(Grafo& g, PyObject* pModule, bool direcionado) {
    if(direcionado){
      cout << "O conceito de arvore geradora minima nao faz sentido para grafos direcionados. saindo da funcao.\n";
      return;
    }
    pair<int, vector<array<int,3>>> resultado = g.encontrar_arvore_geradora_minima();
    cout << "O custo da arvore geradora minima e: " << resultado.first << "\n";
    cout << "Desenhando a arvore geradora minima\n";
    vector<array<int,3>> mst = resultado.second;
    for(auto v : resultado.second){
      cout << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    set<pair<int, int>> set_mst;
    for(auto& a : mst) set_mst.insert({a[0], a[1]});

    vector<array<int,3>> todas_arestas = g.get_arestas(); 
    int n = g.get_tamanho();
    
    vector<array<int, 4>> dados_visualizacao;

    for (auto& a : todas_arestas) {
        int u = a[0];
        int v = a[1];
        int w = a[2];
        
        int cor = 0; // Cinza
        if ((set_mst.find({u, v}) != set_mst.end()) || (set_mst.find({v, u}) != set_mst.end())) {
            cor = 2; // Azul (MST) - Cor 2 definida no Python
        }
        dados_visualizacao.push_back({u, v, w, cor});
    }
    chamar_plotar_arestas(pModule, n, dados_visualizacao, false);
}

void visualizar_componentes_conexos(Grafo& g, PyObject* pModule, bool direcionado) {
    vector<int> cores_scc = g.encontrar_sccs(); 

    vector<array<int,3>> todas_arestas = g.get_arestas(); 
    int n = g.get_tamanho();
    
    chamar_plotar_vertices(pModule, n, cores_scc, todas_arestas, direcionado); // true = geralmente SCC é para direcionado
}


int main() {
    Grafo* meuGrafo = nullptr;
    bool direcionado = false;
    int numVertices = 0;
    PyObject* pModule = iniciar_modulo_python();
    if (!pModule) return 1;

    while (true) {
        cout << "\n================ MENU GRAFOS ================\n";
        cout << "1. Ler Grafo Manualmente\n";
        cout << "2. Criar Grafo Aleatorio\n";
        cout << "3. Mostrar o Grafo\n";
        cout << "4. Arvore Geradora Minima (MST) (Apenas Nao-Direcionado)\n";
        cout << "5. Componentes Fortemente Conexos (SCC)\n";
        cout << "6. Encontrar Caminho Minimo (Dijkstra)\n";
        cout << "7. Sair\n";
        cout << "=============================================\n";
        
        int escolha = ler_inteiro("Escolha uma opcao: ", 1, 7);

        switch (escolha) {
            case 1:
              criar_grafo_manual(meuGrafo, direcionado, numVertices);
              break;
            case 2:
              criar_grafo_aleatorio(meuGrafo, direcionado, numVertices);
              break;
            case 3:
              visualizar_grafo_geral(*meuGrafo, pModule, direcionado);
              break;
            case 4:
              visualizar_arvore_geradora(*meuGrafo, pModule, direcionado);
              break;
            case 5:
              visualizar_componentes_conexos(*meuGrafo, pModule, direcionado);
              break;
            case 6:
              {
              int inicio = ler_inteiro("Vertice inicial: ", 0, numVertices - 1);
              int fim = ler_inteiro("Vertice final: ", 0, numVertices - 1);
              visualizar_caminho_minimo(*meuGrafo, pModule, inicio, fim, direcionado);
              }
              break;
            case 7:
              if (meuGrafo != nullptr) delete meuGrafo;
              cout << "Saindo.\n";
              return 0;
        }
    }
}