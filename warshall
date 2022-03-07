#define _CRT_SECURE_NO_WARNINGS //prototypes.h begin (for me)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "windows.h"

int size_counter(char *Path);
void file_open(int *A, char *Path, int Size);
void file_save(int *B, int Size);
void matrix_print(int *A, int Size);
void transitive(int *A, int Size);
void matrix_clearity(int *A, int *cA, int Size);
void editing(int *A, int i, int p, int Size); //prototypes.h end (for me)

#define ARROUT
#define INPUT

#define PATH "matrixes/graph_"
#define EXT ".txt"
#define PATH_S "matrixes/transitive.txt"

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
    int *CopyGraph = NULL;

    Graph = (int*)malloc(graph_size * graph_size * sizeof(int));
    CopyGraph = (int*)malloc(graph_size * graph_size * sizeof(int));

    file_open(Graph, PATH_O, graph_size);
    file_open(CopyGraph, PATH_O, graph_size);

    if ((format == 1) || (format == 3)) {
        system("cls");
        matrix_print(Graph, graph_size);
    }

    transitive(Graph, graph_size);
    matrix_clearity(Graph, CopyGraph, graph_size);

    free(CopyGraph);

    if ((format == 1) || (format == 3)) {
        matrix_print(Graph, graph_size);

        getchar(); getchar();
    }

    if ((format == 2) || (format == 3)) {
        system("cls");
        file_save(Graph, graph_size);
        printf("The adjacency matrix of the input graph was loaded from %s\nTransitive matrix is preparing...\n", PATH_O);

        Sleep(1000);

        printf("A file with a transitive matrix was saved on path:\n%s\n", PATH_S);

        getchar();
    }

    free(Graph);

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

    for (int i=0; i<Size; i++) {
        for (int k=0; k<Size; k++)
            fprintf(f, "%d ", *(B + i*Size + k));

        fprintf(f, "\n");
    }

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

void transitive(int *A, int Size) {
    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            if (*(A + i*Size + k) == 1)
                editing(A, k, i, Size);
}

void editing(int *A, int i, int p, int Size) {
    for (int j=0; j<Size; j++)
        if (*(A + i*Size + j) == 1) {
            *(A + p*Size + j) = 2;
            if (j != p)
                editing(A, j, p, Size);
        }
}

void matrix_clearity(int *A, int *cA, int Size) {
    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            if (*(A + i*Size + k) == 2)
                *(A + i*Size + k) = 1;

    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            if (*(A + i*Size + k) == *(cA + i*Size + k))
                *(A + i*Size + k) = 0;
}
