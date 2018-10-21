
from exp_base import *

def ejecutar_y_escribir_resultado_variando_alpha(cant_alpha):
    # Creo archivo resultado
 
    # Para cada grafo generado, vamos variando p y escribiendo los resultados
    for alpha in list(numpy.linspace(5, 5000, cant_alpha))[1:-1]:
        ejecutar_con_args(['-o ./coso.csv', "--train-entries 200",  "--test-entries 200", " -a",  alpha])
        #ejecutar_con_args()

class TestFiles: 
    def __init__(self, input_dir, output_dir):
        self.input_dir = input_dir
        self.output_dir = output_dir

# Ejecuto input generados (estrellas)
    print("Ejecutando ahora ")
    ejecutar_y_escribir_resultado_variando_alpha(100)
    print("")
