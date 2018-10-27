import numpy
import time
from exp_base import *


def escribir_resultados_en_archivo(res, resultado, tiempo_ns, norma):
    tiempo_ns = res["time-testing"][:res["time-testing"].find("ms")]
    resultado.write("{},{},{},{},{},{},{},{},{},{},{}\n".format(res["accuracy"], res["alpha"], res["k"],res["countEntries"], res["recall"], res["falseP"], res["falseN"], res["trueN"], res["trueP"],tiempo_ns, norma))

def ejecutar_y_escribir_resultado_variando_norma(exp_args):
    cant_normas = exp_args["CANT_NORMAS"]
    
    min_alpha = exp_args["MIN_ALPHA"]
    max_alpha = exp_args["MAX_ALPHA"]
    resultado = open("./res/exp_distintas_normas"+str(cant_normas)+".csv", "w")
    resultado.write("accuracy,alpha,k,testing entries,recall,falseP,falseN,trueN,trueP,tiempo,norma\n") # header
    for alpha in list(numpy.linspace(min_alpha, max_alpha, exp_args["CANT_ALPHA"]))[1:-1]:
        for norm in range(1, cant_normas):
            program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                            "-t": exp_args["TRAINING_FILE"],
                            "-q": exp_args["TESTING_FILE"],
                            "-m": exp_args["METHOD_NUMBER"],
                            "--maxVocabFreq" : 0.1,
                            "-n" : norm,
                            "-a" : alpha,
                            "--cache": exp_args["CACHE_FOLDER"],
                            "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                            "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                "--quiet": ""}

            tiempo_inicial = time.time()

            output = ejecutar_con_args(program_args)
            tiempo_final = time.time()

            tiempo_ns = tiempo_final - tiempo_inicial
            #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
            res = (parsear_output(output)) 
            escribir_resultados_en_archivo(res, resultado, tiempo_ns, norm)   
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": exp_args["METHOD_NUMBER"],
                        "--maxVocabFreq" : 0.1,
                        "-a" : alpha,
                        "--cache": exp_args["CACHE_FOLDER"],
                        "-n" : "chi2",
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
            "--quiet": ""}

        tiempo_inicial = time.time()

        output = ejecutar_con_args(program_args)
        tiempo_final = time.time()

        tiempo_ns = tiempo_final - tiempo_inicial
        #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado, tiempo_ns, "chi2")    
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": exp_args["METHOD_NUMBER"],
                        "-n" : "inf",
                        "--maxVocabFreq" : 0.1,
                        "-a" : alpha,
                        "--cache": exp_args["CACHE_FOLDER"],
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
            "--quiet": ""}

        tiempo_inicial = time.time()

        output = ejecutar_con_args(program_args)
        tiempo_final = time.time()

        tiempo_ns = tiempo_final - tiempo_inicial
        #escribir_resultados_en_archivo(input_name, cant_nodos, nro_intento, tiempo_ns, t_args, t_file)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado, tiempo_ns, "inf")
    resultado.close()



exp_args = {"VOCAB_FILE": "../data/vocab.csv",
            "CANT_NORMAS": 5,
            "MIN_ALPHA" : 5,
            "MAX_ALPHA" : 50,
            "CANT_ALPHA" : 10,
            "--maxVocabFreq" : 0.1,
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 2,
            "NUMBER_OF_TRAINING_ENTRIES": 2000,
            "NUMBER_OF_TESTING_ENTRIES": 5000,
            "CACHE_FOLDER": "../cache/"}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_norma(exp_args)
print("Listo!")