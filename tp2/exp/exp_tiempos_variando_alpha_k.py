import numpy
import time
from exp_base import *


def escribir_resultados_en_archivo(res, resultado):
    tiempo_ns = res["time-testing"][:res["time-testing"].find("ms")]
    resultado.write("{},{},{},{},{},{},{},{},{},{}\n".format(res["accuracy"], res["alpha"], res["k"],res["countEntries"], res["recall"], res["falseP"], res["falseN"], res["trueN"], res["trueP"],tiempo_ns))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_alpha = 1
    max_alpha = 15
    min_k = exp_args["MIN_K"]
    max_k = exp_args["MAX_K"]

    resultado = open("./tiempos_variando_alpha_k"+str(min_alpha)+"_"+str(max_alpha)+"_"+str(min_k)+"_"+str(max_k)+".csv", "w")
    resultado.write("accuracy,alpha,k,testing entries,recall,falseP,falseN,trueN,trueP,tiempo\n") # header
    
    for k in list(numpy.linspace(min_k, max_k, exp_args["CANT_K"]))[1:-1]:
        for alpha in list(numpy.linspace(min_alpha, max_alpha, 15))[1:-1]:
            program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                            "-t": exp_args["TRAINING_FILE"],
                            "-q": exp_args["TESTING_FILE"],
                            "-m": exp_args["METHOD_NUMBER"],
                            "--maxVocabFreq" : 0.1,
                            "--cache": exp_args["CACHE_FOLDER"],
                            "-k" : k,
                            "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                            "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                            #"-o" : "./res_variando_alpha"+str(min_alpha)+"_"+str(max_alpha)+".csv",
                            "-a": alpha,
                "--quiet": ""}

            output = ejecutar_con_args(program_args)
            res = parsear_output(output)
            escribir_resultados_en_archivo(res, resultado)
        print(res["accuracy"])
    resultado.close()



exp_args = {"ALPHA": 15,
            
            "CANT_K": 15,
            "MIN_K": 1,
            "MAX_K": 100,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 2,
            "NUMBER_OF_TRAINING_ENTRIES": 10000,
            "CACHE_FOLDER": "../cache/",
            "NUMBER_OF_TESTING_ENTRIES": 10000}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")