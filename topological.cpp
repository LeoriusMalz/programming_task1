#define _CRT_SECURE_NO_WARNINGS //prototypes.h begin (for me)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "windows.h"

int size_counter(char *Path);
int sum(int *A, int Size);
void file_open(int *A, char *Path, int Size);
void file_save(int *B, int Size);
void clr_init(int *Clr, int Size);
void matrix_print(int *A, int Size);
void array_print(int *A, int Size);
void top_sort(int *A, int *Str, int *Clr, int Size);
void append(int *A, int ch); //prototypes.h end (for me)

#define ARROUT
#define INPUT1

#define PATH "matrixes/graph_"
#define EXT ".txt"
#define PATH_S "matrixes/sequence.txt"

int main(int argc, char **argv) {
    int number = 0;
    int format = 0;

#ifdef INPUT
    char *p;

    if (argc != 3) {
        printf("Error: %d argument received, 2 was expected.\n", argc-1);

        getchar();
        return 0;
    }

    number = strtol(argv[1], &p, 10);
    format = strtol(argv[2], &p, 10);
#endif

#ifndef INPUT
    scanf("%d %d", &number, &format);
#endif

    char path[30];

    snprintf(path, sizeof(path), "%s%d%s", PATH, number, EXT);

    char *PATH_O = path;
    int graph_size = size_counter(PATH_O);

    int *Graph = NULL;
    int *Seq = NULL;
    int *Color = NULL;

    Graph = (int*)malloc(graph_size * graph_size * sizeof(int));
    Seq = (int*)malloc(graph_size * sizeof(int));
    Color = (int*)malloc(graph_size * sizeof(int));

    file_open(Graph, PATH_O, graph_size);
    clr_init(Color, graph_size);

    if ((format == 1) || (format == 3)) {
        system("cls");
        matrix_print(Graph, graph_size);
    }

#ifndef ARROUT
    array_print(Color, graph_size);
    array_print(Seq, graph_size);
#endif

    top_sort(Graph, Seq, Color, graph_size);

#ifndef ARROUT
    array_print(Color, graph_size);
    array_print(Seq, graph_size);
#endif

    free(Graph);
    free(Color);

    if (((format == 1) || (format == 3)) && Color[0] != -1) {
        array_print(Seq, graph_size);

        getchar(); getchar();
    }

    if (((format == 2) || (format == 3)) && Color[0] != -1) {
        system("cls");
        file_save(Seq, graph_size);
        printf("The adjacency matrix of the input orient graph was loaded from %s\nTopological sort sequence is preparing...\n", PATH_O);

        Sleep(1000);

        printf("A file with a sequence of vertexes was saved on path:\n%s\n", PATH_S);

        getchar();
    }

    if (Color[0] == -1) {
        printf("Error: the graph cannot be sorted by topological sorting, cycles exist.");
        getchar(); getchar();
    }

    free(Seq);

    return 0;
}

int size_counter(char *Path) {
    int counter = 0;
    FILE *f;

    f = fopen(Path, "r");

    fseek(f, 0, SEEK_SET);

    while (true) {
        int value = 0;

        if (fscanf(f, "%d", &value) == 1)
            counter++;

        if (feof(f))
            break;
    }

    fseek(f, 0, SEEK_SET);

    fclose(f);

    return ((int)sqrt(counter));
}

void file_open(int *A, char *Path, int Size) {
    FILE *f;

    f = fopen(Path, "r");

    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            fscanf(f, "%d", (A + i*Size + k));

    fclose(f);
}

void file_save(int *B, int Size) {
    FILE *f;

    f = fopen(PATH_S, "w");

    for (int i=0; i<Size; i++)
        fprintf(f, "%d ", B[i]);

    fclose(f);
}

void matrix_print(int *A, int Size) {
    for (int i=0; i<Size; i++) {
        for (int k=0; k<Size; k++)
            printf("%d ", *(A + i*Size + k));

        printf("\n");
    }

    printf("\n");
}

void array_print(int *A, int Size) {
    for (int i=0; i<Size; i++)
        printf("%d ", A[i]);

    printf("\n");
}

void top_sort(int *A, int *Str, int *Clr, int Size) {
    int k = 0;

    if (check(A, Size) != 0)
        Clr[0] = -1;

    else
        while (k != Size) {
            k = sum(Clr, Size);

            for (int j=0; j<Size; j++) {
                int sumcol = 0;

                for (int i=0; i<Size; i++)
                    sumcol += *(A + i*Size + j);

                if ((sumcol == 0) && (Clr[j] == 0)) {
                    Clr[j] = 1;
                    append(Str, j+1);

                    for (int i=0; i<Size; i++)
                        *(A + j*Size + i) = 0;
                }
            }
        }
}

int check(int *A, int Size) {
    int sumcol = 0;
    int sum = 1;

    for (int i=0; i<Size; i++) {
        for (int j=0; j<Size; j++)
            sumcol += *(A + i*Size + j);

        sum *= sumcol;
        sumcol = 0;
    }

    return sum;
}

int sum(int *A, int Size) {
    int sum = 0;

    for (int i=0; i<Size; i++)
        sum += A[i];

    return sum;
}

void clr_init(int *Clr, int Size) {
    for (int i=0; i<Size; i++)
        Clr[i] = 0;
}

void append(int *A, int ch) {
    static int j = 0;

    A[j] = ch;
    j++;
}
