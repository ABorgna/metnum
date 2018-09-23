from generador_grafos import *
from grafos_utils import *
from exp_base import *


def generar_grafos_simples(dir, cant_nodos):
    print("Generando grafos en directorio: '{}'".format(dir))
    print("--> params: cant_nodos={}".format(cant_nodos))

    # Limpio directorio o lo creo si no existe
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.mkdir(dir)

    # Grafo trivial
    print("Generando ahora grafo-trivial")
    trivial = generar_grafo_trivial(cant_nodos["trivial"])
    guardar_matriz_en_archivo(trivial, dir + "trivial", False)

    # Grafo completo
    print("Generando ahora grafo-completo")
    completo = agregar_grafo_completo(generar_grafo_trivial(cant_nodos["completo"]))
    guardar_matriz_en_archivo(completo, dir + "completo", False)

    print("Listo! Generacion de grafos OK")     

def generar_grafos_estrella(dir, cant_nodos):
    # Limpio directorio
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.mkdir(dir)

    # Grafo union estrellas hacia adentro
    print("Generando ahora grafo-union-estrellas-in")
    cant_estrellas_in = cant_nodos["union-estrellas-in"][0]
    cant_nodos_por_estrellas_in = cant_nodos["union-estrellas-in"][1]
    union_estrellas_in = generar_grafo_estrellas_unidas_hacia_adentro(cant_estrellas_in, cant_nodos_por_estrellas_in)
    guardar_matriz_en_archivo(union_estrellas_in, dir + "union-estrellas-in", False)

    # Grafo union estrellas hacia afuera
    print("Generando ahora grafo-union-estrellas-out")
    cant_estrellas_out = cant_nodos["union-estrellas-out"][0]
    cant_nodos_por_estrellas_out = cant_nodos["union-estrellas-out"][1]
    union_estrellas_out = generar_grafo_estrellas_unidas_hacia_afuera(cant_estrellas_out, cant_nodos_por_estrellas_out)
    guardar_matriz_en_archivo(union_estrellas_out, dir + "union-estrellas-out", False)

def escribir_resultados_en_archivo_simple(archivo, n, nombre, p, ranks):
    for i in range(0, n):
        archivo.write("{},{},{},{},{}\n".format(nombre,n,p,i,ranks[i]))
        print("{},{},{},{},{}".format(nombre,n,p,i,ranks[i]))

def escribir_resultados_en_archivo_estrellas(archivo, n, nombre, p, ranks):
    cant_nodos_totales = n[0] * n[1] + 1
    for i in range(0,cant_nodos_totales):
        archivo.write("{},{},{},{},{},{}\n".format(nombre,n[0],n[1],p, i, ranks[i]))
        print("{},{},{},{},{},{}".format(nombre,n[0],n[1],p, i, ranks[i]))

def ejecutar_y_escribir_resultado_variando_p(cant_nodos, cant_p, input_name, t_files, header, escribir_resultados_en_archivo):
    # Creo archivo resultado
    resultado = open(t_files.output_dir + input_name + "-var-p-data.csv", "w")
    resultado.write(header)

    # Para cada grafo generado, vamos variando p y escribiendo los resultados
    for p in list(numpy.linspace(0, 1, cant_p))[1:-1]:
        output = ejecutar_con_args(['-o -', t_files.input_dir + input_name, p])
        p, ranks = parsear_output(output)
        escribir_resultados_en_archivo(resultado, cant_nodos, input_name, p, ranks)

    resultado.close()

class TestFiles: 
    def __init__(self, input_dir, output_dir):
        self.input_dir = input_dir
        self.output_dir = output_dir

# Parametros
t_files = TestFiles("gen/", "resultados/")
cant_nodos = {"trivial":10, "completo":10, "union-estrellas-in":(3,4), "union-estrellas-out":(3,4)}
cant_p = 10

# Creo carpeta output si no existe
if not os.path.exists(t_files.output_dir):
    os.mkdir(t_files.output_dir)
 
# Genero grafos simples (trivial, completo)
generar_grafos_simples(t_files.input_dir, cant_nodos)

# Ejecuto inputs generados (simples)
input_files = os.listdir(t_files.input_dir)
for input_name in input_files:
    print("Ejecutando ahora " + input_name)
    header = "nombre,n,p,nodo,val_nodo\n"
    ejecutar_y_escribir_resultado_variando_p(cant_nodos[input_name], cant_p, input_name, t_files, header, escribir_resultados_en_archivo_simple)
    print("")

# Genero grafos estrella (union estrella in, union estrella)
generar_grafos_estrella(t_files.input_dir, cant_nodos)

# Ejecuto input generados (estrellas)
input_files = os.listdir(t_files.input_dir)
for input_name in input_files:
    print("Ejecutando ahora " + input_name)
    header = "nombre,cant_estrellas,cant_nodos_por_estrella,p,nodo,val_nodo\n"
    ejecutar_y_escribir_resultado_variando_p(cant_nodos[input_name], cant_p, input_name, t_files, header, escribir_resultados_en_archivo_estrellas)
    print("")



