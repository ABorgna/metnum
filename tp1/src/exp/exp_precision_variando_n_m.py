from generador_grafos import *
from grafos_utils import *
from exp_base import *
import time


def generar_grafos(t_args, cant_densidades, cant_intentos, output_dir="gen"):
    print("Generando grafos en directorio: '{}/'".format(output_dir))
    #print("--> params: n={}, #densidades={}, #intentos={}".format(cant_nodos, cant_densidades, cant_intentos))
    
    # Limpio directorio o lo creo si no existe
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.mkdir(output_dir)
    for n in range(t_args.min_cant_nodos, t_args.max_nodos, t_args.step_nodos):
    
        for p_densidad in list(numpy.linspace(0.00, 0.95, cant_densidades)):
            print("Generando ahora random-{}-pdens-{}-cant_nodos (x{})".format(p_densidad, n, cant_intentos))
            g = generar_grafo_random_de_densidad(n, p_densidad)
            nombre = "random-{}-pdens-{}-cant_nodos-{}".format(p_densidad, n, 1)
            guardar_matriz_en_archivo(g, "{}/{}".format(output_dir, nombre), False)

    print("Listo! Generacion de grafos OK")     

def escribir_resultados_en_archivo(input_name, cant_nodos, p_densidad, nro_intento, val, t_args, t_file, p):
    resultado.write("{},{},{},{},{},{},{},{}\n".format(input_name, cant_nodos, p, p_densidad, t_args.cant_densidades, nro_intento, t_args.cant_intentos, val))
    print("{},{},{},{},{},{},{},{}".format(input_name, cant_nodos, p, p_densidad, t_args.cant_densidades, nro_intento, t_args.cant_intentos, val))

def parsear_nombre(input_name):
    tokens = input_name.split("-")
    return tokens[3],tokens[1]
    
def ejecutar_y_escribir_resultado_variando_n(input_name, t_args, t_file):
    cant_nodos, p_densidad = parsear_nombre(input_name)

    #val = ejecutar_con_input(t_file.input_dir + input_name, t_args.p)
    #val = float(val) 
    #print(val)

    #tiempo_ns = tiempo_final - tiempo_inicial
    #escribir_resultados_en_archivo(input_name, p_densidad, nro_intento, val, t_args, t_file)

    # Para cada grafo generado, vamos variando n y escribiendo los resultados
   # for n in list(numpy.linspace(0, 500, t_args.cant_nodos)):
    val = ejecutar_con_args(["-j ", t_files.input_dir + input_name, t_args.p])
    val = float(val)
    escribir_resultados_en_archivo(input_name, cant_nodos, p_densidad, 1, val, t_args, t_file, t_args.p)

class TestFiles: 
    def __init__(self, input_dir, output_dir, output_file):
        self.input_dir = input_dir
        self.output_dir = output_dir
        self.output_file = output_file

class TestArgs:
    def __init__(self, p, min_cant_nodos, cant_intentos, cant_densidades, step_nodos, max_nodos):
        self.max_nodos = max_nodos
        self.step_nodos = step_nodos
        self.p = p
        self.min_cant_nodos = min_cant_nodos
        self.cant_intentos = cant_intentos
        self.cant_densidades = cant_densidades

# Parametros
t_files = TestFiles("gen/", "resultados/", "random-var-m-n-data.csv")
t_args = TestArgs(0.5, 1, 1, 20, 20, 500)

# Creo carpeta output si no existe
if not os.path.exists(t_files.output_dir):
    os.mkdir(t_files.output_dir)

# Genero grafos
generar_grafos(t_args, t_args.cant_densidades, t_args.cant_intentos)

# Creo archivo resultado
resultado = open(t_files.output_dir + t_files.output_file, "w")
#comma_sep_nodos = ",".join([str(i) for i in range(0,  t_args.cant_nodos)])
resultado.write("nombre,n,p,p_densidad,cant_densidades,nro_intento,cant_intentos,val\n") # header

# Ejecuto todos inputs generados
input_files = os.listdir(t_files.input_dir)
for input_name in input_files:
    print("Ejecutando ahora " + input_name)
    ejecutar_y_escribir_resultado_variando_n(input_name, t_args, t_files)
    print("")

resultado.close()
