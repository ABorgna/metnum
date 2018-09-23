import os
import shutil
import numpy
import subprocess

def parsear_output(output):
    sep = output.find("\n")
    return output[0:sep], output[sep+1:-1].split("\n") #p, ranks

def ejecutar_con_args(args):
    cmd = "../tp1 " + " ".join([str(arg) for arg in args])
    # print(cmd)
    # output = subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")
    # print(output)
    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")
