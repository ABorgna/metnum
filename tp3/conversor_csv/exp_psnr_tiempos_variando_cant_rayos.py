from utils import *
import numpy
import shutil


# LEEEEMEEEEEEEEE (para correr)
# 1. borrar imgs en imgs_input
# 2. generar imgs a correr en imgs_input: python csv_converter.py ../data/ imgs_input/ .png 32
# los archivos tienen que ser de la forma "nombre-tamaño.csv" (el conversor los genera asi por default)
# 2. correr y buscar resultados del exp. en carpeta resultados =)

def escribir_resultados_en_archivo(resultado, img, output, psnr, cant_rayos):
    tiempo_ms = output["time-lsq"][:output["time-lsq"].find("ms")]
    resultado.write("{},{},{},{}\n".format(img, cant_rayos, psnr,tiempo_ms))
    print(">> {},{},{},{}".format(img, cant_rayos, psnr,tiempo_ms))

def listar_archivos_en_carpeta(path):
    filenames = []
    for root, dirs, files in os.walk(path):
        for filename in files:
            filenames = filenames + [filename]
    return filenames

def ejecutar_y_escribir_resultado(exp_args):
    print("\n-----------CLEAN AND CREATE FOLDERS----------------")

    if os.path.exists(exp_args["IMGS_DIR_OUTPUT"]):
        shutil.rmtree(exp_args["IMGS_DIR_OUTPUT"])

    if not os.path.exists("./resultados/"):
        os.mkdir("./resultados/")
        print("Nueva carpeta: ./resultados/")

    os.mkdir(exp_args["IMGS_DIR_OUTPUT"])
    print("Re-creada:" + exp_args["IMGS_DIR_OUTPUT"])


    print("\n------------------FETCH INPUT----------------------")
    print("Leyendo imagenes en carpeta " + exp_args["IMGS_DIR_INPUT"])
    imgs = listar_archivos_en_carpeta(exp_args["IMGS_DIR_INPUT"])
    print("Imagenes encontradas: " + ", ".join(imgs))
    print("\n------------------ EXECUTION-----------------------")

    output_name = "resultados/psnr-tiempos-variando-rayos-data.csv"
    print("OUTPUT_FILE: {}\n".format(output_name))
    resultado = open("./" + output_name, "w")

    header = "nombre_imagen,cant_rayos,psnr,tiempo_ms"
    resultado.write(header + "\n")
    print(">> " + header)

    min = exp_args["MIN_RAYS_COUNT"]
    max = exp_args["MAX_RAYS_COUNT"]
    count = exp_args["COUNT_RAYS_COUNT"]

    img_count = 1
    id = 0
    for img in imgs:
        dimension = img.split("-")[-1][:-4]
        print("({}/{}) Ejecutando imagen {}".format(img_count, len(imgs), img))
        img_count += 1

        for rays_count in list(numpy.linspace(min, max, count).astype(int)):

            input_img = exp_args["IMGS_DIR_INPUT"] + img
            output_img =  exp_args["IMGS_DIR_OUTPUT"] + img + "-output-" + str(id)

            program_args = {input_img: output_img,
                            "--ray-count": rays_count,
                            "--ray-type": exp_args["RAY_TYPE"],
                            # "--cache": exp_args["CACHE_FOLDER"],
                            "-n": dimension}

            output = ""
            for line in ejecutar_tp(input_img, output_img, program_args):
                output += line
                print(line, end="")
            parsed_output = parsear_output(output)

            value_psnr = psnr(output_img, exp_args["IMGS_DIR_INPUT"] + img)
            escribir_resultados_en_archivo(resultado, img, parsed_output, value_psnr, rays_count)

            id +=1
        print()
    resultado.close()

exp_args = {"IMGS_DIR_INPUT": "./imgs_input/",
            "IMGS_DIR_OUTPUT": "./imgs_output/",
            "RAY_TYPE": 0,
            "MIN_RAYS_COUNT": 10,
            "MAX_RAYS_COUNT": 30000,
            "COUNT_RAYS_COUNT": 20}

print("Ejecutando exp...")
ejecutar_y_escribir_resultado(exp_args)
print("Listo!")
