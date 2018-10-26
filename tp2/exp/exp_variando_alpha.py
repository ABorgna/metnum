import numpy
import time
from exp_base import *

def escribir_resultados_en_archivo(res, resultado):
    tiempo_ns = res["time-testing"][:res["time-testing"].find("ms")]
    resultado.write("{},{},{},{}\n".format(res["accuracy"], res["alpha"], res["k"],res["countEntries"]))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_alpha = exp_args["MIN_ALPHA"]
    max_alpha = exp_args["MAX_ALPHA"]

    resultado = open("./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv", "w")
    resultado.write("accuracy,alpha,k,testing entries\n") # header
    
    
    for alpha in list(numpy.linspace(min_alpha, max_alpha, exp_args["CANT_ALPHA"]))[1:-1]:
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": exp_args["METHOD_NUMBER"],
                        "--maxVocabFreq" : 0.1,
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                        #"-o" : "./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv",
                        "-a": alpha,
			"--quiet": ""}
        
        output = ejecutar_con_args(program_args)
        #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado)
        print(res["accuracy"])
    resultado.close()



exp_args = {"CANT_ALPHA": 30,
            "MIN_ALPHA": 1,
            "MAX_ALPHA": 100,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 2,
            "NUMBER_OF_TRAINING_ENTRIES": 7500,
            "NUMBER_OF_TESTING_ENTRIES": 10000}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")

