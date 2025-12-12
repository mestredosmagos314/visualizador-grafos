import matplotlib
# Força o uso do backend TkAgg para abrir janelas
matplotlib.use('TkAgg') 

import networkx as nx
import matplotlib.pyplot as plt

CORES = ['lightgray', 'red', 'blue', 'green', 'orange', 'purple', 'cyan', 'magenta']

def _get_color(index):
    return CORES[index % len(CORES)]

def plotar_grafo(n, lista_arestas, direcionado):
    """
    lista_arestas: lista de tuplas [u, v, peso]
    """
    if direcionado:
        G = nx.DiGraph()
    else:
        G = nx.Graph()
        
    G.add_nodes_from(range(n))
    
    for u, v, w in lista_arestas:
        G.add_edge(u, v, weight=w)
        
    pos = nx.spring_layout(G, seed=42)
    
    plt.figure("Visualização do Grafo")
    nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500)
    
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    
    plt.show()

    
def plotar_grafo_arestas_coloridas(n, lista_arestas_com_cor, direcionado):
    """
    lista_arestas_com_cor: lista de tuplas [u, v, peso, cor_id]
    """
    if direcionado:
        G = nx.DiGraph()
    else:
        G = nx.Graph()
        
    G.add_nodes_from(range(n))
    
    for u, v, w, c in lista_arestas_com_cor:
        cor_hex = _get_color(c)
        G.add_edge(u, v, weight=w, color=cor_hex)
        
    pos = nx.spring_layout(G, seed=42)
    
    edges = G.edges()
    colors = [G[u][v]['color'] for u, v in edges]
    
    plt.figure("Grafo com Arestas Coloridas")
    
    nx.draw(G, pos, with_labels=True, node_color='lightblue', 
            edgelist=edges, edge_color=colors, width=2, node_size=500)
    
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    
    plt.show()

def plotar_grafo_vertices_coloridos(n, lista_cores_vertices, lista_arestas, direcionado):
    if direcionado:
        G = nx.DiGraph()
    else:
        G = nx.Graph()
        
    G.add_nodes_from(range(n))
    
    for u, v, w in lista_arestas:
        G.add_edge(u, v, weight=w)
        
    node_colors_mapped = [_get_color(c) for c in lista_cores_vertices]
    
    pos = nx.spring_layout(G, seed=42)
    
    plt.figure("Grafo com Vértices Coloridos")
    nx.draw(G, pos, with_labels=True, node_color=node_colors_mapped, node_size=500)
    
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    
    plt.show()