import numpy as np

def generar_grafo_trivial(n):
    """Genera grafo de n vertices aislados en forma de matriz de adyacencias"""
    return [[0]*n for i in range(0, n)]


def agregar_grafo_completo(matriz, n=-1, inicio_index=0):
    """Agrega a la matriz un grafo completo cuyos nodos son [inicio_index, inicio_index+1,..., inicio_index+n-1]"""
    if n == -1:
        n = len(matriz)
    
    for i in range(0, n):
        for j in range(0, n):
            if i == j: continue
            matriz[inicio_index + i][inicio_index + j] = 1
    return matriz

def agregar_grafo_estrella_hacia_adentro(matriz, n=-1, inicio_index=0):
    """Agrega a la matriz un grafo estrella dirigido cuyos nodos son [inicio_index, inicio_index+1,..., inicio_index+n-1],
    todos apuntando al nodo 'inicio_index'
    """
    if n == -1:
        n = len(matriz)

    for i in range(1, n):
        matriz[inicio_index + i][inicio_index] = 1
    
    return matriz
        
def agregar_grafo_estrella_hacia_afuera(matriz, n=-1, inicio_index=0):
    """Agrega a la matriz un grafo estrella dirigido cuyos nodos son [inicio_index, inicio_index+1,..., inicio_index+n-1],
    donde el nodo 'inicio_index' apunta al resto de los nodos.
    """
    if n == -1:
        n = len(matriz)

    for i in range(1, n):
        matriz[inicio_index][inicio_index + i] = 1

    return matriz

def generar_grafo_union(matrices):
    """Genera un nuevo grafo que es la union de todos los pasados por param."""

    # calculo cantidad total de nodos
    n = 0
    for matriz in matrices:
        n += len(matriz)
    
    union = generar_grafo_trivial(n)
    
    # agrego ejes de cada grafo indexados adecuadamente
    inicio_index = 0
    for matriz in matrices:
        for i in range(0, len(matriz)):
            for j in range(0, len(matriz)):
                union[inicio_index + i][inicio_index + j] = matriz[i][j]
        inicio_index += len(matriz)
    return union        

def generar_grafo_estrellas_unidas_hacia_adentro(cant_estrellas, cant_nodos_por_estrella):
    """ Genera 'cant_estrellas' grafos estrellas hacia adentro, de 'cant_nodos_por_estrella' cada uno y
    agrega un nuevo eje desde el centro de cada estrella hacia un nuevo nodo central
    """
    n = cant_estrellas * cant_nodos_por_estrella + 1
    m = generar_grafo_trivial(n)

    # agrego grafos estrella
    inicio_index = 0
    for i in range(0, cant_estrellas):
        agregar_grafo_estrella_hacia_adentro(m, cant_nodos_por_estrella, inicio_index)
        inicio_index += cant_nodos_por_estrella
    
    # uno los centros {0, cant_nodos_por_estrella, 2*cant_nodos_por_estrella,...} al ultimo nodo
    for i in range(0, n-1, cant_nodos_por_estrella):
        m[i][n-1] = 1
    
    return m   

def generar_grafo_estrellas_unidas_hacia_afuera(cant_estrellas, cant_nodos_por_estrella):
    """ Genera 'cant_estrellas' grafos estrellas hacia adentro, de 'cant_nodos_por_estrella' cada uno y
    agrega un nuevo eje desde el centro de cada estrella hacia un nuevo nodo central
    """

    n = cant_estrellas * cant_nodos_por_estrella + 1
    m = generar_grafo_trivial(n)

    # agrego grafos estrella
    inicio_index = 0
    for i in range(0, cant_estrellas):
        agregar_grafo_estrella_hacia_afuera(m, cant_nodos_por_estrella, inicio_index)
        inicio_index += cant_nodos_por_estrella
    
    # uno los centros 0, cant_nodos_por_estrella, 2*cant_nodos_por_estrella,etc al ultimo nodo
    for i in range(0, n-1, cant_nodos_por_estrella):
        m[n-1][i] = 1
    
    return m   


def generar_grafo_random(cant_nodos, p_densidad):
    """ Genera un grafo random de 'cant_nodos' nodos con densidad de aristas 'p_densidad' (que es una probabilidad)"""
    max_m = int(cant_nodos*(cant_nodos-1))
    m = int(max_m*p_densidad)
    np_matrix = np.array([0] * (max_m - m) + [1] * m)
    np.random.shuffle(np_matrix)
    np_matrix = np.split(np_matrix, cant_nodos)
    for i in range(0, cant_nodos):
        np_matrix[i] = np.insert(np_matrix[i],i,0)

    return np_matrix