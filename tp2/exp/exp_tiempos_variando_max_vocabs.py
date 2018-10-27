import numpy
import time
from exp_base import *

def escribir_resultados_en_archivo(res, resultado, max_vocab_freq):
    tiempo_ns = res["time-testing"][:res["time-testing"].find("ms")]
    resultado.write("{},{},{},{},{}\n".format(res["accuracy"], max_vocab_freq,res["countEntries"], tiempo_ns, res["method"]))

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min = exp_args["MIN_MAX_VOCAB_FREQ"]
    max = exp_args["MAX_MAX_VOCAB_FREQ"]

    resultado = open("./tiempos_var_max_vocabs_freq-"+ str(min)+"_"+str(max)+".csv", "w")
    resultado.write("accuracy,max_vocab_freq,testing entries,tiempo,metodo\n") # header


    for max_vocab_freq in list(numpy.linspace(min, max, exp_args["CANT_MAX_VOCAB_FREQ"]))[1:-1]:
        for i in range(0,4):
            program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                            "-t": exp_args["TRAINING_FILE"],
                            "-q": exp_args["TESTING_FILE"],
                            "-m": i,
                            "-k" : exp_args["K"],
                            "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                            "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                            "-a": exp_args["ALPHA"],
                            "--maxVocabFreq": max_vocab_freq,
                            "--cache": exp_args["CACHE_FOLDER"],
                            "--quiet": ""}

            output = ejecutar_con_args(program_args)
            res = (parsear_output(output))
            escribir_resultados_en_archivo(res, resultado, max_vocab_freq   )
        print()
    resultado.close()



exp_args = {"ALPHA": 200, # 1497 = 0.1 * #vocabs
            "K": 50,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "CACHE_FOLDER": "../cache/",
            "NUMBER_OF_TRAINING_ENTRIES": 25000,
            "NUMBER_OF_TESTING_ENTRIES": 2000,
            "MIN_MAX_VOCAB_FREQ": 0.0,
            "MAX_MAX_VOCAB_FREQ": 0.8,
            "CANT_MAX_VOCAB_FREQ": 15}

print("Ejecutando ahora ejecutar_y_escribir_resultado_variando_alpha")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")