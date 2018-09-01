def imprimir_matriz(matriz):
    n = len(matriz)
    for i in range(0, n):
        fila = ""
        for j in range(0, n):
            fila = fila + str(matriz[i][j]) + ' '
        print(fila)
        
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