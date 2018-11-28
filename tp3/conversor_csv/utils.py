import csv
import os
import pandas as pd
import math

def mse(img1, img2):
    rows = img1.shape[0]
    cols = img2.shape[1]

    val = (img1 - img2).apply(lambda x : x**2).values.sum()

    return val / (rows*cols)

def psnr(img1_rel_path, img2_rel_path):
    current_dir = os.path.dirname(os.path.realpath('__file__'))
    img1_abs_path = os.path.join(current_dir, img1_rel_path)
    img2_abs_path = os.path.join(current_dir, img2_rel_path)

    print("IMG1 abs. path: " + img1_abs_path)
    print("IMG2 abs. path: " + img2_abs_path)

    df1 = pd.read_csv(img1_abs_path)
    df2 = pd.read_csv(img2_abs_path)

    MAX = 255
    mse_val = mse(df1,df2)
    if mse_val == 0:
        return -1
    else:
        return 20*math.log10(MAX/math.sqrt(mse_val))



print("PSNR: " + str(psnr('../data/phantom3.csv', '../data/phantom3.csv')))