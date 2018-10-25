import numpy
import time
from exp_base import *

def escribir_resultados_en_archivo(res, resultado, tiempo_ns, training):
    resultado.write("{},{},{},{}\n".format(res["accuracy"], training, tiempo_ns, res["recall"]))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_training = exp_args["MIN_TRAINING"]
    max_training = exp_args["MAX_TRAINING"]
    cant_training = exp_args["CANT_TRAINING"]

    resultado = open("./exp_training_entries_vs_accuracy_vs_tiempo"+str(min_training)+"_"+str(max_training)+".csv", "w")
    resultado.write("accuracy,training entries,tiempo,recall\n") # header
    
    
    for training in list(numpy.linspace(min_training, max_training, cant_training))[1:-1]:
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": exp_args["METHOD_NUMBER"],
                        "-k" : 5,
                        "--train-entries": training,
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                        #"-o" : "./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv",
                        "--quiet": ""}

        tiempo_inicial = time.time()

        output = ejecutar_con_args(program_args)
        tiempo_final = time.time()

        tiempo_ns = tiempo_final - tiempo_inicial
        #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado, tiempo_ns, training)
        print(res["accuracy"])
    resultado.close()



exp_args = {"MIN_TRAINING": 10,
            "MAX_TRAINING": 20000,
            "CANT_TRAINING": 10,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 3,
            "NUMBER_OF_TESTING_ENTRIES": 2000}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")