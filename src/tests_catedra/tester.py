import sys
import subprocess
import itertools
import time


def test(catedra_in, p):
    catedra_out = catedra_in + '.out'
    print("Testeando {} con p: {}".format(catedra_in, p))
    start = time.time()
    res = subprocess.check_output(
        "../tp1 {} {}; exit 0".format(catedra_in, p), shell=True).decode()
    end = time.time()
    print("El tiempo de ejecucion fue {}s".format(end - start))

    max_dif = 0.0
    with open(catedra_out, 'r') as catedra_file:
        for line_catedra, line_nuesto in itertools.zip_longest(catedra_file, res.splitlines()):
            if (line_nuesto == None or line_catedra == None):
                print("La cantidad de elementos es distinta")
            dif = abs(float(line_catedra) - float(line_nuesto))
            max_dif = max(dif, max_dif)
            if (dif >= 0.0001):
                print('='*15 + 'ERRORRR' + '='*15)
                print("Se encontre diferencia de {}".format(dif))
    print('la maxima diferencia fue: {}'.format(max_dif))
    print('='*60)


if __name__ == "__main__":
    if (len(sys.argv) == 3):
        test(sys.argv[1], sys.argv[2])
    elif (len(sys.argv) == 1):
        tests = [('test_15_segundos', 0.9), ('test_30_segundos', 0.8), ('test_aleatorio', 0.85),
                 ('test_aleatorio_desordenado', 0.76), ('test_trivial', 0.3),
                 ('test_completo', 0.5), ('test_sin_links', 0.64)]
        tests = [(s + '.txt', p) for (s, p) in tests]
        print(tests)
        for (s, p) in tests:
            test(s, p)
