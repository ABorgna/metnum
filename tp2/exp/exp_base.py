# import os
# import shutil

import subprocess
import time

def parsear_output(output):
    sep = "---------------- Testing -----------------"
    output = output[output.find(sep) + len(sep) + 1:-1].split("\n") # ['param: val', ..., 'param: val']
    output = [elem.split(": ") for elem in output]                  # [['param','val'], ..., ['param', 'val]]
    output = dict(output)                                           # {'param': val', ..., 'param': val}
    return output

def ejecutar_con_args(args):
    """ args: diccionario """

    cmd = "../tp2 " + " ".join("{} {}".format(key, val) for (key, val) in args.items())
    print(cmd)
    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")

def tomar_tiempo_ejecucion_con_args(args):
    before = time.time_ns()
    ejecutar_con_args(args)
    after = time.time_ns()
    return after - before
