import csv
import os
import pandas as pd
import math
import subprocess

def parsear_output(output):
    delim = "---------------- Results -----------------"
    output = output[output.find(delim)+len(delim):].strip()
    output = output.split("\n")
    pairs = [elem.strip().split(": ") for elem in output]
    return dict(pairs)

def ejecutar(cmd):
    print(cmd)
    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")

def ejecutar_tp(input, output, dict_args):

    cmd = "../../tp3 " +" {} {} ".format(input, output) + " ".join("{} {}".format(key, val) for (key, val) in dict_args.items())
    print(cmd)

    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")


def mse(img1, img2):
    # Para imprimir matriz entera en pandas:
    # pd.set_option('display.max_columns', None)
    # pd.set_option('display.expand_frame_repr', False)
    # pd.set_option('max_colwidth', -1)
    assert img1.shape == img2.shape

    rows = img1.shape[0]
    cols = img1.shape[1]


    val = (img1 - img2).apply(lambda x : x**2).values.sum()

    return val / (rows*cols)

def psnr(img1_rel_path, img2_rel_path):
    current_dir = os.path.dirname(os.path.realpath('__file__'))
    img1_abs_path = os.path.join(current_dir, img1_rel_path)
    img2_abs_path = os.path.join(current_dir, img2_rel_path)

    # print("IMG1 abs. path: " + img1_abs_path)
    # print("IMG2 abs. path: " + img2_abs_path)

    df1 = pd.read_csv(img1_abs_path, header=None)
    df2 = pd.read_csv(img2_abs_path, header=None)

    MAX = 255
    mse_val = mse(df1,df2)
    if mse_val == 0:
        return -1
    else:
        return 20*math.log10(MAX/math.sqrt(mse_val))

def init_resultados(nombre = 'exp_res.csv'):
    if not os.path.isfile(nombre):
        resultado = open("./" + nombre, "w")
        header = 'nombre_exp,nombre_img,n_cells,ray_type,ray_cnt,lsq,error,error_std,tiempo_lsq,cant_autovalores,psnr'
        resultado.write(header + "\n")
    else:
        resultado = open(nombre, 'a')
    return resultado

def escribir_resultados_en_archivo(resultado, nombre, img, n_cells, ray_type, ray_cnt, lsq, error, error_std, output, psnr):
    tiempo_ms = output["time-lsq"][:output["time-lsq"].find("ms")]
    line = ''
    line += str(nombre)
    line += ','
    line += str(img)
    line += ','
    line += str(n_cells)
    line += ','
    line += str(ray_type)
    line += ','
    line += str(ray_cnt)
    line += ','
    line += str(lsq)
    line += ','
    line += str(error)
    line += ','
    line += str(error_std)
    line += ','
    line += str(tiempo_ms)
    line += ','
    line += str(0) # TODO: imprimir autovalores desde el tp3, y parsear
    line += ','
    line += str(psnr)
    line += '\n'

    resultado.write(line)
    print(">> {}".format(line))

# print("PSNR: " + str(psnr('../data/phantom3.csv', '../data/phantom3.csv')))