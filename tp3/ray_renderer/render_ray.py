#!/bin/env python3
import argparse
import csv
from math import sqrt
from PIL import Image, ImageDraw
import sys

class Ray:
    def __init__(self, start, end, values):
        self.start = start
        self.end = end

        width = int(sqrt(len(values)))
        assert(width * width == len(values))
        self.values = [values[i*width:(i+1)*width] for i in range(width)]

def parse_coords(string):
    [x,y] = string.strip('()"').split(',')
    return (float(x),float(y))

def read_rays(filename, maxEntries):
    rays = []
    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            start = parse_coords(row[0])
            end = parse_coords(row[1])
            values = row[2:]
            values = list(map(float, values))
            rays.append(Ray(start,end,values))

            if(maxEntries > 0 and len(rays) >= maxEntries):
                break
    return rays

def draw_checkboard(draw, ncells, color='#eee'):
    sz = draw.im.size
    width = sz[0] // ncells

    for y in range(0, sz[1], width):
        off = ((y//width) % 2) * width
        for x in range(off, sz[0], width*2):
            a = (x,y)
            b = (x+width,y+width)
            draw.rectangle([a,b], fill=color)

def draw_heatmap(draw, rays, invert):
    ncells = len(rays[0].values)
    sz = draw.im.size
    width = sz[0] // ncells

    cells = [[0. for x in range(ncells)] for y in range(ncells)]

    for ray in rays:
        sumVals = sum(map(sum, ray.values))
        for y in range(ncells):
            for x in range(ncells):
                cells[y][x] += ray.values[y][x] / sumVals

    maxCell = 0
    for y in range(ncells):
        for x in range(ncells):
            maxCell = max(maxCell, cells[y][x])

    for y in range(ncells):
        py = y * width
        for x in range(ncells):
            px = x * width

            a = (px,py)
            b = (px+width,py+width)
            color = int(round(255 * cells[y][x] / maxCell))
            if invert:
                color = 255 - color
            draw.rectangle([a,b], fill=(color,color,color))

def draw_ray(draw, ray, color='red', width=0):
    sz = draw.im.size[0]
    start = (ray.start[0] * sz, ray.start[1] * sz)
    end = (ray.end[0] * sz, ray.end[1] * sz)

    draw.line([start, end], fill=color, width=width)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Mostrar los gráficos del tp.')

    parser.add_argument('filename', type=str,
                        help='CSV con rayos generados por el tp.')
    parser.add_argument('image', type=str,
                        help='Imagen de salida.')
    parser.add_argument("-n", "--nrays", type=int, default=0,
                    help="Cantidad de rayos a procesar.")
    parser.add_argument("-c", "--cells", type=int, default=32,
                    help="Cantidad de celdas en la imagen.")

    parser.add_argument("--no-checkboard", dest="checkboard", action="store_false",
                    help="Dibujar un ajedrez de fondo.")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-b", "--background-img",
                    help="Imprimir una imagen de fondo")
    group.add_argument("--heatmap", action="store_true",
                    help="Dibujar un heatmap de incidencia.")
    parser.add_argument("--invert-heatmap", action="store_true",
                    help="Negro es mas alto en el heatmap.")

    parser.add_argument("-r", "--resolution", type=int, default=256,
                    help="Resolución de la imagen (cuadrada).")
    parser.add_argument("--no-rays", dest="render_rays", action="store_false",
                    help="No renderear las lineas de los rayos.")
    parser.add_argument("--ray-width", type=int, default=0,
                    help="Ancho de los rayos.")
    parser.add_argument("--ray-color", default='red',
                    help="Color de los rayos.")

    args = parser.parse_args()

    # Procesar los rayos de entrada
    rays = read_rays(args.filename, args.nrays)

    # Crear una imagen vacia
    if args.background_img is not None:
        try:
            img = Image.open(args.background_img)
        except IOError:
            print("Could not open the image \"" + str(args.background_img) + "\".")
            sys.exit(1)

        img = img.resize((args.cells, args.cells), resample = Image.LANCZOS)
        img = img.resize((args.resolution, args.resolution), resample = Image.NEAREST)
    else:
        img = Image.new('RGB', (args.resolution, args.resolution), color='white')
    draw = ImageDraw.Draw(img)

    # Dibujar el heatmap
    if args.heatmap:
        draw_heatmap(draw, rays, args.invert_heatmap)

    # Dibujar el tablero de ajedrez
    if args.checkboard and args.background_img is None and not args.heatmap:
        draw_checkboard(draw, args.cells)

    # Dibujar los rayos
    if args.render_rays:
        for ray in rays:
            draw_ray(draw, ray, args.ray_color, args.ray_width)

    del draw
    img.save(args.image)
