import numpy
import time
from exp_base import *


def escribir_resultados_en_archivo(res, resultado, tiempo_ns):
    resultado.write("{},{},{},{},{},{},{},{},{},{}\n".format(res["accuracy"], res["alpha"], res["k"],res["countEntries"], res["recall"], res["falseP"], res["falseN"], res["trueN"], res["trueP"],tiempo_ns))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_alpha = exp_args["MIN_ALPHA"]
    max_alpha = exp_args["MAX_ALPHA"]
    min_k = exp_args["MIN_K"]
    max_k = exp_args["MAX_K"]

    resultado = open("./tiempos_variando_alpha_k"+str(min_alpha)+"_"+str(max_alpha)+"_"+str(min_k)+"_"+str(max_k)+".csv", "w")
    resultado.write("accuracy,alpha,k,testing entries,recall,falseP,falseN,trueN,trueP,tiempo\n") # header
    
    for k in list(numpy.linspace(min_k, max_k, exp_args["CANT_K"]))[1:-1]:
        for alpha in list(numpy.linspace(min_alpha, max_alpha, exp_args["CANT_ALPHA"]))[1:-1]:
            program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                            "-t": exp_args["TRAINING_FILE"],
                            "-q": exp_args["TESTING_FILE"],
                            "-m": exp_args["METHOD_NUMBER"],
                            "-k" : k,
                            "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                            "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                            #"-o" : "./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv",
                            "-a": alpha,
                "--quiet": ""}

            tiempo_inicial = time.time()

            output = ejecutar_con_args(program_args)
            tiempo_final = time.time()

            tiempo_ns = tiempo_final - tiempo_inicial
            #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
            res = (parsear_output(output)) 
            escribir_resultados_en_archivo(res, resultado, tiempo_ns)
        print(res["accuracy"])
    resultado.close()



exp_args = {"CANT_ALPHA": 5,
            "MIN_ALPHA": 10,
            "MAX_ALPHA": 50,
            "CANT_K": 10,
            "MIN_K": 1,
            "MAX_K": 50,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 2,
            "NUMBER_OF_TRAINING_ENTRIES": 200,
            "NUMBER_OF_TESTING_ENTRIES": 200}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")