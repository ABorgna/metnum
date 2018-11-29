import csv_converter
import sys
from utils import *

if len(sys.argv) < 4:
    print("Correr python3 runner.py <archivo> <archivo output> <tamano>.")
    print("Por ejemplo: python3 runner.py ../data/phantom.png ../data/output.png 40")
    exit(0)

file = sys.argv[1]
output_file = sys.argv[2]
tamano = int(sys.argv[3])

input_folder = "/".join(file.split("/")[0:-1]) + "/"
output_folder= "/".join(output_file.split("/")[0:-1]) + "/"
filename = file.split("/")[-1]
filename_output = output_file.split("/")[-1]
print("-------------PARAMS--------------")
print("Archivo entrada: " + filename)
print("Carpeta entrada: " + input_folder)

print("Archivo salida: " + filename_output)
print("Carpeta salida: " + output_folder)
print("Tamaño: " + str(tamano) + "\n")

print("------GENERACION DE ARCHIVO------")
archivo_generado_ruta = csv_converter.convertImg(file, input_folder, output_folder, ".png", ".csv", tamano)
print("Archivo generado:" + archivo_generado_ruta + "\n")


print("-----------EJECUCION-------------")
cmd = "../tp3 {} {} -n {}".format(archivo_generado_ruta, output_file,tamano)
print(cmd + "\n")
output = ejecutar(cmd)
# print(output)

print("----------PARSE OUTPUT-----------")
parseado = parsear_output(output)
print(parseado)
print()


