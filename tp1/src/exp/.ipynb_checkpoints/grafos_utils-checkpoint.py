import networkx as nx
import matplotlib.pyplot as plt


def imprimir_matriz(matriz):
    n = len(matriz)
    for i in range(0, n):
        fila = ""
        for j in range(0, n):
            fila = fila + str(matriz[i][j]) + ' '
        print(fila)

def imprimir(g, file = None):
    """Imprime el grafo utilizando NetworkX"""
    plt.clf()
    fig = plt.figure(figsize=(20,20))
    nx.draw_kamada_kawai(a_network(g), with_labels=True)
    if file is None:
        fig.show()
    else:
        fig.savefig(file)

""" Guardar el grafo en formato graphml, para abrir con Cytoscape
"""
def guardar_graphml(g, file):
    nx.write_graphml(a_network(g), file)
        
def guardar_matriz_en_archivo(matriz, nombre_archivo, imprimir_escritura=False):
    archivo = open(nombre_archivo, 'w')
    
    # guardo cant. nodos
    n = len(matriz)
    archivo.write(str(n) + '\n')
    if imprimir_escritura:
        print(str(n))
    
    # guardo cant. aristas
    m = 0
    for i in range(0, n):
        for j in range (0, n):
            if matriz[i][j] == 1:
                m+=1
    archivo.write(str(m) + '\n')
    if imprimir_escritura:
        print(str(m))
                
    # guardo aristas
    for i in range(0, n):
        for j in range (0, n):
            if matriz[i][j] == 1:
                archivo.write("{} {}\n".format(i+1,j+1))
                if imprimir_escritura:
                    print("{} {}".format(i+1,j+1))
                
    archivo.close()
    
def a_network(matriz):
    g = nx.DiGraph()
    g.add_nodes_from(range(len(matriz)))
    for i in range(len(matriz)):
        for j in range(len(matriz)):
            if matriz[i][j]:
                g.add_edge(i, j)
    return g
    