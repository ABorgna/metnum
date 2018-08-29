from generador_grafos import *
from grafos_utils import *
from exp_base import *


def generar_grafos(output_dir="gen", logs_activados=True):
    if logs_activados:
        print("Generando grafos en directorio: '{}/'".format(output_dir))

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
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)

    # Genero grafos
    for grafo in grafos:
        if logs_activados:
            print("Generando ahora '{}':".format(grafo[0]))
            imprimir_matriz(grafo[1])
            print("")
        guardar_matriz_en_archivo(grafo[1], "{}/{}".format(output_dir, grafo[0]), False)

    if logs_activados:
        print("Listo! Generacion de grafos OK")     

def escribir_resultados_en_archivo(archivo, n, nombre, p, ranks):
    comma_sep_ranks = ",".join([str(ranks[i]) for i in range(0, n)])
    archivo.write("{},{},{}\n".format(nombre,p,comma_sep_ranks))
    print("{},{},{}".format(nombre,p,comma_sep_ranks))
    return 1    
    

def ejecutar_y_escribir_resultado_variando_p(cant_p, input_file, output_dir="resultados/"):
    # Creo archivo resultado
    n = obtener_cant_nodos_de_nombre_archivo(input_file)
    resultado = open(output_dir + input_file + "-var-p-data", "w")
    resultado.write("nombre,p," + ",".join([str(i) for i in range(0, n)]) + "\n") # header

    # Para cada grafo generado, vamos variando p y escribiendo los resultados
    for p in list(numpy.linspace(0, 1, cant_p))[1:]:
        output = ejecutar_con_input(input_file, p)
        p, ranks = parsear_output(output)
        escribir_resultados_en_archivo(resultado, n, input_file, p, ranks)

    resultado.close()



output_dir = "resultados/"
input_dir = "gen/"

# Creo carpeta output si no existe
if not os.path.exists(output_dir):
    os.mkdir(output_dir)

generar_grafos()

input_files = os.listdir(input_dir)
for input in input_files:
    print("Ejecutando ahora " + input)
    ejecutar_y_escribir_resultado_variando_p(10, input, output_dir)
    print("")


