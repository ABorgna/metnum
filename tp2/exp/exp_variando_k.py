
import numpy
from exp_base import *

def ejecutar_y_escribir_resultado_variando_alpha(exp_args):
    min_alpha = exp_args["MIN_ALPHA"]
    max_alpha = exp_args["MAX_ALPHA"]

    for alpha in list(numpy.linspace(min_alpha, max_alpha, exp_args["CANT_ALPHA"]))[1:-1]:
        program_args = {"--vocabulary": exp_args["VOCAB_FILE"],
                        "-t": exp_args["TRAINING_FILE"],
                        "-q": exp_args["TESTING_FILE"],
                        "-m": exp_args["METHOD_NUMBER"],
                        "--train-entries": exp_args["NUMBER_OF_TRAINING_ENTRIES"],
                        "--test-entries": exp_args["NUMBER_OF_TESTING_ENTRIES"],
                        "-a": alpha}
        output = ejecutar_con_args(program_args)
        print(parsear_output(output))
        print()

exp_args = {"CANT_ALPHA": 5,
            "MIN_ALPHA": 1,
            "MAX_ALPHA": 5,
            "VOCAB_FILE": "../data/vocab.csv",
            "TRAINING_FILE": "../data/imdb_tokenized.csv",
            "TESTING_FILE": "../data/imdb_tokenized.csv",
            "METHOD_NUMBER": 2,
            "NUMBER_OF_TRAINING_ENTRIES": 20,
            "NUMBER_OF_TESTING_ENTRIES": 20}

print("Ejecutando ahora ")
ejecutar_y_escribir_resultado_variando_alpha(exp_args)
print("Listo!")
