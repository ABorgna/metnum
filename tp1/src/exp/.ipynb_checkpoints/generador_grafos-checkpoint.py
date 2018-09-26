import numpy as np

def generar_grafo_trivial(n):
    """Genera grafo de n vertices aislados en forma de matriz de adyacencias"""
    return [[0]*n for i in range(0, n)]

""" Generar un grafo de estrellas anidadas.
    Nivel 1 es una estrella simple.
    Nivel n es una estrella de estrellas n-1;
    El nodo `hojas+1` es el centro
"""
def generar_grafo_estrella_anidado(grado, nivel, paraAdentro=True):
    if grado < 1:
        raise Exception("Grado tiene que ser >= 1")
        
    if nivel < 0:
        raise Exception("Nivel tiene que ser >= 0")
    elif nivel == 0:
        return generar_grafo_trivial(1)
    elif nivel == 1:
        grafo = generar_grafo_trivial(grado+1)
        size_hojas = 1
    elif nivel > 0:
        hojas = [generar_grafo_estrella_anidado(grado, nivel-1, paraAdentro)
                 for i in range(grado)]
        size_hojas = len(hojas[0])
        grafo = generar_grafo_union(hojas + [generar_grafo_trivial(1)])
        
    hojas = range(size_hojas-1, size_hojas * grado, size_hojas)
    raiz = len(grafo) - 1
        
    for hoja in hojas:
        if paraAdentro:
            grafo[hoja][raiz] = 1
        else:
            grafo[raiz][hoja] = 1
    return grafo

""" Generar un grafo estrella con `grado` hojas. El nodo `grado+1` es el centro
"""
def generar_grafo_estrella(grado, paraAdentro=True):
    return generar_grafo_estrella_anidado(grado, 1, paraAdentro)

def extender_grafo(matriz, n):
    """Extiende el grafo matriz para que tenga n nodos"""
    ng = len(matriz)
    n = max(n, ng)
    
    
    for i in range(ng, n):
        matriz.append([])
        for j in range(0, n):
            matriz[i].append(0)
    for i in range(0, ng):
        for j in range(ng, n):
            matriz[i].append(0)
    return matriz


def agregar_grafo_completo(matriz, n=-1, inicio_index=0):
    """Agrega a la matriz un grafo completo cuyos nodos son [inicio_index, inicio_index+1,..., inicio_index+n-1]"""
    if n == -1:
        n = len(matriz)
    matriz = extender_grafo(matriz, n + inicio_index)
    
    for i in range(0, n):
        for j in range(0, n):
            if i == j: continue
            matriz[inicio_index + i][inicio_index + j] = 1
    return matriz

def agregar_grafo_estrella_hacia_adentro(matriz, n=-1, inicio_index=0, objetivo=None):
    """Agrega a la matriz un grafo estrella dirigido cuyos nodos son [inicio_index, inicio_index+1,..., inicio_index+n-1],
    todos apuntando al nodo objetivo o 'inicio_index' si es None
    """
    if n == -1:
        n = len(matriz)
    matriz = extender_grafo(matriz, n + inicio_index)
    
    if objetivo is None:
        objetivo = inicio_index

    for i in range(1 if objetivo == inicio_index else 0, n):
        matriz[inicio_index + i][objetivo] = 1
    
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


def generar_grafo_random_de_densidad(cant_nodos, p_densidad):
    """ Genera un grafo random de 'cant_nodos' nodos con densidad de aristas 'p_densidad' (que es una probabilidad)"""
    max_m = int(cant_nodos*(cant_nodos-1))
    m = int(max_m*p_densidad)
    return generar_grafo_random_m_fijo(cant_nodos, m)

def generar_grafo_random_m_fijo(cant_nodos, cant_aristas):
    """ Genera un grafo random de 'cant_nodos' nodos con 'cant_aristas' aristas """
    max_m = int(cant_nodos*(cant_nodos-1))
    np_matrix = np.array([0] * (max_m - cant_aristas) + [1] * cant_aristas)
    np.random.shuffle(np_matrix)
    np_matrix = np.split(np_matrix, cant_nodos)
    for i in range(0, cant_nodos):
        np_matrix[i] = (np.insert(np_matrix[i],i,0)).tolist()

    return np_matrix