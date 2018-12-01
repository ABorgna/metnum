# TP3: Tomografía Computada
Métodos Numéricos (UBA) - 2do cuatrimestre 2018

## Compilación

Simplemente correr `make tp3`. Esto genera un ejecutable `./tp3`.

## Ejecución

Correr el programa seteando los parámetros buscados.
Por ejemplo, `TODO`

Parámetros posibles:

```
TODO
```

## Experimentos

La experimentación requiere la librería `Pillow`.
Para instalarla, correr `pip install Pillow`.

## Renderear los rayos

Correr el tp con `--ray-file rayos.csv` (y `--no-lsq` para que no tarde banda)
así genera los rayos que uno quiere y los guarda en ese archivo.

Después pasarselo al renderer:

```
ray_renderer/render_ray.py rayos.csv rayos.png --ray-count 100
```

Ver `ray_renderer/render_ray.py -h` para mas parámetros.

## Links

Otras distribuciónes de error:
  - https://ned.ipac.caltech.edu/level5/Stetson/Stetson3_2.html
  - http://www.cs.rug.nl/~roe/publications/fmrinoiseIJBI.pdf
