import sys

archivo_nombre = (sys.argv)[1]
p = (sys.argv)[2]

archivo = open(archivo_nombre, "r")
n = int(archivo.readline())
print(str(p))
for i in range(0, n):
    print(i)