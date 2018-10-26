import numpy
import time
from exp_base import *

def escribir_resultados_en_archivo(res, resultado):
    tiempo_ns = res["time-testing"][:res["time-testing"].find("ms")]
    resultado.write("{},{},{},{},{}\n".format(res["accuracy"], res["k"],res["countEntries"], tiempo_ns, res["method"]))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_k = exp_args["MIN_K"]
    max_k = exp_args["MAX_K"]

    resultado = open("./res_pca_n_vs_sin_pca_"+ str(min_k)+"_"+str(max_k)+".csv", "w")
    resultado.write("accuracy,k,testing entries,tiempo,metodo\n") # header
    
    
    for k in list(numpy.linspace(min_k, max_k, exp_args["CANT_K"]))[1:-1]:
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": 2,
                        "-k" : k,
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                        "-a": exp_args["ALPHA"],
                        "--maxVocabFreq": exp_args["MAX_VOCAB_FREQ"],
                        "--cache": exp_args["CACHE_FOLDER"],
                        "--quiet": ""}

        output = ejecutar_con_args(program_args)
        res = (parsear_output(output)) 
        escribir_resultados_en_archivo(res, resultado)

        program_args["-m"] = 0
        output = ejecutar_con_args(program_args)
        res = (parsear_output(output))
        escribir_resultados_en_archivo(res, resultado)
        print()
    resultado.close()



exp_args = {"ALPHA": 1497, # 1497 = 0.1 * #vocabs
            "CANT_K": 15,
            "MIN_K": 1,
            "MAX_K": 200,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "CACHE_FOLDER": "../cache/",
            "NUMBER_OF_TRAINING_ENTRIES": 25000,
            "NUMBER_OF_TESTING_ENTRIES": 2000,
            "MAX_VOCAB_FREQ": 0.1}

print("Ejecutando ahora ejecutar_y_escribir_resultado_variando_alpha")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")