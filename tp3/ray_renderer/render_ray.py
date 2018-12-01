import argparse
import csv
from PIL import Image, ImageDraw
import sys

class Ray:
    def __init__(self, start, end, values):
        self.start = start
        self.end = end
        self.values = values

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

    parser.add_argument("--checkboard", action="store_true",
                    help="Dibujar un ajedrez de fondo.")

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
    img = Image.new('RGB', (args.resolution, args.resolution), color='white')
    draw = ImageDraw.Draw(img)

    # Dibujar el tablero de ajedrez
    if args.checkboard:
        draw_checkboard(draw, args.cells)

    # Dibujar los rayos
    if args.render_rays:
        for ray in rays:
            draw_ray(draw, ray, args.ray_color, args.ray_width)

    del draw
    img.save(args.image)
