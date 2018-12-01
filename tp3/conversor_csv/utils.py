import csv
import os
import pandas as pd
import math
import subprocess

def parsear_output(output):
    delim = "---------------- Results -----------------"
    output = output[output.find(delim)+len(delim):].strip()
    output = output.split("\n")
    pairs = (elem.strip().split(": ") for elem in output)
    pairs = (p for p in pairs if len(p) == 2)
    return dict(pairs)

def ejecutar(cmd):
    print(cmd)
    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("utf-8")

def ejecutar_tp(input, output, dict_args):

    cmd = ["../../tp3", input, output]
    cmd += list(map(str,sum(dict_args.items(), ())))
    print(" ".join(cmd))

    popen = subprocess.Popen(cmd, stderr=subprocess.PIPE, stdout=subprocess.PIPE, universal_newlines=True)
    for stderr_line in iter(popen.stderr.readline, ""):
        yield stderr_line
    for stdout_line in iter(popen.stdout.readline, ""):
        yield stdout_line
    popen.stdout.close()
    return_code = popen.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, cmd)

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
        header = 'nombre_exp,nombre_img,n_cells,ray_type,ray_cnt,lsq,error,error_std,alpha,cache,tiempo_lsq_pre,tiempo_lsq,cant_autovalores,psnr'
        resultado.write(header + "\n")
        resultado.flush()
    else:
        resultado = open(nombre, 'a')
    return resultado

def escribir_resultados_en_archivo(resultado, nombre, img, n_cells, ray_type, ray_cnt, lsq, error, error_std, alpha, cache, output, psnr):
    tiempo_lsq = output["time-lsq"][:output["time-lsq"].find("ms")] if 'time-lsq' in output else 0
    tiempo_lsq_pre = output["time-lsqPreprocessing"][:output["time-lsqPreprocessing"].find("ms")]if 'time-lsqPreprocessing' in output else 0
    sing_val = output['sing-values'] if 'sing-values' in output else 0
    print(sing_val)
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
    line += str(alpha)
    line += ','
    line += str(cache)
    line += ','
    line += str(tiempo_lsq_pre)
    line += ','
    line += str(tiempo_lsq)
    line += ','
    line += str(sing_val)
    line += ','
    line += str(psnr)
    line += '\n'

    resultado.write(line)
    resultado.flush()
    print(">> {}".format(line))

# print("PSNR: " + str(psnr('../data/phantom3.csv', '../data/phantom3.csv')))
