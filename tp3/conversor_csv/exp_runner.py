from utils import *
from csv_converter import convertImgs, convertImg
import numpy
import shutil
import json

# LEEEEMEEEEEEEEE (para correr)
# 1. borrar imgs en imgs_input
# 2. generar imgs a correr en imgs_input: python csv_converter.py ../data/ imgs_input/ .png 32
# los archivos tienen que ser de la forma "nombre-tamaño.csv" (el conversor los genera asi por default)
# 2. correr y buscar resultados del exp. en carpeta resultados =)



def ejecutar_y_escribir_resultado(exp_args):
    print("\n-----------CREATE IMAGES----------------")

    if not os.path.exists('../imgs_input'):
        os.mkdir('../imgs_input')
    for t in exp_args['n_cells']:
        convertImgs('../../data', '../imgs_input', '.png', tam = t)
    if not os.path.exists('./cache'):
            os.mkdir('./cache')


    res = init_resultados()
    for img in exp_args['imgs']:
        for tam in exp_args['n_cells']:
            input_img = '../imgs_input/' + img + '-' + str(tam) + '.csv'
            for ray_type in exp_args['rays_types']:
                for ray_cnt in exp_args['rays_cnt']:
                    for lsq in exp_args['lsq']:
                        for error_type in exp_args['error_type']:
                            for error_std in exp_args['error_std']:
                                for alpha in exp_args['alpha']:
                                    for cache in exp_args['cache']:
                                        for rep in range(exp_args['reps']):
                                            output_img = '-'.join(map(str, 
                                                [img, tam, ray_type, ray_cnt, lsq, error_type, error_std, alpha, cache, rep]))
                                            output_img += '.csv'
                                            program_args = {'-n' : tam,
                                                            '--ray-type': ray_type,
                                                            '--ray-count': ray_cnt,
                                                            '-l' : lsq,
                                                            '-e' : error_type,
                                                            '-E' : error_std,
                                                            '-a' : alpha}
                                            if cache == 0:
                                                program_args['--no-cache'] = ''

                                            output = ejecutar_tp(input_img, output_img, program_args)
                                            print(output)
                                            parsed_output = parsear_output(output)

                                            convertImg(output_img, './', './', '.csv', extension_salida = '.png', tam = None)

                                            value_psnr = psnr(output_img, input_img)
                                            escribir_resultados_en_archivo(res,
                                                exp_args['nombre'], img, tam, ray_type, ray_cnt, lsq,
                                                error_type, error_std, alpha, cache, parsed_output, value_psnr)



    res.close()




with open("exp.json", "r") as read_file:
    exp_args = json.load(read_file)
exp_args['rays_cnt'] = list(range(exp_args['rays_cnt']['from'], exp_args['rays_cnt']['to'] + 1, exp_args['rays_cnt']['step']))
print(exp_args)

print("Ejecutando exp...")
ejecutar_y_escribir_resultado(exp_args)
print("Listo!")