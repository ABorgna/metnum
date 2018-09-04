from generador_grafos import *
from grafos_utils import *
from exp_base import *
import time


def generar_grafos(cant_nodos, cant_densidades, cant_intentos, output_dir="gen"):
    print("Generando grafos en directorio: '{}/'".format(output_dir))
    print("--> params: n={}, #densidades={}, #intentos={}".format(cant_nodos, cant_densidades, cant_intentos))
    
    # Limpio directorio o lo creo si no existe
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)
    
    for p_densidad in list(numpy.linspace(0, 1, cant_densidades)):
        print("Generando ahora random-{}-pdens (x{})".format(p_densidad, cant_intentos))

        for nro_prueba in range(0, cant_intentos):
            g = generar_grafo_random_de_densidad(cant_nodos, p_densidad)
            nombre = "random-{}-pdens-{}".format(p_densidad, nro_prueba)
            guardar_matriz_en_archivo(g, "{}/{}".format(output_dir, nombre), False)

    print("Listo! Generacion de grafos OK")     

def escribir_resultados_en_archivo(input_name, p_densidad, nro_intento, tiempo_ns, t_args, t_file):
    resultado.write("{},{},{},{},{},{},{},{}\n".format(input_name, t_args.cant_nodos, t_args.p, p_densidad, t_args.cant_densidades, nro_intento, t_args.cant_intentos, tiempo_ns))
    print("{},{},{},{},{},{},{},{}".format(input_name, t_args.cant_nodos, t_args.p, p_densidad, t_args.cant_densidades, nro_intento, t_args.cant_intentos, tiempo_ns))

def parsear_nombre(input_name):
    tokens = input_name.split("-")
    return tokens[1],tokens[3]
    
def ejecutar_y_escribir_resultado_variando_p(input_name, t_args, t_file):
    p_densidad, nro_intento = parsear_nombre(input_name)

    tiempo_inicial = time.time_ns()
    ejecutar_con_input(t_file.input_dir + input_name, t_args.p)
    tiempo_final = time.time_ns()

    tiempo_ns = tiempo_final - tiempo_inicial
    escribir_resultados_en_archivo(input_name, p_densidad, nro_intento, tiempo_ns, t_args, t_file)

class TestFiles: 
    def __init__(self, input_dir, output_dir, output_file):
        self.input_dir = input_dir
        self.output_dir = output_dir
        self.output_file = output_file

class TestArgs:
    def __init__(self, p, cant_nodos, cant_intentos, cant_densidades):
        self.cant_nodos = cant_nodos
        self.p = p
        self.cant_intentos = cant_intentos
        self.cant_densidades = cant_densidades

# Parametros
t_files = TestFiles("gen/", "resultados/", "random-var-m-data.csv")
t_args = TestArgs(0.7, 100, 20, 20)

# Creo carpeta output si no existe
if not os.path.exists(t_files.output_dir):
    os.mkdir(t_files.output_dir)

# Genero grafos
generar_grafos(t_args.cant_nodos, t_args.cant_densidades, t_args.cant_intentos)

# Creo archivo resultado
resultado = open(t_files.output_dir + t_files.output_file, "w")
comma_sep_nodos = ",".join([str(i) for i in range(0,  t_args.cant_nodos)])
resultado.write("nombre,n,p,p_densidad,cant_densidades,nro_intento,cant_intentos,tiempo_ns\n") # header

# Ejecuto todos inputs generados
input_files = os.listdir(t_files.input_dir)
for input_name in input_files:
    print("Ejecutando ahora " + input_name)
    ejecutar_y_escribir_resultado_variando_p(input_name, t_args, t_files)
    print("")

resultado.close()

