#define _CRT_SECURE_NO_WARNINGS //prototypes.h begin (for me)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "windows.h"

int size_counter(char *PATH_O);
void file_open(int *A, char *PATH_O, int Size);
void file_save(int *B, int Size);
void point_init(int *Pnt, int Size);
void clr_init(int *Clr, int *A, int Size);
void matrix_print(int *Matr, int Size);
void array_print(int *Arr, int Size);
void spanning_tree(int *Matr, int *Clr, int *Pnt, int Size);
void clearity(int *Matr, int n, int Size);
void morphos(int *Matr, int n, int Size);
int multiply(int *Arr, int Size); //prototypes.h end (for me)

#define ARROUT
#define INPUT

#define PATH "matrixes/graph_"
#define EXT ".txt"
#define PATH_S "matrixes/spanning_tree.txt"

int main(int argc, char **argv) {
    long int number = 0;
    long int format = 0;
    char *p;

#ifdef INPUT
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

    snprintf(path, sizeof(path), "%s%ld%s", PATH, number, EXT);

    char *PATH_O = path;
    int tree_size = size_counter(PATH_O);

    int *A = NULL;
    int *Color = NULL;
    int *Points = NULL;

    A = (int*)malloc(tree_size * tree_size * sizeof(int));
    Color = (int*)malloc(tree_size * tree_size * sizeof(int));

    Points = (int*)malloc(tree_size * sizeof(int));

    file_open(A, PATH_O, tree_size);
    clr_init(Color, A, tree_size);
    point_init(Points, tree_size);

    if ((format == 1) || (format == 3))
        matrix_print(A, tree_size);

#ifndef ARROUT
    array_print(Points, tree_size);
#endif

    spanning_tree(A, Color, Points, tree_size);

#ifndef ARROUT
    array_print(Points, tree_size);
#endif

    free(Points);
    free(Color);

    if ((format == 1) || (format == 3)) {
        matrix_print(A, tree_size);

        getchar(); getchar();
    }

    if ((format == 2) || (format == 3)) {
        system("cls");
        file_save(A, tree_size);
        printf("The adjacency matrix of the input tree was loaded from %s\nSpanning tree is preparing...\n", PATH_O);

        Sleep(1000);

        printf("The file with the adjacency matrix of the spanning tree was saved on path:\n%s\n", PATH_S);

        getchar();
    }

    free(A);

    return 0;
}

int size_counter(char *PATH_O) {
    int counter = 0;
    FILE *f;

    f = fopen(PATH_O, "r");

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

void file_open(int *A, char *PATH_O, int Size) {
    FILE *f;

    f = fopen(PATH_O, "r");

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

void matrix_print(int *Matr, int Size) {
    for (int i=0; i<Size; i++) {
        for (int k=0; k<Size; k++)
            printf("%d ", *(Matr + i*Size + k));

        printf("\n");
    }

    printf("\n");
}

void array_print(int *Arr, int Size) {
    for (int i=0; i<Size; i++)
        printf("%d ", Arr[i]);

    printf("\n");
}

void clr_init(int *Clr, int *A, int Size) {
    *(Clr + 0*Size + 0) = 0;

    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            if (*(A + i*Size + k) != 0)
                *(Clr + i*Size + k) = 1;
}

void point_init(int *Pnt, int Size) {
    for (int i=0; i<Size; i++)
        Pnt[i] = 0;
}

void spanning_tree(int *Matr, int *Clr, int *Pnt, int Size) {
    Pnt[0] = 1;
    morphos(Clr, 0, Size);
    clearity(Clr, 0, Size);

    while (multiply(Pnt, Size) != 1) {
        int minimum = 1000000;
        int X = 0;
        int Y = 0;

        for (int i=0; i<Size; i++)
            for (int k=0; k<Size; k++)
                if ((*(Clr + i*Size + k) == 2) && (*(Matr + i*Size + k) <= minimum)) {
                    minimum = *(Matr + i*Size + k);
                    X = i;
                    Y = k;
                }

        *(Clr + X*Size + Y) = 3;
        morphos(Clr, Y, Size);
        clearity(Clr, Y, Size);
        Pnt[Y] = 1;
    }

    for (int i=0; i<Size; i++)
        for (int k=0; k<Size; k++)
            if ((*(Clr + i*Size + k) == 0) && (*(Clr + k*Size + i) == 0)) {
                *(Matr + i*Size + k) = 0;
                *(Matr + k*Size + i) = 0;
            }
}

void clearity(int *Matr, int n, int Size) {
    for (int i=0; i<Size; i++)
        if (*(Matr + i*Size + n) != 3)
            *(Matr + i*Size + n) = 0;
}

void morphos(int *Matr, int n, int Size) {
    for (int i=0; i<Size; i++)
        if (*(Matr + n*Size + i) == 1)
            *(Matr + n*Size + i) = 2;
}

int multiply(int *Arr, int Size) {
    int mult = 1;

    for (int i=0; i<Size; i++)
        mult = mult * Arr[i];

    return mult;
}
