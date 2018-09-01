from generador_grafos import *
from grafos_utils import *
from exp_base import *


def generar_grafos(dir):
    print("Generando grafos en directorio: '{}'".format(dir))
    grafos = []

    # Grafo trivial
    n_t5 = 5
    t5 = generar_grafo_trivial(n_t5)
    grafos.append(("trivial-{}-nodos".format(n_t5), t5))
    
    # Grafo completo
    n_k5 = 5
    k5 = agregar_grafo_completo(generar_grafo_trivial(n_k5))
    grafos.append(("completo-{}-nodos".format(n_k5) , k5))

    # Grafo union estrellas hacia adentro
    cant_estrellas_in = 3
    cant_nodos_por_estrellas_in = 5
    ue_in = generar_grafo_estrellas_unidas_hacia_adentro(cant_estrellas_in, cant_nodos_por_estrellas_in)
    grafos.append(("union-{}-estrellas-{}-in-{}-nodos".format(cant_estrellas_in, cant_nodos_por_estrellas_in, cant_estrellas_in*cant_nodos_por_estrellas_in+1) , ue_in))

    # Grafo union estrellas hacia afuera
    cant_estrellas_out = 3
    cant_nodos_por_estrellas_out = 5
    ue_out = generar_grafo_estrellas_unidas_hacia_afuera(cant_estrellas_out, cant_nodos_por_estrellas_out)
    grafos.append(("union-{}-estrellas-{}-out-{}-nodos".format(cant_estrellas_out, cant_nodos_por_estrellas_out, cant_estrellas_out*cant_nodos_por_estrellas_out+1) , ue_out))

    # Limpio directorio o lo creo si no existe
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.mkdir(dir)

    # Genero grafos
    for grafo in grafos:
        print("Generando ahora '{}':".format(grafo[0]))
        guardar_matriz_en_archivo(grafo[1], "{}/{}".format(dir, grafo[0]), False)

    print("Listo! Generacion de grafos OK")     

def escribir_resultados_en_archivo(archivo, n, nombre, p, ranks):
    comma_sep_ranks = ",".join([str(ranks[i]) for i in range(0, n)])
    archivo.write("{},{},{},{}\n".format(nombre,n,p,comma_sep_ranks))
    print("{},{},{},{}".format(nombre,n,p,comma_sep_ranks))
    return 1    
    

def ejecutar_y_escribir_resultado_variando_p(cant_nodos, cant_p, input_name, t_files):
    # Creo archivo resultado
    n = obtener_cant_nodos_de_nombre_archivo(input_name)
    resultado = open(t_files.output_dir + input_name + "-var-p-data", "w")
    resultado.write("nombre,n,p," + ",".join([str(i) for i in range(0, n)]) + "\n") # header

    # Para cada grafo generado, vamos variando p y escribiendo los resultados
    for p in list(numpy.linspace(0, 1, cant_p))[1:-1]:
        output = ejecutar_con_input(t_files.input_dir + input_name, p)
        p, ranks = parsear_output(output)
        escribir_resultados_en_archivo(resultado, n, input_name, p, ranks)

    resultado.close()

class TestFiles: 
    def __init__(self, input_dir, output_dir):
        self.input_dir = input_dir
        self.output_dir = output_dir

# Parametros
t_files = TestFiles("gen/", "resultados/")
cant_nodos = 20
cant_p = 10

# Creo carpeta output si no existe
if not os.path.exists(t_files.output_dir):
    os.mkdir(t_files.output_dir)
 
# Genero grafos
generar_grafos(t_files.input_dir)

# Ejecuto inputs generados
input_files = os.listdir(t_files.input_dir)
for input_name in input_files:
    print("Ejecutando ahora " + input_name)
    ejecutar_y_escribir_resultado_variando_p(cant_nodos, cant_p, input_name, t_files)
    print("")


