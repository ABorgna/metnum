# TP2: Análisis de sentimientos
Métodos Numéricos (UBA) - 2do cuatrimestre 2018

## Compilación

Simplemente correr `make`. Esto genera un ejecutable `./tp2`.

## Ejecución

Correr el programa seteando los parámetros buscados.
Por ejemplo, `./tp2 -m1 --maxVocabFreq 0.15 -k50`

Parámetros posibles:

```
Usage: ./tp2 [OPTIONS]

Options:
    -h, --help     Show this help message.
    -v, --verbose  Print debug info to stderr.
        --quiet    Do not print debug info.
        --analyze  Save analysis on the analyze/ folder.
    -j <threads>   Number of threads to utilize. (Default: # of cores)

  MODEL
    -m #           Method:
                     0: kNN.
                     1: Inverted index kNN.
                     2: PCA + kNN.
                     3: PCA + Inverted Index kNN (default).
    -k #           K hyper-parameter for kNN (Default: 20)
    -a #           Alpha hyper-parameter for PCA (Default: 10)
    -n, --norm <norm>
                   Norm used in the kNN:
                     #: p-norm (default: 1)
                     INF: L∞-norm
                     CHI2: Chi-squared norm
                     RANDOM: Random norm (yay!)
    -Q, --no-test  Only run the training step. Save the model using -c.
    -C, --cache <path>
                   Directory for storing and reading the trained model cache.
        --no-cache
                   Don't use a cache for the model.
    -c, --classif-file <file>
                   Output the classification results for each test case.
                   Use '-' for stdout.

  ENTRIES
    -d <file>      File with training and testing set. Use '-' for stdin.
                   (Default: 'data/imdb_tokenized.csv')
    -t <file>      File with the training set. Use '-' for stdin.
                   (Default: 'data/imdb_tokenized.csv')
    -q <file>      File with the testing set. Use '-' for stdin.
                   (Default: 'data/imdb_tokenized.csv')
    -o <file>      Output file with the test entries result. Use '-' for stdout.
                   (Default: stdout)
        --train-entries n
                   Train at most n entries.
        --test-entries n
                   Test at most n entries.

  VOCABULARY
    -p, --vocabulary <file>
                   File with the vocabulary.
                   (Default: 'data/vocab.csv')
        --minVocabFreq <number>
                   Trim the tokens in the vocabulary with small frequency.
                   (Default: 0.01)
        --maxVocabFreq <number>
                   Trim the tokens in the vocabulary with large frequency.
                   (Default: 0.99)
```
