import numpy
import time
from exp_base import *

def escribir_resultados_en_archivo(res, resultado, tiempo_ns, conalpha):
    resultado.write("{},{},{},{},{},{},{},{},{},{}\n".format(res["accuracy"], res["alpha"], res["k"],res["countEntries"], res["recall"], res["falseP"], res["falseN"], res["trueN"], res["trueP"],conalpha))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_k = exp_args["MIN_K"]
    max_k = exp_args["MAX_K"]

    resultado = open("./res/exp_pca_vs_sin_pca.csv", "w")
    resultado.write("accuracy,alpha,k,testing entries,conAlpha\n") # header
    
    
    for k in list(numpy.linspace(min_k, max_k, exp_args["CANT_K"]))[1:-1]:
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": exp_args["METHOD_NUMBER"],
                        "-k" : k,
                        "-a" : 168278,
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                        #"-o" : "./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv",
            "--quiet": ""}

        tiempo_inicial = time.time()

        output = ejecutar_con_args(program_args)
        tiempo_final = time.time()

        tiempo_ns = tiempo_final - tiempo_inicial
        #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado, tiempo_ns, 1)
        print(res["accuracy"])        
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": 0,
                        "-k" : k,
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                        #"-o" : "./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv",
            "--quiet": ""}

        tiempo_inicial = time.time()

        output = ejecutar_con_args(program_args)
        tiempo_final = time.time()

        tiempo_ns = tiempo_final - tiempo_inicial
        #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado, tiempo_ns, 0)
        print(res["accuracy"])
    resultado.close()



exp_args = {"CANT_K": 20,
            "MIN_K": 1,
            "MAX_K": 100,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 2,
            "NUMBER_OF_TRAINING_ENTRIES": 2000,
            "NUMBER_OF_TESTING_ENTRIES": 2000}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")