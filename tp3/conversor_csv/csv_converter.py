# -*- coding: utf-8 -*-

from PIL import Image
import numpy as np
import sys
import os
import csv
import pydicom


#Useful function
def createFileList(myDir, format='.png'):
    fileList = []
    for root, dirs, files in os.walk(myDir, topdown=False):
        for name in files:
            if name.endswith(format):
                fullName = os.path.join(root, name)
                fileList.append(fullName)
    return fileList

def convertImg(infile, input_folder, output_folder, extension, extension_salida = '.csv', tam = None):
    print(infile)
    if extension == '.dcm':
        img_file = Image.fromarray(pydicom.read_file(infile).pixel_array)
    elif extension == '.csv':
        img_file = Image.fromarray(np.genfromtxt(infile, delimiter=','))
        img_file = img_file.convert('RGB')
        tam = None # si lo uso para ir de CSV a png, no darle bola a esto
    else:
        img_file = Image.open(infile)

    # get original image parameters...
    width, height = img_file.size
    new_size = min(width, height)
    new_size = min(new_size, tam) if tam is not None else new_size
    format = img_file.format
    mode = img_file.mode

    # Make image Greyscale
    if mode == 'RGBA' or mode == 'RGB':
        img_grey = img_file.convert('L')
    else:
        img_grey = img_file

    # Make square image.
    img_grey = img_grey.resize((new_size, new_size), Image.ANTIALIAS)

    # Save Greyscale values
    value = np.asarray(img_grey.getdata(), dtype=np.int).reshape((img_grey.size[1], img_grey.size[0]))

    if extension_salida == '.csv':
        name = output_folder + infile.replace(extension, "").replace(input_folder, "") + ("-" + str(tam) if tam is not None else "") + ".csv"
        with open(name, 'w') as f:
            writer = csv.writer(f)
            for v in value:
                writer.writerow(v)
        return name

    else: # any other supported image file format
        name = output_folder + infile.replace(extension, "").replace(input_folder, "") + extension_salida
        # with open(name, 'w') as f:
        img_grey.save(name)
        return name


def convertImgs(input_folder, output_folder, extension, extension_salida = '.csv', tam = None):
    # load the original image
    myFileList = createFileList(input_folder, extension)
    for file in myFileList:
        convertImg(file, input_folder, output_folder, extension, extension_salida, tam)

if __name__ == "__main__":

    if len(sys.argv) < 4:
        print("Correr python csv_converter.py <carpeta_imagenes> <carpeta_output> <extension_imagenes> <tamano>.")
        print("Por ejemplo: python csv_converter.py imagenes/ imagenes_convertidas/ .png 16")
        print("Toma todas las imagenes de imagenes/ en format .png y las convierte a")
        print("formate .csv, con un tamaño de 16x16. El tamano default es el minimo que la haga cuadrada.")
        exit(0)

    input_folder = sys.argv[1]
    output_folder = sys.argv[2]
    extension = sys.argv[3]
    tamano = int(sys.argv[4]) if len(sys.argv) >= 5 else None
    extension_salida = sys.argv[5] if len(sys.argv) == 6 else ".csv"

    print("Extension: " + extension)
    print("Carpeta salida: " + output_folder)
    print("Carpeta entrada: " + input_folder)

    convertImgs(input_folder, output_folder, extension, extension_salida, tamano)