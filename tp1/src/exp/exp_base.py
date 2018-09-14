import os
import shutil
import numpy
import subprocess

def parsear_output(output):
    sep = output.find("\n")
    return output[0:sep], output[sep+1:-1].split("\n") #p, ranks

def ejecutar_con_input(input_file, p):
    cmd = "../tp1 {} {}".format(input_file, str(p))
    # print(cmd)
    # output = subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")
    # print(output)
    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")
