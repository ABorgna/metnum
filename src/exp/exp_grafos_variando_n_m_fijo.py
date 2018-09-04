from generador_grafos import *
from grafos_utils import *
from exp_base import *
import time


def generar_grafos(t_args, t_files):
    print("Generando grafos en directorio: '{}/'".format(t_files.input_dir))
    print("--> params: {} <= n < {}, step_n:{}, m={}, #intentos={}".format(t_args.min_cant_nodos, t_args.max_cant_nodos, t_args.step_cant_nodos, t_args.cant_aristas, t_args.cant_intentos))
    
    # Limpio directorio o lo creo si no existe
    if os.path.exists(t_files.input_dir):
        shutil.rmtree(t_files.input_dir)
    os.mkdir(t_files.input_dir)
    
    for n in range(t_args.min_cant_nodos, t_args.max_cant_nodos, t_args.step_cant_nodos):
        print("Generando ahora random-{}-nodos (x{})".format(n, t_args.cant_intentos))

        for nro_prueba in range(0, t_args.cant_intentos):
            g = generar_grafo_random_m_fijo(n, t_args.cant_aristas)
            nombre = "random-{}-nodos-{}".format(n, nro_prueba)
            guardar_matriz_en_archivo(g, "{}/{}".format(t_files.input_dir, nombre), False)

    print("Listo! Generacion de grafos OK")     

def escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file):
    resultado.write("{},{},{},{},{},{},{},{},{},{}\n".format(input_name, t_args.p, t_args.cant_aristas, t_args.min_cant_nodos, t_args.max_cant_nodos, t_args.step_cant_nodos, cant_nodos, nro_intento, t_args.cant_intentos, tiempo_ns))
    print("{},{},{},{},{},{},{},{},{},{}".format(input_name, t_args.p, t_args.cant_aristas, t_args.min_cant_nodos, t_args.max_cant_nodos, t_args.step_cant_nodos, cant_nodos, nro_intento, t_args.cant_intentos, tiempo_ns))

def parsear_nombre(input_name):
    tokens = input_name.split("-")
    return tokens[1],tokens[3]
    
def ejecutar_y_escribir_resultado_variando_p(input_name, t_args, t_file):
    cant_nodos, nro_intento = parsear_nombre(input_name)

    tiempo_inicial = time.time_ns()
    ejecutar_con_input(t_file.input_dir + input_name, t_args.p)
    tiempo_final = time.time_ns()

    tiempo_ns = tiempo_final - tiempo_inicial
    escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)

class TestFiles: 
    def __init__(self, input_dir, output_dir, output_file):
        self.input_dir = input_dir
        self.output_dir = output_dir
        self.output_file = output_file

class TestArgs:
    def __init__(self, p, cant_aristas, cant_intentos, min_cant_nodos, max_cant_nodos, step_cant_nodos):
        self.cant_aristas = cant_aristas
        self.p = p
        self.cant_intentos = cant_intentos
        self.min_cant_nodos = min_cant_nodos
        self.max_cant_nodos = max_cant_nodos
        self.step_cant_nodos = step_cant_nodos

# Parametros
t_files = TestFiles("gen/", "resultados/", "random-var-n-m-fijo-data.csv")
t_args = TestArgs(0.5, 380, 20, 20, 200, 5)

# Creo carpeta output si no existe
if not os.path.exists(t_files.output_dir):
    os.mkdir(t_files.output_dir)

# Genero grafos
generar_grafos(t_args, t_files)

# Creo archivo resultado
resultado = open(t_files.output_dir + t_files.output_file, "w")
resultado.write("nombre,p,m,min_cant_nodos,max_cant_nodos,step_nodos,cant_nodos,nro_intento,cant_intentos,tiempo_ns\n") # header

# Ejecuto todos inputs generados
input_files = os.listdir(t_files.input_dir)
for input_name in input_files:
    print("Ejecutando ahora " + input_name)
    ejecutar_y_escribir_resultado_variando_p(input_name, t_args, t_files)
    print("")

resultado.close()

